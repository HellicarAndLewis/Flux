#include "RenderEngine.h"

void RenderEngine::setup(ModelLoader * _modelLoader, CalibrationLoader * _calibration){
    modelLoader = _modelLoader;
    calibration = _calibration;
    
    cam.setDistance(800);
    
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
}

void RenderEngine::draw(){
    ofEnableDepthTest();
    
    
    
    ofEnableLighting();
    pointLight.enable();
    
    //cam.begin();
    calibration->shoe[0].begin();
    
    render();
    
    calibration->shoe[0].end();
    
    //cam.end();
    
    pointLight.disable();

}


void RenderEngine::render(){
    modelLoader->shoe.getMesh(0).drawWireframe();
    
}