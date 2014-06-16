#pragma once

#include "ofxAssimpModelLoader.h"


class RenderAssets {
public:
    void load();
    int terrainResolution();
    
    ofVboMesh       terrainMesh,shoeMesh;
    ofTexture       terrainDepthMap, terrainNormalMap, terrainMask0;
    ofPoint         sceneMin,sceneMax;

};