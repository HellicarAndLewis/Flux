#pragma once

#include "ofxAssimpModelLoader.h"


class RenderAssets {
public:
    void load();
    int terrainResolution();
    
    ofVboMesh       terrainMesh,shoeMesh;
    ofTexture       terrainDepthMap, terrainNormalMap, terrainMask1, terrainMask2;
    ofTexture       shoeMask;
    ofPoint         sceneMin,sceneMax;

};