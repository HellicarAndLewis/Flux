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
    audioTerrainDebug.loadFrag(getDataPath()+"shaders/audioTerrainDebug.frag");
    
    ripples.allocate(assets->terrainResolution(), assets->terrainResolution(), GL_RGBA);
    
    //  SHOES
    //
    shoeTransition.load(getDataPath()+"shaders/shoesTrans");
    shoeLaserTransition.load(getDataPath()+"shaders/shoesLaserTrans");
    
    //  TERRAIN
    //
    terrainShader.load(getDataPath()+"shaders/terrain");
    
    radarShader.loadFrag(getDataPath()+"shaders/radar.frag");
    radarTexture.allocate(assets->terrainResolution(),assets->terrainResolution());
    
    circleShader.loadFrag(getDataPath()+"shaders/circle.frag");
    
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
    testFbo.allocate(1024,1024, GL_RGB);
    ofEnableArbTex();
}

void RenderRadar::selfSetupGuis(){
    
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audioIn);
    
    // 2D (just frag shaders)
    guiAdd(audioTerrain);
    guiAdd(audioTerrainDebug);
    
    guiAdd(radarShader);
    
    guiAdd(circleShader);
    
    guiAdd(terrainNoise);
    guiAdd(terrainTransition);
    
    //  3D
    guiAdd(terrainShader);
    guiAdd(terrainMeshShader);
    
    guiAdd(shoeTransition);
    guiAdd(shoeLaserTransition);
}

void RenderRadar::selfGuiEvent(ofxUIEventArgs &e){
}

