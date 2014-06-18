//
//  RenderRadar.cpp
//
//  Created by Patricio Gonzalez Vivo on 6/11/14.
//
//
#include "RenderRadar.h"

void RenderRadar::selfSetup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    //  AUDIO
    //
    audioBufferSize = 256;
    audioIn.setup(44100, audioBufferSize);
    audioIn.start();
    audioPixels.allocate(audioBufferSize,1,3);
    audioTex.allocate(audioBufferSize,1, GL_RGB16F);
    
    //  USERNAME TEXT
    //
    textTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    //  SHOES
    //
    shoeTransition.load(getDataPath()+"shaders/shoesTrans");
    
    //  TERRAIN
    //
    noiseShader.loadFrag(getDataPath()+"shaders/noise.frag");
    noiseTexture.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    radarShader.loadFrag(getDataPath()+"shaders/radar.frag");
    radarTexture.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    terrainMeshTransition.loadFrag(getDataPath()+"shaders/terrainMeshTrans.frag");
    terrainMeshTransitionTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    terrainTransition.loadFrag(getDataPath()+"shaders/terrainTrans.frag");
    terrainTransitionTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    terrainTransitionTex.clear();
    
    ofDisableArbTex();
    
    terrainMeshTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    terrainTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    terrainTex.begin();
    ofClear(0, 0);
    terrainTex.end();
    
    shoeTex.allocate(100, 100);
    ofEnableArbTex();
}

void RenderRadar::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audioIn);
    
    guiAdd(noiseShader);
    guiAdd(radarShader);
    
    guiAdd(terrainTransition);
    guiAdd(terrainMeshTransition);
    
    guiAdd(shoeTransition);
}

void RenderRadar::selfGuiEvent(ofxUIEventArgs &e){
    
}

void RenderRadar::selfSetupSystemGui(){
    sysGui->addLabel("Radar");
    sysGui->add2DPad("Radar_Center",ofVec2f(0,assets->terrainResolution()),ofVec2f(0,assets->terrainResolution()),&radarCenter);
    sysGui->addSlider("Radar_Pct", 0.0, 1.0, &radarPct);
    sysGui->addSlider("Radar_Color_R", 0 , 1.0, &radarColor.r);
    sysGui->addSlider("Radar_Color_G", 0 , 1.0, &radarColor.g);
    sysGui->addSlider("Radar_Color_B", 0 , 1.0, &radarColor.b);
    sysGui->addSlider("Radar_Height",assets->sceneMin.y,assets->sceneMax.y, &radarHeight);
    
    sysGui->addLabel("Text");
    sysGui->add2DPad("Text_offset", ofVec2f(0,assets->terrainResolution()),ofVec2f(0,assets->terrainResolution()), &textOffset);
    sysGui->addSlider("Text_Alpha", 0.0, 1.0, &textAlpha);
    

}

void RenderRadar::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

void RenderRadar::selfSetupRenderGui(){
    rdrGui->addLabel("Render mode");
    rdrGui->addToggle("Simulator", &simulatorMode);
}

void RenderRadar::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

//---------------------------------------------------

