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
#include "RenderLasers.h"

enum ANIMATION_STYLE {
    RADAR = 0,
    LASERS
};

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
    
    void loadScene();
    void loadAnimation(ANIMATION_STYLE _animation);
    
    RenderEngine *renderEngine;
    
    ImageQueue imageQueue;
    CalibrationLoader calibration;
    
    ofVboMesh       terrainMesh,shoeMesh;
    ofTexture       terrainDepthMap, terrainNormalMap;
    ofPoint         sceneMin,sceneMax;
};
