#pragma once

#include "ofMain.h"
#include "ModelLoader.h"
#include "CalibrationLoader.h"
#include "RenderEngine.h"

class ofApp : public ofBaseApp{
    
public:
    
    CalibrationLoader calibrationLoader;
    ModelLoader modelLoader;
    RenderEngine renderEngine;
    
    
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
    
};
