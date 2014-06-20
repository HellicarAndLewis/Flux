//
//  RenderRadar.cpp
//
//  Created by Patricio Gonzalez Vivo on 6/11/14.
//
//
#include "RenderRadar.h"

void RenderRadar::selfSetup(){
    RenderEngine::selfSetup();
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    //  AUDIO
    //
    audioIn.start();
    
    audioTerrain.loadFrag(getDataPath()+"shaders/audioTerrain.frag");
    
    ripples.allocate(assets->terrainResolution(), assets->terrainResolution(),GL_RGBA);
    ripplesNormals.allocate(assets->terrainResolution(), assets->terrainResolution());
    
    //  SHOES
    //
    shoeTransition.load(getDataPath()+"shaders/shoesTrans");
    
    //  TERRAIN
    //
    terrainShader.load(getDataPath()+"shaders/terrain");
    
    radarShader.loadFrag(getDataPath()+"shaders/radar.frag");
    radarTexture.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    terrainNoise.loadFrag(getDataPath()+"shaders/noise.frag");
    terrainNoiseTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    terrainMeshShader.load(getDataPath()+"shaders/terrainMesh");
    
    terrainTransition.loadFrag(getDataPath()+"shaders/terrainTrans.frag");
    terrainTransitionTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    terrainTransitionTex.clear();
    
    terrainTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    terrainTex.begin();
    ofClear(0, 0);
    terrainTex.end();
    

    for(int i=0;i<3;i++){
        terrainMask[i].allocate(assets->terrainResolution(),assets->terrainResolution());
    }

    //  Non-Arb
    ofDisableArbTex();
    shoeTex.allocate(100, 100);
    ofEnableArbTex();
}

void RenderRadar::selfSetupGuis(){
    
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audioIn);
    
    // 2D (just frag shaders)
    guiAdd(audioTerrain);
    
    guiAdd(radarShader);
    guiAdd(terrainNoise);
    guiAdd(terrainTransition);
    
    //  3D
    guiAdd(terrainShader);
    guiAdd(terrainMeshShader);
    
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
    sysGui->addSlider("Text_Scale", 0.0, 1.0, &textScale);
    
    sysGui->addLabel("Ripples");
    sysGui->addSlider("Ripples_R", 0., 1., &ripplesColor.r);
    sysGui->addSlider("Ripples_G", 0., 1., &ripplesColor.g);
    sysGui->addSlider("Ripples_B", 0., 1., &ripplesColor.b);
    
//    sysGui->addSlider("Ripples_dump", 0.9, 0.999999, &ripples.damping);
}

void RenderRadar::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}

void RenderRadar::selfSetupRenderGui(){

    rdrGui->addLabel("Render mode");
    rdrGui->addToggle("Simulator", &simulatorMode);

    RenderEngine::selfSetupRenderGui();

}

void RenderRadar::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
}
//---------------------------------------------------

void RenderRadar::selfBegin(){
    ripples.begin();
    ofClear(0,255);
    ripples.end();
    ripples.update();
    ripples.begin();
    ofClear(0,255);
    ripples.end();
}

//---------------------------------------------------

