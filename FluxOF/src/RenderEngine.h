//
//  RenderEngine.h
//  FluxOF
//
//  Created by Patricio Gonzalez Vivo on 6/14/14.
//
//

#pragma once

#include "ofxCv.h"
#include "UITime3DProject.h"

//  INPUTS
//
#include "UIBufferIn.h"
#include "QueueItem.h"
#include "CalibrationLoader.h"

class RenderEngine : public UITime3DProject {
public:
    
    virtual void addUiClass(UIClass *uiClass);
    virtual void setCalibration(CalibrationLoader *_calibration);
    
    virtual string getSystemName(){ return "RenderEngine-SuperClass";}
    
    virtual void startTransitionTo(QueueItem queueItem);
    virtual bool transitionDone();
    
    virtual void selfSceneTransformation();
    
    ofVboMesh       shoeMesh;
    ofVboMesh       terrainMesh;
    ofTexture       terrainDepthMap;
    ofTexture       terrainNormalMap;
    float           terrainResolution;
    ofPoint         sceneMin,sceneMax;
    
    ofTexture       shoeTexA,shoeTexB;
    
    vector<ofColor> colorPalette;
    
    string          text;
    
    bool            simulatorMode;
    
    CalibrationLoader   *calibration;
    UIBufferIn          audioIn;
};