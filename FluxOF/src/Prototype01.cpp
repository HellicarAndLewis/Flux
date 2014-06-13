//
//  Prototype01.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//
#include "Prototype01.h"

void Prototype01::selfSetup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    //  AUDIO
    //
    audio.setup(44100, 256);
    audio.start();
    
    //  FONTS
    //
    font.loadFont(getDataPath()+"Exo2-Light.ttf", 44);
    font.setSpaceSize(0.65);
    
    //  SHOES
    //
    ofxAssimpModelLoader loader;
    loader.loadModel(getDataPath()+"terrain_and_shoe.dae");
    shoeMesh = loader.getMesh(1);
    shoeTransition.load(getDataPath()+"shaders/shoesTrans");
    
    //  TERRAIN
    //
    ofLoadImage(terrainDepthMap, getDataPath()+"terrainDepthMap.png");
    terrainResolution = terrainDepthMap.getWidth();
    terrainTex.allocate(terrainResolution,terrainResolution);
    terrainTex.clear();
    terrainTransition.loadFrag(getDataPath()+"shaders/terrainTrans.frag");
    terrainMesh = loader.getMesh(0);
    
    blur.allocate(terrainResolution,terrainResolution);
    blur.fade = 1.0;
    normalMap.allocate(terrainResolution,terrainResolution);
    
    noiseShader.loadFrag(getDataPath()+"shaders/noise.frag");
    noiseTexture.allocate(terrainResolution,terrainResolution);
    
    radarShader.loadFrag(getDataPath()+"shaders/radar.frag");
    radarTexture.allocate(terrainResolution,terrainResolution);
    
    displace.allocate(terrainResolution,terrainResolution);
    
    //  CALIBRATION
    //
    calibration.setup();
}

void Prototype01::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audio);
    
    guiAdd(radarShader);
    guiAdd(noiseShader);
    guiAdd(terrainTransition);
    
    guiAdd(shoeTransition);
    
}

void Prototype01::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Prototype01::selfSetupSystemGui(){

}

void Prototype01::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

void Prototype01::selfSetupRenderGui(){
    rdrGui->addLabel("Render mode");
    rdrGui->addToggle("Simulator", &simulatorMode);
}

void Prototype01::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

//---------------------------------------------------

void Prototype01::startTransitionTo(string _twitterUser, string _twitterImgPath){

    text = _twitterUser;
    
    //  Load Image
    //
    ofImage img;
    img.loadImage(_twitterImgPath);
    colorPalette = ColorTheory::getColorPalette(img, 5);
    
    //  Keep the image as a destinationTexture
    //
    shoeTexB = img.getTextureReference();
    
    //  Start Animation
    //
    timeline->setPercentComplete(0.0);
    timeline->play();
}

