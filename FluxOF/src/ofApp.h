#pragma once

#include "ofMain.h"

//  Addons
//
#include "ofxAssimpModelLoader.h"

//  Loaders
//
#include "RenderEngine.h"
#include "ImageQueue.h"
#include "CalibrationLoader.h"

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
    
    RenderEngine *renderEngine;
    
    ImageQueue imageQueue;
    CalibrationLoader calibration;
    
    ofxAssimpModelLoader shoeModel;
    ofxAssimpModelLoader terrainModel;
};
