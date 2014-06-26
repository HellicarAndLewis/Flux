#pragma once

#include "ofMain.h"

//  Addons
//
#include "ofxMultiGLFWWindow.h"


//  Loaders
//
#include "CalibrationLoader.h"
#include "RenderAssets.h"

// Controllers
//
#include "ImageQueue.h"
#include "NodeCommunication.h"
#include "AudioTrigger.h"


//  Animation Prototypes
//
#include "RenderRadar.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void loadAnimation();
    
    RenderEngine *renderEngine;
    
    RenderAssets renderAssets;
    
    ImageQueue imageQueue;
    NodeCommunication nodeCommunication;
    AudioTrigger audioTrigger;
    
    CalibrationLoader calibration;
    
    bool guiShown;
    GLFWwindow * guiWindow;
    vector<ofRectangle> monitorSizes;

};
