#pragma once

#include "ofMain.h"
#include "ModelLoader.h"
#include "CalibrationLoader.h"

class RenderEngine {
public:
    void setup(ModelLoader * modelLoader, CalibrationLoader * calibrationLoader);
    void draw();

    
    ModelLoader * modelLoader;
    CalibrationLoader * calibration;
    
    ofEasyCam cam;
    ofLight pointLight;
    
    
private:
    void render();

};