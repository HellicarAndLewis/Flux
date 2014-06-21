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
    
    ofDisableArbTex();
    terrainTexture.allocate(2048, 2048);
    terrainMaskTexture.allocate(1024, 1024);
    shoeBackgroundTexture.allocate(1024, 1024);
    shoeForegroundTexture.allocate(1024, 1024);
    ofEnableArbTex();
    
    
}

void RenderEngine::selfSetupRenderGui(){
    rdrGui->addToggle("TERRAIN 1 SYPHON", &terrainMaskSyphon);
    
    rdrGui->addLabel("Render stages");
    rdrGui->addToggle("Terrain", &terrainDrawEnabled);
    rdrGui->addToggle("Terrain Mask", &terrainMaskDrawEnabled);
    rdrGui->addToggle("Shoe Background", &shoeBackgroundDrawEnabled);
    rdrGui->addToggle("Shoe Details", &shoeDetailsDrawEnabled);
    rdrGui->addToggle("Shoe Foreground", &shoeForegroundDrawEnabled);
    rdrGui->addToggle("Shoe Mask", &shoeMaskDrawEnabled);
}


void RenderEngine::selfUpdate(){
    setupNumViewports(3);
    
    terrainTexture.begin();{
        ofClear(0,255);
        updateTerrainTexture(ofPoint(terrainTexture.getWidth(), terrainTexture.getHeight()));
    } terrainTexture.end();
    
    shoeBackgroundTexture.begin();{
        ofClear(0);
        updateShoeBackgroundTexture(ofPoint(shoeBackgroundTexture.getWidth(), shoeBackgroundTexture.getHeight()));
    } shoeBackgroundTexture.end();
    
    shoeForegroundTexture.begin(); {
        ofClear(0 , 0);
        updateShoeForegroundTexture(ofPoint(shoeForegroundTexture.getWidth(), shoeForegroundTexture.getHeight()));
    } shoeForegroundTexture.end();
    
    if(renderPasses.getWidth() != ofGetWidth() || renderPasses.getHeight() != ofGetHeight()){
        cout<<"Allocate render passes fbo "<<ofGetWidth()<<"x"<<ofGetHeight()<<endl;
        renderPasses.allocate(ofGetWidth(), ofGetHeight());
    }
    
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
/*
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
*/


void RenderEngine::startMatrixTranformation(int viewport, bool terrain){
    if(viewport == 0){
        getCameraRef().begin();
    } else {
        if(terrain){
            calibration->ground[viewport-1].begin();
        } else {
            calibration->shoe[viewport-1].begin();
        }
    }
}

void RenderEngine::endMatrixTranformation(int viewport, bool terrain){
    if(viewport == 0){
        getCameraRef().end();
    } else {
        if(terrain){
            calibration->ground[viewport-1].end();
        } else {
            calibration->shoe[viewport-1].end();
        }
    }
}


//
// Main draw loop, done once for every window
//
void RenderEngine::draw(ofEventArgs & args){
    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    currentViewPort = glfw->getWindowIndex();
    
    ofPushStyle();
    ofDisableLighting();

    
    getRenderTarget(currentViewPort).begin();{
        
        //Blue debug background
        ofClear(0,0,100);
        
        
        renderPasses.clear();
    
        // 1: Terrain draw
        //
        if(terrainDrawEnabled){
            ofPushMatrix();
            renderPasses.dst->begin();
            startMatrixTranformation(currentViewPort, true);
            drawTerrain(currentViewPort);
            endMatrixTranformation(currentViewPort, true);
            renderPasses.dst->end();
            renderPasses.swap();
            ofPopMatrix();
        }
        
        // 2: Terrain Mask draw
        //
        if(terrainMaskDrawEnabled){
            
            //Update the mask
            terrainMaskTexture.begin();{
                ofClear(255, 255);
                updateTerrainMaskTexture(ofPoint(terrainMaskTexture.getWidth(), terrainMaskTexture.getHeight()), currentViewPort);
            } terrainMaskTexture.end();
            

            // Render the mask
            renderPasses.dst->begin(); {
                
                //Draw the source (the thing that should be masked)
                renderPasses.src->draw(0,0);

                
                
                // Render the mask fbo, reusing the src fbo for effeciency
                renderPasses.src->begin();{
                    ofClear(0, 255);
                    startMatrixTranformation(currentViewPort, true);
                    drawTerrainMask(currentViewPort);
                    endMatrixTranformation(currentViewPort, true);
                } renderPasses.src->end();
                
                //Multiply the mask
                ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
                ofDisableDepthTest();
                
                //Draw the mask (can't do it with draw)
                renderPasses.src->getTextureReference().bind();
                int s = renderPasses.src->getWidth();
                glBegin(GL_QUADS);{
                    glTexCoord2d(0, 0); glVertex2d(0, 0);
                    glTexCoord2d(s, 0); glVertex2d(s, 0);
                    glTexCoord2d(s, s); glVertex2d(s, s);
                    glTexCoord2d(0, s); glVertex2d(0, s);
                }glEnd();
                renderPasses.src->getTextureReference().unbind();
                
                ofEnableAlphaBlending();
                
            } renderPasses.dst->end();
            
            renderPasses.swap();
        }
        
        
        
        ofSetColor(255);
        renderPasses.src->draw(0,0);
    
    } getRenderTarget(currentViewPort).end();
    
    
    //  Post-Draw ( shader time )
    //
    ofDisableLighting();
    selfPostDraw();
    
    logGui.drawStatus();
    
    ofPopStyle();
    
    

    /*if(bRenderSystem){
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
    */
    
    
	if(timeline != NULL && currentViewPort == 0){
        
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


void RenderEngine::updateTerrainTexture(ofPoint size){
    assets->terrainUVWireframe.draw(0, 0, size.x, size.y);
}

void RenderEngine::updateTerrainMaskTexture(ofPoint size, int viewport){
    
    //Draw the mask into a fbo that can be applied
    //
    //    if(viewport > 0){
    ofTexture * _tex;
    if(viewport == 1){
        _tex = &assets->terrainMask1;
    } else {
        _tex = &assets->terrainMask2;
    }
    _tex->bind();

    ofSetColor(255);
    glBegin(GL_QUADS);{
        glTexCoord2d(0, 0); glVertex2d(0, 0);
        glTexCoord2d(size.x, 0); glVertex2d(size.x, 0);
        glTexCoord2d(size.x, size.y); glVertex2d(size.x, size.y);
        glTexCoord2d(0, size.y); glVertex2d(0, size.y);
    }glEnd();
    _tex->unbind();
    // }
}


void RenderEngine::updateShoeBackgroundTexture(ofPoint size){
    
}
void RenderEngine::updateShoeForegroundTexture(ofPoint size){
    
}


//Draw a simple UV wireframe map
//
void RenderEngine::drawTerrain(int viewport){
    ofEnableDepthTest();
    ofSetColor(255);
    terrainTexture.getTextureReference().bind();{
        assets->terrainMesh.draw();
    } terrainTexture.getTextureReference().unbind();
}


void RenderEngine::drawTerrainMask(int viewport){
    
    //Apply the fbo to the terrain mesh
    //
    ofEnableDepthTest();
    terrainMaskTexture.getTextureReference().bind();{
        assets->terrainMesh.draw();
    } terrainMaskTexture.getTextureReference().unbind();

}
void RenderEngine::drawShoeBackground(int viewport){
}
void RenderEngine::drawShoeDetails(int viewport){
}
void RenderEngine::drawShoeForeground(int viewport){
}
void RenderEngine::drawShoeMask(int viewport){
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