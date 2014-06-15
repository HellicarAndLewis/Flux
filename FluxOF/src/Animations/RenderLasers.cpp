//
//  RenderLasers.cpp
//
//  Created by Patricio Gonzalez Vivo on 6/15/14.
//
//
#include "RenderLasers.h"

void RenderLasers::selfSetup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    //  AUDIO
    //
    audioIn.setup(44100, 256);
    audioIn.start();
    
    //  SHOES
    //
    shoeTransition.load(getDataPath()+"shaders/shoesTrans");
    
    //  TERRAIN
    //
    noiseShader.loadFrag(getDataPath()+"shaders/noise.frag");
    noiseTexture.allocate(terrainResolution,terrainResolution);
    
    radarShader.loadFrag(getDataPath()+"shaders/radar.frag");
    radarTexture.allocate(terrainResolution,terrainResolution);
    
    terrainTransition.loadFrag(getDataPath()+"shaders/terrainTrans.frag");
    terrainTransitionTex.allocate(terrainResolution,terrainResolution);
    terrainTransitionTex.clear();
    
    ofDisableArbTex();
    terrainTex.allocate(terrainResolution,terrainResolution);
    terrainTex.begin();
    ofClear(0, 0);
    terrainTex.end();
    ofEnableArbTex();
}

void RenderLasers::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audioIn);
    
    guiAdd(noiseShader);
    guiAdd(radarShader);
    guiAdd(terrainTransition);
    
    guiAdd(shoeTransition);
}

void RenderLasers::selfGuiEvent(ofxUIEventArgs &e){
    
}

void RenderLasers::selfSetupSystemGui(){

}

void RenderLasers::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

void RenderLasers::selfSetupRenderGui(){
    rdrGui->addLabel("Render mode");
    rdrGui->addToggle("Simulator", &simulatorMode);
}

void RenderLasers::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

//---------------------------------------------------

void RenderLasers::selfUpdate(){
    if(simulatorMode){
        setupNumViewports(1);
        cameraEnable(true);
        setupRenderIsFlipped(false);
    } else {
        setupNumViewports(2);
        cameraEnable(false);
        setupRenderIsFlipped(true);
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
    
    noiseTexture.begin();
    noiseShader.begin();
    noiseShader.getShader().setUniformTexture("depthMap", terrainDepthMap, 0);
    noiseShader.getShader().setUniformTexture("normalMap", terrainNormalMap, 1);
    noiseShader.getShader().setUniformTexture("maskTex", radarTexture, 2);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(terrainResolution, 0); glVertex3f(terrainResolution, 0, 0);
    glTexCoord2f(terrainResolution, terrainResolution); glVertex3f(terrainResolution, terrainResolution, 0);
    glTexCoord2f(0,terrainResolution);  glVertex3f(0,terrainResolution, 0);
    glEnd();
    noiseShader.end();
    noiseTexture.end();
    
    terrainTransitionTex.swap();
    terrainTransitionTex.dst->begin();
    terrainTransition.begin();
    terrainTransition.getShader().setUniformTexture("backbuffer", *terrainTransitionTex.src, 0);
    terrainTransition.getShader().setUniformTexture("depthMap", terrainDepthMap, 1);
    terrainTransition.getShader().setUniformTexture("normalMap", noiseTexture, 2);
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
    
    terrainTransitionTex.dst->end();
    
    terrainTex.begin();
    terrainTransitionTex.dst->draw(0, 0);
    terrainTex.end();
}

void RenderLasers::selfDraw(){
    materials["MATERIAL 1"]->begin();

    ofPushMatrix();

    ofSetColor(255);
    
    //  TERRAIN
    //

    ofPushMatrix();
    ofSetSmoothLighting(false);
    
    ofDisableArbTex();
    terrainTex.getTextureReference().bind();
    terrainMesh.draw();
    terrainTex.getTextureReference().unbind();
    ofEnableArbTex();
    
    ofPopMatrix();

    //  SHOE
    //
    ofPushMatrix();
    ofSetSmoothLighting(true);
    shoeTransition.begin();
    shoeMesh.draw();
    shoeTransition.end();
    ofPopMatrix();
    
    ofPopMatrix();
  
    if(!simulatorMode){
        calibration->shoe[currentViewPort].end();
    }
    
    materials["MATERIAL 1"]->end();
}

void RenderLasers::selfDrawOverlay(){
    if(bDebug){
        
        ofPushMatrix();
        ofScale(0.5, 0.5);
        terrainTransitionTex.dst->draw(terrainResolution*0.25,0);
        
        ofPushMatrix();
        ofTranslate(terrainResolution*1.25,0);
        ofScale(0.5, 0.5);
        shoeTexB.draw(0, 0);
        ofPopMatrix();
        
        ofPushMatrix();
        ofScale(0.25, 0.25);
        terrainDepthMap.draw(0,0);
        terrainNormalMap.draw(0,terrainResolution);
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