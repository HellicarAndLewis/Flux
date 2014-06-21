//
//  RenderEngine.cpp
//  FluxOF
//
//  Created by Patricio Gonzalez Vivo on 6/14/14.
//
//

#include "RenderEngine.h"

void RenderEngine::selfSetup(){
    cameraEnable(false);
    setupRenderIsFlipped(true);
    
    syphon.set("Photoshop", "psGrabberDebug");
    syphon.setup();
    
}

void RenderEngine::selfSetupRenderGui(){
    rdrGui->addToggle("TERRAIN 1 SYPHON", &terrainMaskSyphon);
}


void RenderEngine::selfUpdate(){
    setupNumViewports(3);

}

void RenderEngine::setCalibration(CalibrationLoader *_calibration){
    calibration = _calibration;
    
}

void RenderEngine::addUIClass(UIClass *_ui){
    guiAdd( *_ui );
}



//---------------------------------------------------------------

void RenderEngine::startTransitionTo(QueueItem queueItem){
    text = "123456789012345";//"@adidas";//queueItem.username;
    
    //  Extract Colors
    //
    srcPalette = dstPalette;
    dstPalette = ofxColorPalette::getColorPalette(queueItem.image, 3);
    
    //  Keep the image as a destinationTexture
    //
    int size = 1024;
    float optimalWidth = 0.7;
    float optimalHeight = 0.95;
    
    int width, height;
    if(queueItem.image.getWidth()/optimalWidth > queueItem.image.getHeight()/optimalHeight){
        height = size * optimalHeight;
        width = height * queueItem.image.getWidth() /  queueItem.image.getHeight();
    } else {
        width = size * optimalWidth;
        height = width * queueItem.image.getHeight() /  queueItem.image.getWidth();
    }
    
    ofDisableArbTex();
    shoeTex.swap();
    
    if(!shoeTex.dst->isAllocated() || shoeTex.dst->getWidth() != size){
        shoeTex.dst->allocate(size,size);
    }
    shoeTex.dst->begin();{
        ofClear(0);
        ofSetColor(255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        queueItem.image.draw(size/2.0,size/2.0, -width, -height);
        ofSetRectMode(OF_RECTMODE_CORNER);
    } shoeTex.dst->end();
    
    ofEnableArbTex();
    
    //  Start Animation
    //
    timeline->setPercentComplete(0.0);
    timeline->play();
}

//
// This will draw a mask image on the terrain that is multiplied to the background.
//
void RenderEngine::drawMask(int view){
    ofSetColor(255);
    int s = 1024;

    if(terrainMaskSyphon ){
        
        //Draw the map with syphon input
        //
        if(view == 1){
            ofClear(255);
              glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ONE, GL_ONE);
            syphon.bind();

            glBegin(GL_QUADS);{
                glTexCoord2d(0, 0); glVertex2d(0, s);
                glTexCoord2d(s, 0); glVertex2d(s, s);
                glTexCoord2d(s, s); glVertex2d(s, 0);
                glTexCoord2d(0, s); glVertex2d(0, 0);
            }glEnd();
            
            syphon.unbind();
            
        } else {
            syphon.draw(0, 0, s, s);
        }
    }
    else if(view > 0){

        //Draw the mask with the loaded texture
        //
        ofTexture * _tex;
        if(view == 1){
            _tex = &assets->terrainMask1;
        } else {
            _tex = &assets->terrainMask2;
        }
        _tex->bind();
        s = _tex->getWidth();
        
        glBegin(GL_QUADS);{
            glTexCoord2d(0, 0); glVertex2d(0, 0);
            glTexCoord2d(s, 0); glVertex2d(s, 0);
            glTexCoord2d(s, s); glVertex2d(s, s);
            glTexCoord2d(0, s); glVertex2d(0, s);
        }glEnd();
        _tex->unbind();
        
    } else {
        // No mask (simulator)
        //
        ofClear(255);
    }

    ofEnableAlphaBlending();
}

