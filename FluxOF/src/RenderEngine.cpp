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
}

void RenderEngine::selfUpdate(){
    setupNumViewports(3);

}

void RenderEngine::setCalibration(CalibrationLoader *_calibration){
    calibration = _calibration;
    
}

void RenderEngine::addUiClass(UIClass *_ui){
    guiAdd( *_ui );
}

void RenderEngine::selfSceneTransformation(){

    if(currentViewPort > 0){
        int view = currentViewPort - 1;
        calibration->shoe[view].begin();
    }
}

//------------------------------------ COLOR TOOLS

float luminance(ofColor col) {
    return col.r * 0.299f *255+ col.g * 0.587f *255+ col.b * 0.114f*255;
}

ofColor getDarkest(vector<ofColor> &colors) {
    ofColor darkest;
    float minBrightness = FLT_MAX;
    for (int i=0;i<colors.size();i++) {
        ofColor c = colors[i];
        float luma = luminance(c);
        if (luma < minBrightness) {
            darkest = c;
            minBrightness = luma;
        }
    }
    return darkest;
}

float distanceBetween(ofColor a,ofColor b) {
    float hue = a.getHue()/255.0f *TWO_PI;
    float hue2 = b.getHue()/255.0f * TWO_PI;
    ofVec3f v1((cos(hue) * a.getSaturation()/255.0f),
               (sin(hue) * a.getSaturation()/255.0f), a.getBrightness()/255.0f);
    ofVec3f v2((cos(hue2) * b.getSaturation()/255.0f),
               (sin(hue2) * b.getSaturation()/255.0f), b.getBrightness()/255.0f);
    return v1.distance(v2);
}

void sortByDistance(vector<ofColor> &colors) {
    if (colors.size() == 0) {
        return;
    }
    int totalCount = colors.size();
    ofColor root = getDarkest(colors);
    // put it in the sorted list as starting element.
    vector<ofColor> sorted;
    sorted.push_back(root);
    
    // Remove the darkest color from the stack,
    
    vector<ofColor> stack(colors);
    vector<ofColor>::iterator it =  std::find(stack.begin(), stack.end(), root);
    // ofLog()<<"erase darkest "<<(*it)<<endl;
    //if(it != stack.end()){
    stack.erase(it);
    // }
    
    
    // Now find the color in the stack closest to that color.
    // Take this color from the stack and add it to the sorted list.
    // Now find the color closest to that color, etc.
    int sortedCount = 0;
    
    while (stack.size() > 1) {
        ofColor closest = stack[0];
        ofColor lastSorted = sorted[sortedCount];
        float distance = distanceBetween(closest, lastSorted);
        // ofLog()<<"searching: "<<endl;
        int foundId=0;
        for (int i = stack.size() - 1; i >= 0; i--) {
            ofColor c = stack[i];
            float d = distanceBetween(c, lastSorted);
            //  ofLog()<<"dist: "<<d<< " stack.size()  "<<stack.size()<< " sortedCount "<<sortedCount<<" totalCount "<<totalCount <<endl;
            if (d < distance) {
                closest = c;
                distance = d;
                foundId=i;
            }
        }
        sorted.push_back(closest);
        stack.erase(stack.begin()+foundId);
        
        sortedCount++;
    }
    sorted.push_back(stack[0]);
    
    colors = sorted;
}

vector<ofVec2f> getRYB_WHEEL(){
    vector<ofVec2f> RYB_WHEEL;
    RYB_WHEEL.push_back(ofVec2f(0, 0));
    RYB_WHEEL.push_back(ofVec2f(15, 8));
    RYB_WHEEL.push_back(ofVec2f(30, 17));
    RYB_WHEEL.push_back(ofVec2f(45, 26));
    RYB_WHEEL.push_back(ofVec2f(60, 34));
    RYB_WHEEL.push_back(ofVec2f(75, 41));
    RYB_WHEEL.push_back(ofVec2f(90, 48));
    RYB_WHEEL.push_back(ofVec2f(105, 54));
    RYB_WHEEL.push_back(ofVec2f(120, 60));
    RYB_WHEEL.push_back(ofVec2f(135, 81));
    RYB_WHEEL.push_back(ofVec2f(150, 103));
    RYB_WHEEL.push_back(ofVec2f(165, 123));
    RYB_WHEEL.push_back(ofVec2f(180, 138));
    RYB_WHEEL.push_back(ofVec2f(195, 155));
    RYB_WHEEL.push_back(ofVec2f(210, 171));
    RYB_WHEEL.push_back(ofVec2f(225, 187));
    RYB_WHEEL.push_back(ofVec2f(240, 204));
    RYB_WHEEL.push_back(ofVec2f(255, 219));
    RYB_WHEEL.push_back(ofVec2f(270, 234));
    RYB_WHEEL.push_back(ofVec2f(285, 251));
    RYB_WHEEL.push_back(ofVec2f(300, 267));
    RYB_WHEEL.push_back(ofVec2f(315, 282));
    RYB_WHEEL.push_back(ofVec2f(330, 298));
    RYB_WHEEL.push_back(ofVec2f(345, 329));
    RYB_WHEEL.push_back(ofVec2f(360, 0));
    
    return RYB_WHEEL;
}

