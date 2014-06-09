#pragma once

#include "ofMain.h"
#include "ModelLoader.h"

class RenderEngine {
public:
    void setup(ModelLoader * modelLoader);
    void draw();
    
    ModelLoader * modelLoader;
    
    ofEasyCam cam;
    ofLight pointLight;

};