void RenderRadar::selfUpdate(){
    RenderEngine::selfUpdate();

    //  AUDIO REACTION
    //  -----------------------------------------
    {

        
        ripples.begin();
        audioTerrain.begin();
        audioTerrain.getShader().setUniformTexture("heightMap", assets->terrainDepthMap, 0);
        audioTerrain.getShader().setUniformTexture("audioFft", audioIn.texture, 1);
        audioTerrain.getShader().setUniform1f("audioFftSize", audioIn.texture.getWidth());
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
        glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
        glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
        glEnd();
        audioTerrain.end();
        
        ripples.end();
        
        ripples.update();
//        ripplesNormals << ripples;
    }
    
    
    //  Color the radar
    //
    if(dstPalette.size()>0){
        radarColor.set(dstPalette[3]);
        radarColor.setBrightness(1.0);
        radarColor.setSaturation(1.0);
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
    
    //  Pixel displacement Transition Animation
    {
        
        if(terrainNoise.bEnable){
            terrainNoiseTex.begin();
            terrainNoise.begin();
            ofClear(0);
            assets->terrainNormalMap.draw(0,0);
            terrainNoise.end();
            terrainNoiseTex.end();
        }
        
        
        terrainTransitionTex.swap();
        terrainTransitionTex.dst->begin();
        terrainTransition.begin();
        terrainTransition.getShader().setUniformTexture("backbuffer", *terrainTransitionTex.src, 0);
        terrainTransition.getShader().setUniformTexture("depthMap", assets->terrainDepthMap, 1);
        terrainTransition.getShader().setUniformTexture("terrainAreas", assets->terrainMask, 2);
        if(terrainNoise.bEnable){
            terrainTransition.getShader().setUniformTexture("normalMap", terrainNoiseTex, 3);
        } else {
            terrainTransition.getShader().setUniformTexture("normalMap", assets->terrainNormalMap, 3);
        }
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
    
    //  OverLayer with information
    {
        
            terrainTex.begin();{
                ofClear(0,0);
                
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
                //
                ofPoint textCenter = assets->font.getStringBoundingBox(text,0,0).getCenter();
                ofPushStyle();
                ofClear(0,0);
                ofSetColor(255, textAlpha*255.0);
                ofPushMatrix();
                ofTranslate(textOffset);
                ofRotate(-90);
                ofScale(textScale, textScale);
                //        assets->font.drawString(text, -textCenter.x, -textCenter.y );
                assets->font.drawString(text, 0.0, -textCenter.y );
                ofPopMatrix();
                
                ofPopStyle();

                
                
                
            }terrainTex.end();
    }
    
    // Mask
    //
    for(int i=0;i<3;i++){
        terrainMask[i].begin();{
            // Draw the mask
            //
            drawMask(i);
            
        } terrainMask[i].end();
    }
}

void RenderRadar::selfDraw(){
   
    int view = 0;
    if(currentViewPort > 0){
        view = currentViewPort - 1;
        calibration->ground[view].begin();
    }
    
    lightsBegin();
    
    materials["MATERIAL 1"]->begin();

    ofSetColor(255);
    
    //  TERRAIN
    //
    ofPushMatrix();
    ofSetSmoothLighting(false);
    
    terrainShader.begin();
    terrainShader.getShader().setUniformTexture("radarMask", radarTexture, 0);
    terrainShader.getShader().setUniformTexture("terrainArea", assets->terrainMask, 1);
    terrainShader.getShader().setUniformTexture("background", terrainTransitionTex.dst->getTextureReference(), 2);
    terrainShader.getShader().setUniformTexture("overlayer", terrainTex, 3);
    terrainShader.getShader().setUniformTexture("ripples", ripples, 4);
    terrainShader.getShader().setUniformTexture("terrainMask", terrainMask[currentViewPort].getTextureReference(), 5);
    terrainShader.getShader().setUniform3f("ripplesColor", ripplesColor.r,ripplesColor.g,ripplesColor.b);
    terrainShader.getShader().setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
    terrainShader.getShader().setUniform1f("resolution", assets->terrainResolution());
    
    assets->terrainMesh.draw();
    terrainShader.end();
    
    terrainMeshShader.begin();
    terrainMeshShader.getShader().setUniformTexture("radarMask", radarTexture, 0);
    terrainMeshShader.getShader().setUniformTexture("terrainArea", assets->terrainMask, 1);
    terrainMeshShader.getShader().setUniformTexture("background", terrainTransitionTex.dst->getTextureReference(), 2);
    terrainMeshShader.getShader().setUniformTexture("overlayer", terrainTex, 3);
    terrainMeshShader.getShader().setUniformTexture("ripples", ripples, 4);
    terrainMeshShader.getShader().setUniformTexture("terrainMask", terrainMask[currentViewPort].getTextureReference(), 5);
    terrainMeshShader.getShader().setUniform3f("ripplesColor", ripplesColor.r,ripplesColor.g,ripplesColor.b);
    terrainMeshShader.getShader().setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
    terrainMeshShader.getShader().setUniform1f("radarPct",radarPct);
    terrainMeshShader.getShader().setUniform1f("resolution", assets->terrainResolution());
    assets->terrainMesh.drawWireframe();
    terrainMeshShader.end();
    
    ofPopMatrix();
    
    lightsEnd();
    
    if(currentViewPort > 0){
        int view = currentViewPort - 1;
        calibration->ground[view].end();
    }
    
    
    

    //  SHOE
    //
    
    if(currentViewPort > 0){
        int view = currentViewPort - 1;
        calibration->shoe[view].begin();
    }
    lightsBegin();
    
    
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
        shoeTransition.getShader().setUniform3f("srcColor"+ofToString(i+1),
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
    
    
    lightsEnd();
    
    if(currentViewPort > 0){
        int view = currentViewPort - 1;
        calibration->shoe[view].end();
    }

    
    materials["MATERIAL 1"]->end();
}

void RenderRadar::selfDrawOverlay(){
    if(bDebug){
        
        ofPushMatrix();
        ofScale(0.5, 0.5);
        ripples.draw(0,0);
        ofPopMatrix();
        audioIn.texture.draw(0,0,audioIn.texture.getWidth()*2.0,10);
        
        float paletteSize = 10;
        float margin = 15;
        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(ofGetWidth()-assets->terrainResolution()*0.5,0);
        
        ofPushMatrix();{
            ofScale(0.5, 0.5);
            shoeTex.dst->draw(0, 0);
            assets->shoeUVWireframe.draw(0, 0,shoeTex.dst->getWidth(), shoeTex.dst->getHeight());
        } ofPopMatrix();
        
        ofTranslate(-margin*1.5-paletteSize,0);
        for(int i = 0; i < dstPalette.size(); i++){
            ofSetColor(dstPalette[i]);
            ofCircle(margin, i*(paletteSize*2.0+5)+paletteSize*0.5+5, paletteSize);
        }
        
        ofSetColor(255);
        
        ofPopStyle();
        ofPopMatrix();
    }
}