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
#include "UIShader.h"

#include "ofxMultiGLFWWindow.h"
#include "ofxColorPalette.h"

class RenderEngine : public UITime3DProject {
public:
    
    virtual void addUiClass(UIClass *uiClass);
    virtual void setCalibration(CalibrationLoader *_calibration);
    
    virtual string getSystemName(){ return "RenderEngine-SuperClass";}
    
    virtual void startTransitionTo(QueueItem queueItem);
    virtual bool transitionDone();
        
    virtual void selfSetup();
    virtual void selfUpdate();
    
    void drawMask(int viewPort);
    
    void draw(ofEventArgs & args);
    void selfPostDraw();
    ofFbo& getRenderTarget(int viewNumber);
    
    UIBufferIn          audioIn;
    
    CalibrationLoader   *calibration;
    RenderAssets        *assets;
    
    PingPong            shoeTex;
    
    vector<ofColor>     srcPalette;
    vector<ofColor>     dstPalette;
    
    ofPoint             textOffset;
    string              text;
    float               textAlpha;
    float               textScale;
    
    bool                simulatorMode;

    ofxMultiGLFWWindow *glfw;
};