void Prototype01::selfUpdate(){
    if(simulatorMode){
        setupNumViewports(1);
        cameraEnable(true);
    } else {
        setupNumViewports(2);
        cameraEnable(false);
    }

    //  TERRAIN TEXTURE
    //
    
    radarTexture.begin();
    ofClear(0,0);
    radarShader.begin();
    radarShader.getShader().setUniform1f("resolution", terrainResolution);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(terrainResolution, 0); glVertex3f(terrainResolution, 0, 0);
    glTexCoord2f(terrainResolution, terrainResolution); glVertex3f(terrainResolution, terrainResolution, 0);
    glTexCoord2f(0,terrainResolution);  glVertex3f(0,terrainResolution, 0);
    glEnd();
    radarShader.end();
    radarTexture.end();
    
    blur << terrainDepthMap;
    normalMap << blur;
    
    noiseTexture.begin();
    noiseShader.begin();
    noiseShader.getShader().setUniformTexture("terrainNormalMap", normalMap, 2);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(terrainResolution, 0); glVertex3f(terrainResolution, 0, 0);
    glTexCoord2f(terrainResolution, terrainResolution); glVertex3f(terrainResolution, terrainResolution, 0);
    glTexCoord2f(0,terrainResolution);  glVertex3f(0,terrainResolution, 0);
    glEnd();
    noiseShader.end();
    noiseTexture.end();
    
    terrainTex.swap();
    terrainTex.dst->begin();
    terrainTransition.begin();
    terrainTransition.getShader().setUniformTexture("backbuffer", *terrainTex.src, 0);
    terrainTransition.getShader().setUniformTexture("depthMap", terrainDepthMap, 1);
    terrainTransition.getShader().setUniformTexture("normalMap", normalMap, 2);
    terrainTransition.getShader().setUniformTexture("radarTex", radarTexture, 3);
    
    terrainTransition.getShader().setUniform3f("dstColor1",
                                               ((float)colorPalette[0].r)/255.0,
                                               ((float)colorPalette[0].g)/255.0,
                                               ((float)colorPalette[0].b)/255.0);
    terrainTransition.getShader().setUniform3f("dstColor2",
                                               ((float)colorPalette[1].r)/255.0,
                                               ((float)colorPalette[1].g)/255.0,
                                               ((float)colorPalette[1].b)/255.0);
    terrainTransition.getShader().setUniform3f("dstColor3",
                                               ((float)colorPalette[2].r)/255.0,
                                               ((float)colorPalette[2].g)/255.0,
                                               ((float)colorPalette[2].b)/255.0);
    terrainTransition.getShader().setUniform3f("dstColor4",
                                               ((float)colorPalette[3].r)/255.0,
                                               ((float)colorPalette[3].g)/255.0,
                                               ((float)colorPalette[3].b)/255.0);
    terrainTransition.getShader().setUniform3f("dstColor5",
                                               ((float)colorPalette[4].r)/255.0,
                                               ((float)colorPalette[4].g)/255.0,
                                               ((float)colorPalette[4].b)/255.0);
    
    terrainTransition.getShader().setUniform1f("resolution", terrainResolution);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(terrainResolution, 0); glVertex3f(terrainResolution, 0, 0);
    glTexCoord2f(terrainResolution, terrainResolution); glVertex3f(terrainResolution, terrainResolution, 0);
    glTexCoord2f(0,terrainResolution);  glVertex3f(0,terrainResolution, 0);
    glEnd();
    terrainTransition.end();
    
    // - TWIITER TEXT
    
    terrainTex.dst->end();
}

void Prototype01::selfDraw(){
    materials["MATERIAL 1"]->begin();

    ofPushMatrix();
    
    if(!simulatorMode){
        calibration.shoe[currentViewPort].begin();
    }
//    if(currentViewPort == 1){
//        ofTranslate(-100, 0,0);
//    }

    ofSetColor(255);
    
    //  TERRAIN
    //

    //  Load Terrain Calib
    ofPushMatrix();
    ofSetSmoothLighting(false);
    
//    terrainTex.dst->bind();
    terrainMesh.draw();
//    terrainTex.dst->unbind();
    
    ofPopMatrix();

    //  SHOE
    //

    ofPushMatrix();
    ofSetSmoothLighting(true);
    //  Load Shoe Calib
    
    shoeTransition.begin();
    shoeMesh.draw();
    shoeTransition.end();
    ofPopMatrix();
    
    ofPopMatrix();
  
    if(!simulatorMode){
        calibration.shoe[currentViewPort].end();
    }
    
    materials["MATERIAL 1"]->end();
}

void Prototype01::selfDrawOverlay(){
    if(bDebug){
        
        ofPushMatrix();
        terrainTex.dst->draw(terrainResolution*0.25,0);
        
        ofPushMatrix();
        ofTranslate(terrainResolution*1.25,0);
        ofScale(0.5, 0.5);
        shoeTexB.draw(0, 0);
        
        ofPopMatrix();
        
        ofPushMatrix();
        ofScale(0.25, 0.25);
        terrainDepthMap.draw(0,0);
        normalMap.draw(0,terrainResolution);
        noiseTexture.draw(0,terrainResolution*2.0);
        radarTexture.draw(0,terrainResolution*3.0);
        ofPopMatrix();
        
        ofPopMatrix();
        
        float paletteSize = 10;
        float margin = 15;
        
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(ofGetWidth()-margin*1.5-paletteSize,0);
        for(int i = 0; i < colorPalette.size(); i++){
            ofSetColor(colorPalette[i]);
            ofCircle(margin, i*(paletteSize*2.0+5)+paletteSize*0.5+5, paletteSize);
        }
        ofPopStyle();
        ofPopMatrix();
    }
}

void Prototype01::selfKeyPressed(ofKeyEventArgs & args){

}

void Prototype01::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Prototype01::selfMouseMoved(ofMouseEventArgs& data){
	
}

void Prototype01::selfMousePressed(ofMouseEventArgs& data){

}

void Prototype01::selfMouseDragged(ofMouseEventArgs& data){

}

void Prototype01::selfMouseReleased(ofMouseEventArgs& data){

}