void RenderRadar::selfUpdate(){
    if(simulatorMode){
        setupNumViewports(1);
        cameraEnable(true);
        setupRenderIsFlipped(false);
    } else {
        setupNumViewports(2);
        cameraEnable(false);
        setupRenderIsFlipped(true);
    }
    
    //  Audio
    //
//    for (int x = 0; x < audioBufferSize; x++){
//        audioPixels.setColor(x, 0, ofFloatColor(abs(sin(audioIn.left[x]*100.0)),0.0,0.0,1.0));
//    }
//    audioTex.loadData(audioPixels);
    
    //  USERNAME TEXTURE
    //
    {
        textTex.begin();
        ofPushStyle();
        ofClear(0,0);
        ofSetColor(255, textAlpha*255.0);
        ofPushMatrix();
        ofTranslate(textOffset);
        ofPoint textCenter = assets->font.getStringBoundingBox(text,0,0).getCenter();
        assets->font.drawString(text, -textCenter.x, -textCenter.y );
        ofPopMatrix();
        ofPopStyle();
        textTex.end();
    }
    
    //  TERRAIN ANIMATION
    //  -----------------------------------------
    
    //  RadarMask
    {
        radarTexture.begin();
        ofClear(0,0);
        radarShader.begin();
        radarShader.getShader().setUniform1f("pct", radarPct);
        radarShader.getShader().setUniform2f("center", radarCenter.x, radarCenter.y);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
        glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
        glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
        glEnd();
        radarShader.end();
        radarTexture.end();
    }
    
    //  Noise Forces (for water-color effect)
    {
        noiseTexture.begin();
        noiseShader.begin();
        noiseShader.getShader().setUniformTexture("depthMap", assets->terrainDepthMap, 0);
        noiseShader.getShader().setUniformTexture("normalMap", assets->terrainNormalMap, 1);
        noiseShader.getShader().setUniformTexture("maskTex", assets->terrainMask, 2);
        
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
        glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
        glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
        glEnd();
        noiseShader.end();
        noiseTexture.end();
    }
    
    //  Pixel displacement Transition Animation
    {
        terrainTransitionTex.swap();
        terrainTransitionTex.dst->begin();
        terrainTransition.begin();
        terrainTransition.getShader().setUniformTexture("backbuffer", *terrainTransitionTex.src, 0);
        terrainTransition.getShader().setUniformTexture("depthMap", assets->terrainDepthMap, 1);
        terrainTransition.getShader().setUniformTexture("maskTex", assets->terrainMask, 2);
        terrainTransition.getShader().setUniformTexture("normalMap", noiseTexture, 3);
        terrainTransition.getShader().setUniformTexture("audioIn", audioTex, 4);
        
        terrainTransition.getShader().setUniformTexture("radarTex", radarTexture, 5);
        
        for(int i = 0; i < srcPalette.size(); i++){
            terrainTransition.getShader().setUniform3f("srcColor"+ofToString(i+1),
                                                       ((float)srcPalette[i].r)/255.0,
                                                       ((float)srcPalette[i].g)/255.0,
                                                       ((float)srcPalette[i].b)/255.0);
        }
        
        for(int i = 0; i < dstPalette.size(); i++){
            terrainTransition.getShader().setUniform3f("dstColor"+ofToString(i+1),
                                                       ((float)dstPalette[i].r)/255.0,
                                                       ((float)dstPalette[i].g)/255.0,
                                                       ((float)dstPalette[i].b)/255.0);
        }
        
        terrainTransition.getShader().setUniform1f("resolution", assets->terrainResolution());
        
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
        glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
        glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
        glEnd();
        terrainTransition.end();
        terrainTransitionTex.dst->end();
    }
    
    //  Terrain texture
    {
        terrainTex.begin();
        
        // colored transition
        terrainTransitionTex.dst->draw(0, 0);
        
        // radar line
        ofPushStyle();
        ofPushMatrix();
        ofSetColor(radarColor);
        ofTranslate(radarCenter);
        float radius = assets->terrainResolution();
        float angle = TWO_PI*radarPct;
        ofLine(0,0,radius*cos(angle),radius*sin(angle));
        ofPopMatrix();
        ofPopStyle();
        
        // username text
        textTex.draw(0,0);
        
        terrainTex.end();
    }
    
    {
        terrainMeshTransitionTex.begin();
        terrainMeshTransition.begin();
        terrainMeshTransition.getShader().setUniformTexture("radarMask", radarTexture, 0);
        terrainMeshTransition.getShader().setUniformTexture("terrainMask", assets->terrainMask, 1);
        terrainMeshTransition.getShader().setUniformTexture("background", terrainTransitionTex.dst->getTextureReference(), 2);
        terrainMeshTransition.getShader().setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
        glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
        glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
        glEnd();
        terrainMeshTransition.end();
        terrainMeshTransitionTex.end();
        
        terrainMeshTex.begin();
        terrainMeshTransitionTex.draw(0,0);
        terrainMeshTex.end();
    }
}

