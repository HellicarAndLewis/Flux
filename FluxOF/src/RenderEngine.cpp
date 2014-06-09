#include "RenderEngine.h"

void RenderEngine::setup(ModelLoader * _modelLoader){
    modelLoader = _modelLoader;
    
    cam.setDistance(800);
    
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
}

void RenderEngine::draw(){
    ofEnableDepthTest();
    
    ofEnableLighting();
    pointLight.enable();
    
    cam.begin();
    modelLoader->shoe.draw(OF_MESH_FILL);
    
    cam.end();
    
    pointLight.disable();

}