//
// Main draw loop, done once for every window
//
void RenderEngine::draw(ofEventArgs & args){
    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    int viewNum = glfw->getWindowIndex();
    
    if(bRenderSystem){
        {
            currentViewPort = viewNum;
            ofPushStyle();
            
            getRenderTarget(viewNum).begin();
            //  Background
            //
            if ( background != NULL ){
                background->draw();
            }
            
            //  2D scene
            //
            ofPushStyle();
            ofPushMatrix();
            selfDrawBackground();
            ofPopMatrix();
            ofPopStyle();
            
            //  Start 3D scene
            //
            {
                if(viewNum == 0){
                    getCameraRef().begin();
                }
                fog.begin();
                
                
                //  Cached Values
                //
                glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.getPtr());
                glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix.getPtr());
                glGetDoublev(GL_PROJECTION_MATRIX, matP);
                glGetDoublev(GL_MODELVIEW_MATRIX, matM);
                glGetIntegerv(GL_VIEWPORT, viewport);
                
                //            glEnable(GL_DEPTH_TEST);
                ofEnableDepthTest();
                //            glDepthMask(false);
                
                if (bEdit){
                    lightsDraw();
                }
                
                //  Draw Debug
                //
                if( bDebug ){
                    ofPushStyle();
                    ofPushMatrix();
                    
                    selfDrawDebug();
                    
                    ofPopMatrix();
                    ofPopStyle();
                }
                
                //  Draw Scene
                //
                {
                    //lightsBegin();
                    ofPushStyle();
                    ofPushMatrix();
                    
                    selfDraw();
                    
                    ofPopMatrix();
                    ofPopStyle();
                   // lightsEnd();
                }
                
                //  Draw Log
                //
                {
                    ofPushStyle();
                    ofDisableLighting();
                    ofSetColor(background->getUIBrightness()*255.0);
                    logGui.draw();
                    ofPopStyle();
                }
                
                //            glDepthMask(true);
                ofDisableDepthTest();
                //            glDisable(GL_DEPTH_TEST);
                fog.end();
                
                //  Update Mouse
                //
                if (bUpdateCursor){
                    unprojectCursor(cursor, ofGetMouseX(), ofGetMouseY());
                    bUpdateCursor = false;
                }

                
                if(viewNum == 0){
                    getCameraRef().end();
                }
            }
            
            //  Draw Overlay
            //
            if(viewNum==0){
                ofPushStyle();
                ofPushMatrix();
                selfDrawOverlay();
                ofPopMatrix();
                ofPopStyle();
            }
            
            
            getRenderTarget(viewNum).end();
            
        }
        //  Post-Draw ( shader time )
        //
        ofDisableLighting();
        selfPostDraw();
        
        logGui.drawStatus();
        
        ofPopStyle();
    }
    
    
	if(timeline != NULL && viewNum == 0){
        
        if ( timeline->getIsShowing() ){
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            ofPushStyle();
            ofFill();
            ofSetColor(0,100);
            ofRect(4, ofGetHeight() - timeline->getHeight() - 4, ofGetWidth() - 8, timeline->getHeight());
            ofPopStyle();
        }
        
		timeline->draw();
	}
}


ofFbo& RenderEngine::getRenderTarget(int viewNumber){
    ofFbo *renderTarget = &renderTargets[viewNumber];
    int width = ofGetWidth() ;
    int height = ofGetHeight();
    if(!renderTarget->isAllocated() || renderTarget->getWidth() != width || renderTarget->getHeight() != height){
        ofFbo::Settings settings;
        settings.width = width;
        settings.height = height;
        settings.internalformat = GL_RGB;
        settings.numSamples = 0;
        settings.useDepth = true;
        settings.useStencil = true;
        settings.depthStencilAsTexture = true;

        settings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
        renderTarget->allocate(settings);

		renderTarget->begin();
		ofClear(0,0,0,0);
		renderTarget->end();
    }
    
    return *renderTarget;
}


void RenderEngine::selfPostDraw(){
    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    
    int i = glfw->getWindowIndex();
    
    if(i == 0){
        getRenderTarget(i).draw(0
                                , 0
                                , getRenderTarget(i).getWidth()
                                , getRenderTarget(i).getHeight());
    } else {
        getRenderTarget(i).draw(0
                                , getRenderTarget(i).getHeight()
                                , getRenderTarget(i).getWidth()
                                , -getRenderTarget(i).getHeight());
    }
}

bool RenderEngine::transitionDone(){
    return timeline->getPercentComplete() == 1;
}