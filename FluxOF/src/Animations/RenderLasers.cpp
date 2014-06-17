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
    terrainMapPrev.allocate(assets->terrainResolution(),assets->terrainResolution());
    lasersMask.allocate(assets->terrainResolution(),assets->terrainResolution());
    lasersMaskShader.loadFrag(getDataPath()+"shaders/lasersMask.frag");
    terrainTransition.loadFrag(getDataPath()+"shaders/terrainTrans.frag");
    terrainTransitionTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    terrainTransitionTex.clear();
    
    ofDisableArbTex();
    terrainTex.allocate(assets->terrainResolution(),assets->terrainResolution());
    terrainTex.begin();
    ofClear(0, 0);
    terrainTex.end();
    ofEnableArbTex();
}

void RenderLasers::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    
    guiAdd(audioIn);
    
    guiAdd(lasersMaskShader);
    guiAdd(terrainTransition);
    
    guiAdd(shoeTransition);
}

void RenderLasers::selfGuiEvent(ofxUIEventArgs &e){
    
}

void RenderLasers::selfSetupSystemGui(){
    sysGui->addLabel("Laser");
    sysGui->addSlider("Laser_Facus_X", assets->sceneMin.x, assets->sceneMax.x, &laserPosition.x);
    sysGui->addSlider("Laser_Facus_Y", assets->sceneMin.z, assets->sceneMax.z, &laserPosition.y);
    sysGui->addSlider("Laser_Color_R", 0 , 1.0, &laserColor.r);
    sysGui->addSlider("Laser_Color_G", 0 , 1.0, &laserColor.g);
    sysGui->addSlider("Laser_Color_B", 0 , 1.0, &laserColor.b);
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

bool RenderLasers::transitionDone(){
    if(RenderEngine::transitionDone()){
        terrainMapPrev.begin();
        terrainTransitionTex.dst->draw(0,0);
        terrainMapPrev.end();
        return true;
    } else {
        return false;
    }
}

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
    ofPoint laserPositionPct;
    laserPositionPct.x = ofMap(laserPosition.x,assets->sceneMin.x,assets->sceneMax.x,0,assets->terrainResolution(),true);
    laserPositionPct.y = ofMap(laserPosition.y,assets->sceneMin.z,assets->sceneMax.z,0,assets->terrainResolution(),true);
    
    lasersMask.begin();
    ofClear(0,0);
    lasersMaskShader.begin();
    lasersMaskShader.getShader().setUniform2f("pct",
                                              laserPositionPct.x,
                                              laserPositionPct.y);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(assets->terrainResolution(), 0); glVertex3f(assets->terrainResolution(), 0, 0);
    glTexCoord2f(assets->terrainResolution(), assets->terrainResolution()); glVertex3f(assets->terrainResolution(), assets->terrainResolution(), 0);
    glTexCoord2f(0,assets->terrainResolution());  glVertex3f(0,assets->terrainResolution(), 0);
    glEnd();
    lasersMaskShader.end();
    lasersMask.end();
    
    if(colorPalette.size() > 0){
        terrainTransitionTex.swap();
        terrainTransitionTex.dst->begin();
        terrainTransition.begin();
        //    terrainTransition.getShader().setUniformTexture("backbuffer", *terrainTransitionTex.src, 0);
        //    terrainTransition.getShader().setUniformTexture("normalMap", terrainNormalMap, 2);
        
        terrainTransition.getShader().setUniformTexture("depthMap", assets->terrainDepthMap, 0);
        terrainTransition.getShader().setUniformTexture("lasersMask", lasersMask, 1);
        terrainTransition.getShader().setUniformTexture("previusMap", terrainMapPrev, 2);
        
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
    terrainTex.begin();
    terrainTransitionTex.dst->draw(0, 0);
    ofPushStyle();
    
    ofSetColor(laserColor);
    ofLine(laserPositionPct.x,0, laserPositionPct.x, assets->terrainResolution());
    ofLine(0,laserPositionPct.y, assets->terrainResolution(), laserPositionPct.y);
    
    
    // Draw the mask
    //
    drawMask(0);
    
    ofPopStyle();
    terrainTex.end();
}

void RenderLasers::selfDraw(){
    materials["MATERIAL 1"]->begin();

    ofPushMatrix();

    ofSetColor(255);
    
    //  TERRAIN
    //
    ofSetSmoothLighting(false);
    
    ofDisableArbTex();
    terrainTex.getTextureReference().bind();
    assets->terrainMesh.draw();
    terrainTex.getTextureReference().unbind();
    ofEnableArbTex();
    
    //  SHOE
    //
    
    ofSetSmoothLighting(true);
    shoeTransition.begin();
    shoeTransition.getShader().setUniform3f("laserColor",laserColor.r,laserColor.g,laserColor.b);
    shoeTransition.getShader().setUniform2f("laserPosition", laserPosition.x, laserPosition.y);
    assets->shoeMesh.draw();
    shoeTransition.end();
    
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
        terrainTransitionTex.dst->draw(assets->terrainResolution()*0.25,0);
        
        ofPushMatrix();
        ofTranslate(assets->terrainResolution()*1.25,0);
        ofScale(0.5, 0.5);
        shoeTexB.draw(0, 0);
        ofPopMatrix();
        
        ofPushMatrix();
        ofScale(0.25, 0.25);
        assets->terrainDepthMap.draw(0,0);
        terrainMapPrev.draw(0,assets->terrainResolution());
        lasersMask.draw(0,assets->terrainResolution()*2.0);
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