ofColor rotateRYB(ofColor col,int theta) {
    
    float h = (float) col.getHue()/255.0 * 360;
    vector<float> hsb;
    hsb.push_back((float) col.getHue()/255.0) ;
    hsb.push_back((float) col.getSaturation()/255.0) ;
    hsb.push_back((float) col.getBrightness()/255.0) ;
    theta %= 360;
    
    float resultHue = 0;
    
    vector<ofVec2f> RYB_WHEEL = getRYB_WHEEL();
    
    for (int i = 0; i < RYB_WHEEL.size() - 1; i++) {
        ofVec2f p = RYB_WHEEL[i];
        ofVec2f q = RYB_WHEEL[i + 1];
        if (q.y < p.y) {
            q.y += 360;
        }
        if (p.y <= h && h <= q.y) {
            resultHue = p.x + (q.x - p.x) * (h - p.y) / (q.y - p.y);
            break;
        }
    }
    
    //fmod = %, ie remainder
    
    // And the user-given angle (e.g. complement).
    resultHue = fmod((resultHue + theta),360);
    
    // For the given angle, find out what hue is
    // located there on the artistic color wheel.
    for (int i = 0; i < RYB_WHEEL.size() - 1; i++) {
        ofVec2f p = RYB_WHEEL[i];
        ofVec2f q = RYB_WHEEL[i + 1];
        if (q.y < p.y) {
            q.y += 360;
        }
        if (p.x <= resultHue && resultHue <= q.x) {
            h = p.y + (q.y - p.y) * (resultHue - p.x) / (q.x - p.x);
            break;
        }
    }
    
    hsb[0] = fmod(h, 360) / 360.0f;
    
    ofColor newCol;
    newCol.setHsb(hsb[0]*255, hsb[1]*255, hsb[2]*255);
    return newCol;
}

ofColor getComplement(ofColor col){
    return rotateRYB(col,180);
}

vector<ofColor> getColorPalette(ofImage &_img, int _nColors){
    
    vector<ofColor> palette;
    
    //  Extract Color Palette
    //
    ofImage smallImg;
    smallImg = _img;
    smallImg.resize(_img.getWidth()*0.25, _img.getHeight()*0.25);
    const int sampleCount = smallImg.getHeight() * smallImg.getWidth();
    cv::Mat colorSamples( sampleCount, 1, CV_32FC3 );
    
    // get our pixels
    unsigned char * pixels = smallImg.getPixels();
    
    // build our matrix of samples
    cv::MatIterator_<cv::Vec3f> sampleIt = colorSamples.begin<cv::Vec3f>();
    for(int i=0; i<sampleCount; i++){
        int pos = i * 3;
        *sampleIt = cv::Vec3f( pixels[pos], pixels[pos+1], pixels[pos+2] );
        sampleIt++;
    }
    
    // call kmeans
    cv::Mat labels, clusters;
    cv::kmeans( colorSamples, _nColors, labels, cv::TermCriteria(), 2, cv::KMEANS_RANDOM_CENTERS, clusters ); //cv::TermCriteria::COUNT, 8, 0
    
    for( int i = 0; i < _nColors; ++i ){
        ofColor clusterColor = ofColor( clusters.at<cv::Vec3f>(i,0)[0], clusters.at<cv::Vec3f>(i,0)[1], clusters.at<cv::Vec3f>(i,0)[2] );
        palette.push_back(clusterColor);
    }
    
    //  Add the complementary of the first one
    //
    palette.push_back(getComplement(palette[0]));
    palette.push_back(ofColor(luminance(palette[0])));
    
    return palette;
}

//---------------------------------------------------------------

void RenderEngine::startTransitionTo(QueueItem queueItem){
    text = "@adidas";//queueItem.username;
    
    //  Extract Colors
    //
    srcPalette = dstPalette;
    dstPalette = getColorPalette(queueItem.image, 3);
    
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
void RenderEngine::drawMask(int viewPort){
    if(!simulatorMode){
        ofSetColor(255);
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        
        ofTexture tex;
        
        if(viewPort == 0){
            tex = assets->terrainMask1;
        } else {
            tex = assets->terrainMask2;
        }
        
        int s = tex.getWidth();
        tex.bind();
        glBegin(GL_QUADS);{
            glTexCoord2d(0, 0); glVertex2d(0, 0);
            glTexCoord2d(s, 0); glVertex2d(s, 0);
            glTexCoord2d(s, s); glVertex2d(s, s);
            glTexCoord2d(0, s); glVertex2d(0, s);
        }glEnd();
        tex.unbind();
        ofEnableAlphaBlending();
    }
}

void RenderEngine::draw(ofEventArgs & args){
    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    int viewNum = glfw->getWindowIndex();

    if(bRenderSystem){
        //for(int viewNum=0;viewNum<numViewports;viewNum++){
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
                
                //  Scene Setup
                //
                selfSceneTransformation();
                
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
                    lightsBegin();
                    ofPushStyle();
                    ofPushMatrix();
                    
                    selfDraw();
                    
                    ofPopMatrix();
                    ofPopStyle();
                    lightsEnd();
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