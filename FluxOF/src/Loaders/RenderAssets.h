#pragma once

#include "ofxAssimpModelLoader.h"


class RenderAssets {
public:
    void load();
    int terrainResolution();
    
    ofVboMesh       terrainMesh,shoeMesh;
    ofTexture       terrainDepthMap, terrainNormalMap, terrainMask, terrainMask1, terrainMask2;
    ofTexture       shoeColorMask, shoeUVWireframe;
    ofPoint         sceneMin,sceneMax;
    
    ofTrueTypeFont  font;
};