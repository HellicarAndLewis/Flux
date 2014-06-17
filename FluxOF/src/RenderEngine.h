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
#include "RenderAssets.h"

#include "PingPong.h"

class RenderEngine : public UITime3DProject {
public:
    
    virtual void addUiClass(UIClass *uiClass);
    virtual void setCalibration(CalibrationLoader *_calibration);
    
    virtual string getSystemName(){ return "RenderEngine-SuperClass";}
    
    virtual void startTransitionTo(QueueItem queueItem);
    virtual bool transitionDone();
    
    virtual void selfSceneTransformation();
    
    void drawMask(int viewPort);
    
    UIBufferIn          audioIn;
    ofFloatPixels       audioPixels;
    ofTexture           audioTex;
    int                 audioBufferSize;
    
    CalibrationLoader   *calibration;
    RenderAssets        *assets;
    
    PingPong            shoeTex;
    
    vector<ofColor>     srcPalette;
    vector<ofColor>     dstPalette;
    
    ofFbo               textTex;
    ofPoint             textOffset;
    string              text;
    
    bool                simulatorMode;
    
    
};