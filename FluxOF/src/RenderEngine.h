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
    
    void setImageQueue(UIClass *_imageQueue);
    void setCalibration(CalibrationLoader *_calibration);
    
    string getSystemName(){ return "RenderEngine-SuperClass";}
    
    void startTransitionTo(QueueItem queueItem);
    bool transitionDone();
    
    void selfSceneTransformation();
    
    ofVboMesh       shoeMesh;
    ofVboMesh       terrainMesh;
    ofTexture       terrainDepthMap;
    float           terrainResolution;
    
    ofTexture       shoeTexA,shoeTexB;
    
    vector<ofColor> colorPalette;
    
    string          text;
    
    bool            simulatorMode;
    
    CalibrationLoader   *calibration;
    UIBufferIn          audioIn;
};