void RenderRadar::selfDraw(){
    materials["MATERIAL 1"]->begin();

    ofSetColor(255);
    
    //  TERRAIN
    //

    ofPushMatrix();
    ofSetSmoothLighting(false);
    
    ofDisableArbTex();
    terrainTex.getTextureReference().bind();
    assets->terrainMesh.draw();
    terrainTex.getTextureReference().unbind();
    
    terrainMeshTex.getTextureReference().bind();
    assets->terrainMesh.drawWireframe();
    terrainMeshTex.getTextureReference().unbind();
    
    ofEnableArbTex();
    
    ofPopMatrix();

    //  SHOE
    //
    ofPushMatrix();
    ofSetSmoothLighting(true);
    
    ofDisableArbTex();
    shoeTransition.begin();
    shoeTransition.getShader().setUniform1f("radarHeight", radarHeight);
    shoeTransition.getShader().setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
    shoeTransition.getShader().setUniformTexture("srcTexture",shoeTex.src->getTextureReference(), 0);
    shoeTransition.getShader().setUniformTexture("dstTexture",shoeTex.dst->getTextureReference(), 1);
    shoeTransition.getShader().setUniformTexture("colorMaskTexture", assets->shoeColorMask, 2);
    
    for(int i = 0; i < srcPalette.size(); i++){
        shoeTransition.getShader().setUniform3f("dstColor"+ofToString(i+1),
                                                ((float)srcPalette[i].r)/255.0,
                                                ((float)srcPalette[i].g)/255.0,
                                                ((float)srcPalette[i].b)/255.0);
    }
    
    for(int i = 0; i < dstPalette.size(); i++){
        shoeTransition.getShader().setUniform3f("dstColor"+ofToString(i+1),
                                                ((float)dstPalette[i].r)/255.0,
                                                ((float)dstPalette[i].g)/255.0,
                                                ((float)dstPalette[i].b)/255.0);
    }
    
    assets->shoeMesh.draw();
    shoeTransition.end();
    ofEnableArbTex();
    
    ofPopMatrix();
    
    if(!simulatorMode){
        calibration->shoe[currentViewPort].end();
    }
    
    materials["MATERIAL 1"]->end();
}

void RenderRadar::selfDrawOverlay(){
    if(bDebug){
        
        ofPushMatrix();
        
        ofScale(0.5, 0.5);
        terrainTex.draw(assets->terrainResolution()*0.25,0);
        
        ofPushMatrix();
        ofTranslate(assets->terrainResolution()*1.25,0);
        shoeTex.dst->draw(0, 0);
        assets->shoeUVWireframe.draw(0, 0,shoeTex.dst->getWidth(), shoeTex.dst->getHeight());
        ofPopMatrix();
        
        ofPushMatrix();
        ofScale(0.25, 0.25);
        assets->terrainDepthMap.draw(0,0);
        assets->terrainNormalMap.draw(0,assets->terrainResolution());
        noiseTexture.draw(0,assets->terrainResolution()*2.0);
        radarTexture.draw(0,assets->terrainResolution()*3.0);
        terrainMeshTex.draw(0,assets->terrainResolution()*4.0);
        ofPopMatrix();
        
        ofPopMatrix();
        
//        audioTex.draw(0,assets->terrainResolution()*0.5,assets->terrainResolution()*0.5,10);
        
        float paletteSize = 10;
        float margin = 15;
        
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(ofGetWidth()-margin*1.5-paletteSize,0);
        for(int i = 0; i < dstPalette.size(); i++){
            ofSetColor(dstPalette[i]);
            ofCircle(margin, i*(paletteSize*2.0+5)+paletteSize*0.5+5, paletteSize);
        }
        ofPopStyle();
        ofPopMatrix();
    }
}