void RenderRadar::selfSetupSystemGui(){
    sysGui->addLabel("TEST Patterns");
    
    vector<string> items;
    items.push_back("disabled");
    items.push_back("grid");
    items.push_back("wireframeTerrain");
    items.push_back("wireframeShoe");
    items.push_back("normalmap");
    sysGui->addRadio("Test pattern", items);
    
    sysGui->addLabel("Radar");
    sysGui->add2DPad("Radar_Center",ofVec2f(0,assets->terrainResolution()),ofVec2f(0,assets->terrainResolution()),&radarCenter);
    sysGui->addSlider("Radar_Pct", 0.0, 1.0, &radarPct);
    sysGui->addSlider("Radar_Alpha",0,255, &radarAlpha);
    sysGui->addSlider("Radar_Height",assets->sceneMin.y,assets->sceneMax.y, &radarHeight);
    
    sysGui->addLabel("Text");
    sysGui->add2DPad("Text_offset", ofVec2f(0,assets->terrainResolution()),ofVec2f(0,assets->terrainResolution()), &textOffset);
    sysGui->addSlider("Text_Alpha", 0.0, 1.0, &textAlpha);
    sysGui->addSlider("Text_Scale", 0.0, 1.0, &textScale);
    
    sysGui->addLabel("Ripples");
    sysGui->addSlider("Lerp_To_RadarColor", 0., 0.1, &ripplesColorLerpToRadar);
    sysGui->addSlider("Lerp_To_WhiteColor", 0., 0.1, &ripplesColorLerpToWhite);
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

void RenderRadar::selfUpdate(){
    RenderEngine::selfUpdate();
    
    if(ofGetElapsedTimef()<10.0){
        ripples.begin();
        ofClear(0,255);
        ripples.end();
    }
    
    //  AUDIO REACTION
    //  -----------------------------------------
    {
        ripples.begin();
        if (audioTerrain.bEnable) {
            audioTerrain.begin();
            audioTerrain.setUniformTexture("heightMap", assets->terrainDepthMap, 0);
            audioTerrain.setUniformTexture("audioFft", audioIn.texture, 1);
            audioTerrain.setUniform1f("audioFftSize", audioIn.texture.getWidth());
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
            glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
            glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
            glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
            glEnd();
            audioTerrain.end();
        } else {
            ofClear(0,255);
        }
        ripples.end();
        ripples.update();
    }
    
    
    //  Color the radar
    //
    if(dstPalette.size()>0){
        radarColor.set(dstPalette[3]);
        radarColor.setBrightness(1.0);
        radarColor.setSaturation(1.0);
    }
    
    if(ripplesColorLerpToRadar>0.0){
        ripplesColor.r = ofLerp(ripplesColor.r, radarColor.r, ripplesColorLerpToRadar);
        ripplesColor.g = ofLerp(ripplesColor.g, radarColor.g, ripplesColorLerpToRadar);
        ripplesColor.b = ofLerp(ripplesColor.b, radarColor.b, ripplesColorLerpToRadar);
    }
    
    if(ripplesColorLerpToWhite>0.0){
        ripplesColor.r = ofLerp(ripplesColor.r, 1, ripplesColorLerpToWhite);
        ripplesColor.g = ofLerp(ripplesColor.g, 1, ripplesColorLerpToWhite);
        ripplesColor.b = ofLerp(ripplesColor.b, 1, ripplesColorLerpToWhite);
    }
    
    //  TERRAIN ANIMATION
    //  -----------------------------------------
    
    //  RadarMask
    {
        radarTexture.begin();
        ofClear(0,0);
        
        radarShader.begin();
        radarShader.setUniform1f("pct", radarPct);
        radarShader.setUniform2f("center", radarCenter.x, radarCenter.y);
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
        terrainTransitionTex.dst->begin();{
            
            ofClear(0,0);
            
            terrainTransition.begin();{
                terrainTransition.setUniformTexture("backbuffer", *terrainTransitionTex.src, 0);
                terrainTransition.setUniformTexture("depthMap", assets->terrainDepthMap, 1);
                terrainTransition.setUniformTexture("terrainAreas", assets->terrainAreasMap, 2);
                if(terrainNoise.bEnable){
                    terrainTransition.setUniformTexture("normalMap", terrainNoiseTex, 3);
                } else {
                    terrainTransition.setUniformTexture("normalMap", assets->terrainNormalMap, 3);
                }
                terrainTransition.setUniformTexture("radarTex", radarTexture, 5);
                
                for(int i = 0; i < srcPalette.size(); i++){
                    terrainTransition.setUniform3f("srcColor"+ofToString(i+1),
                                                               ((float)srcPalette[i].r)/255.0,
                                                               ((float)srcPalette[i].g)/255.0,
                                                               ((float)srcPalette[i].b)/255.0);
                }
                
                for(int i = 0; i < dstPalette.size(); i++){
                    terrainTransition.setUniform3f("dstColor"+ofToString(i+1),
                                                               ((float)dstPalette[i].r)/255.0,
                                                               ((float)dstPalette[i].g)/255.0,
                                                               ((float)dstPalette[i].b)/255.0);
                }
                
                terrainTransition.setUniform1f("resolution", assets->terrainResolution());
                
                glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
                glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
                glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
                glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
                glEnd();
            } terrainTransition.end();
        } terrainTransitionTex.dst->end();
    }
    
    //  OverLayer with information
    {
        
        terrainTex.begin();{
            ofClear(0,0);
            
            circleShader.begin();
            circleShader.setUniform3f("color",radarColor.r,radarColor.g,radarColor.b);
            circleShader.setUniform2f("center", radarCenter.x, radarCenter.y);
            circleShader.setUniform1f("resolution", assets->terrainResolution());
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
            glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
            glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
            glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
            glEnd();
            circleShader.end();
            
            // Circle line
            //
            
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(radarCenter);
            
            // radar line
            ofSetLineWidth(1);
            ofSetColor(radarColor,radarAlpha);
            float radius = assets->terrainResolution();
            float angle = TWO_PI*radarPct;
            ofLine(0,0,radius*cos(angle),radius*sin(angle));
            
            ofPopMatrix();
            ofPopStyle();
            
            // username text
            //
            ofPoint textCenter = assets->font.getStringBoundingBox(text,0,0).getCenter();
            ofPushStyle();
            ofSetColor(255, textAlpha*255.0);
            ofPushMatrix();
            ofTranslate(textOffset);
            ofRotate(-90);
            ofScale(textScale, textScale);
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
            //drawMask(i);
            
        } terrainMask[i].end();
    }
    
    // TEST
    //
    ofxUIRadio * ui = ( ofxUIRadio * )sysGui->getWidget("Test pattern");
    testMode = ui->getActiveName();
    testPatternEnabled = (testMode.length() != 0 && testMode != "disabled");
    if(testPatternEnabled){
        
        testFbo.begin();
        
        if(testMode == "grid"){
            
            ofClear(50,0,0,255);
            for(int x=0;x<1024;x+= 15){
                ofLine(x, 0, x, 1024);
            }
            for(int x=0;x<1024;x+= 15){
                ofLine(0, x, 1024, x);
            }
        } else if (testMode == "normalmap"){
            assets->terrainNormalMap.draw(0, 0, 1024, 1024);
        } else {
            ofClear(0);
        }
        
        testFbo.end();
    }
}

void RenderRadar::drawTerrain(int viewport){
    ofEnableDepthTest();
    
    
    lightsBegin();
    
    materials["MATERIAL 1"]->begin();
    
    ofSetColor(255);
    
    //  TERRAIN
    //
    ofPushMatrix();
    ofSetSmoothLighting(false);
    
    
    if(terrainShader.bEnable){
        terrainShader.begin();
        terrainShader.setUniformTexture("terrainAreas", assets->terrainAreasMap, 0);
        terrainShader.setUniformTexture("background", terrainTransitionTex.dst->getTextureReference(), 1);
        terrainShader.setUniformTexture("overlayer", terrainTex, 2);
        terrainShader.setUniformTexture("ripples", ripples, 3);
        terrainShader.setUniformTexture("radarMask", radarTexture, 4);
        
        terrainShader.setUniform3f("ripplesColor", ripplesColor.r,ripplesColor.g,ripplesColor.b);
        terrainShader.setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
        
        terrainShader.setUniform1f("resolution", assets->terrainResolution());
        
        assets->terrainMesh.draw();
       
        terrainShader.end();
    }
    
    if(terrainMeshShader.bEnable){
        terrainMeshShader.begin();
        
        terrainMeshShader.setUniformTexture("terrainAreas", assets->terrainAreasMap, 0);
        terrainMeshShader.setUniformTexture("background", terrainTransitionTex.dst->getTextureReference(), 1);
        terrainMeshShader.setUniformTexture("overlayer", terrainTex, 2);
        terrainMeshShader.setUniformTexture("ripples", ripples, 3);
        terrainMeshShader.setUniformTexture("radarMask", radarTexture, 4);
        
        terrainMeshShader.setUniform3f("ripplesColor", ripplesColor.r,ripplesColor.g,ripplesColor.b);
        terrainMeshShader.setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
        
        terrainMeshShader.setUniform1f("resolution", assets->terrainResolution());
        
        assets->terrainMesh.drawWireframe();
        terrainMeshShader.end();
    }
    
    ofPopMatrix();
   
    lightsEnd();

    materials["MATERIAL 1"]->end();

    
}

void RenderRadar::drawShoeBackground(int viewport){
    
    //  SHOE
    //
    
    if(currentViewPort > 0){
        int view = currentViewPort - 1;
        calibration->shoe[view].begin();
    }
    
    if(shoeTransition.bEnable){
        lightsBegin();
        
        ofPushMatrix();
        ofSetSmoothLighting(true);
        
        ofDisableArbTex();
        
        if(shoeTransition.bEnable){
            shoeTransition.begin();
            shoeTransition.setUniform1f("radarHeight", radarHeight);
            shoeTransition.setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
            shoeTransition.setUniformTexture("srcTexture",shoeTex.src->getTextureReference(), 0);
            shoeTransition.setUniformTexture("dstTexture",shoeTex.dst->getTextureReference(), 1);
            shoeTransition.setUniformTexture("colorMaskTexture", assets->shoeColorMask, 2);
            shoeTransition.setUniform1i("splitLaser", simulatorMode);
            
            for(int i = 0; i < srcPalette.size(); i++){
                shoeTransition.setUniform3f("srcColor"+ofToString(i+1),
                                                        ((float)srcPalette[i].r)/255.0,
                                                        ((float)srcPalette[i].g)/255.0,
                                                        ((float)srcPalette[i].b)/255.0);
            }
            
            for(int i = 0; i < dstPalette.size(); i++){
                shoeTransition.setUniform3f("dstColor"+ofToString(i+1),
                                                        ((float)dstPalette[i].r)/255.0,
                                                        ((float)dstPalette[i].g)/255.0,
                                                        ((float)dstPalette[i].b)/255.0);
            }
            
            assets->shoeMesh.draw();
            shoeTransition.end();
        }
        
        ofEnableArbTex();
        ofPopMatrix();
        lightsEnd();
    }
}



void RenderRadar::drawShoeForeground(int viewport){
    
    //  SHOE
    //
    
    if(currentViewPort > 0){
        int view = currentViewPort - 1;
        calibration->shoe[view].begin();
    }
    
    if(shoeLaserTransition.bEnable){
        lightsBegin();
        
        ofPushMatrix();
        ofSetSmoothLighting(true);
        
        ofDisableArbTex();
        
        if(shoeLaserTransition.bEnable){
            shoeLaserTransition.begin();
            shoeLaserTransition.setUniform1f("radarHeight", radarHeight);
            shoeLaserTransition.setUniform3f("radarColor",radarColor.r,radarColor.g,radarColor.b);
            shoeLaserTransition.setUniformTexture("srcTexture",shoeTex.src->getTextureReference(), 0);
            shoeLaserTransition.setUniformTexture("dstTexture",shoeTex.dst->getTextureReference(), 1);
            shoeLaserTransition.setUniformTexture("colorMaskTexture", assets->shoeColorMask, 2);
            
            for(int i = 0; i < srcPalette.size(); i++){
                shoeLaserTransition.setUniform3f("srcColor"+ofToString(i+1),
                                                             ((float)srcPalette[i].r)/255.0,
                                                             ((float)srcPalette[i].g)/255.0,
                                                             ((float)srcPalette[i].b)/255.0);
            }
            
            for(int i = 0; i < dstPalette.size(); i++){
                shoeLaserTransition.setUniform3f("dstColor"+ofToString(i+1),
                                                             ((float)dstPalette[i].r)/255.0,
                                                             ((float)dstPalette[i].g)/255.0,
                                                             ((float)dstPalette[i].b)/255.0);
            }
            
            assets->shoeMesh.draw();
            shoeLaserTransition.end();
        }
        
        ofEnableArbTex();
        
        ofPopMatrix();
        
        lightsEnd();
    }
}


void RenderRadar::selfDrawOverlay(){
    if(bDebug){
        
        ofPushMatrix();
        ofScale(0.5, 0.5);
        ripples.draw(0,0);
        terrainTex.draw(0,0);
        ofPopMatrix();
        audioIn.texture.draw(0,0,audioIn.texture.getWidth()*2.0,10);
        
        if (audioTerrainDebug.bEnable) {
            ofSetColor(255);
            audioTerrainDebug.begin();
            audioTerrainDebug.setUniformTexture("audioFft", audioIn.texture, 0);
            audioTerrainDebug.setUniform1f("audioFftSize", audioIn.texture.getWidth());

            //audioIn.texture.draw(audioIn.texture.getWidth()*2.0,0,audioIn.texture.getWidth()*2.0,150);
            glPushMatrix();
            
            glTranslated(audioIn.texture.getWidth()*2.0, 0, 0);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
            glTexCoord2f(audioIn.texture.getWidth(), 0); glVertex3f(audioIn.texture.getWidth()*2.0, 0, 0);
            glTexCoord2f(audioIn.texture.getWidth(),100.); glVertex3f(audioIn.texture.getWidth()*2.0, 150, 0);
            glTexCoord2f(0,100.);  glVertex3f(0,150, 0);
            glEnd();
            
            glPopMatrix();

            audioTerrainDebug.end();
            /*audioTerrain.begin();
            audioTerrain.setUniformTexture("heightMap", assets->terrainDepthMap, 0);
            audioTerrain.setUniformTexture("audioFft", audioIn.texture, 1);
            audioTerrain.setUniform1f("audioFftSize", audioIn.texture.getWidth());
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
            glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
            glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
            glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
            glEnd();
            audioTerrain.end();*/
        }
        
        
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