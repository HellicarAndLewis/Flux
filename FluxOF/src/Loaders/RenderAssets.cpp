//
//  RenderAssets.cpp
//  FluxOF
//
//  Created by Jonas Jongejan    Firma HalfdanJ on 16/06/14.
//
//

#include "RenderAssets.h"

int RenderAssets::terrainResolution(){
    return terrainDepthMap.getWidth();
}

void RenderAssets::onDirectoryWatcherItemModified(const DirectoryWatcherManager::DirectoryEvent& evt)
{
    ofLogError("Modified: " + evt.item.path());
    
    if (evt.item.path().find("shoeDetails1") != std::string::npos) {
        dispatch_async(dispatch_get_main_queue(), ^{
            ofLoadImage(shoeDetails1, "textures/shoeDetails1.png");
        });
    }
    if (evt.item.path().find("shoeDetails2") != std::string::npos) {
        dispatch_async(dispatch_get_main_queue(), ^{
            ofLoadImage(shoeDetails2, "textures/shoeDetails2.png");
        });
    }
    
    if (evt.item.path().find("terrainMask1") != std::string::npos) {
        dispatch_async(dispatch_get_main_queue(), ^{
            ofLoadImage(terrainMask1, "textures/terrainMask1.png");
        });
    }
    if (evt.item.path().find("terrainMask2") != std::string::npos) {
        dispatch_async(dispatch_get_main_queue(), ^{
            ofLoadImage(terrainMask2, "textures/terrainMask2.png");
        });
    }

  
    
    if (evt.item.path().find("shoeMask1") != std::string::npos) {
        dispatch_async(dispatch_get_main_queue(), ^{
            ofLoadImage(shoeMask1, "textures/shoeMask1.png");
        });
    }
    if (evt.item.path().find("shoeMask2") != std::string::npos) {
        dispatch_async(dispatch_get_main_queue(), ^{
            ofLoadImage(shoeMask2, "textures/shoeMask2.png");
        });
    }
    

}


void RenderAssets::load(){
    watcher.registerAllEvents(this);
    
    std::string folderToWatch = ofToDataPath("textures", true);
    bool listExistingItemsOnStart = true;
    
    watcher.addPath(folderToWatch, listExistingItemsOnStart, &fileFilter);

    
    //  Load modeles
    //
    ofxAssimpModelLoader shoeModel;
    ofxAssimpModelLoader terrainModel;
    shoeModel.loadModel("models/shoeModel.obj");
    terrainModel.loadModel("models/terrainModel.obj");
    
    //  Calculate Scene Min/Max values
    //
    terrainMesh = terrainModel.getMesh(0);
    shoeMesh = shoeModel.getMesh(0);
    sceneMax.set(0, 0, 0);
    sceneMin.set(100000, 100000, 100000);
	for(unsigned int i = 0; i < terrainMesh.getVertices().size(); i++) {
		if (terrainMesh.getVertices()[i].x < sceneMin.x ){
            sceneMin.x = terrainMesh.getVertices()[i].x;
        }
        
        if (terrainMesh.getVertices()[i].x > sceneMax.x ){
            sceneMax.x = terrainMesh.getVertices()[i].x;
        }
        
        if (terrainMesh.getVertices()[i].y < sceneMin.y ){
            sceneMin.y = terrainMesh.getVertices()[i].y;
        }
        
        if (terrainMesh.getVertices()[i].y > sceneMax.y ){
            sceneMax.y = terrainMesh.getVertices()[i].y;
        }
        
        if (terrainMesh.getVertices()[i].z < sceneMin.z ){
            sceneMin.z = terrainMesh.getVertices()[i].z;
        }
        
        if (terrainMesh.getVertices()[i].z > sceneMax.z ){
            sceneMax.z = terrainMesh.getVertices()[i].z;
        }
	}
    
    for(unsigned int i = 0; i < shoeMesh.getVertices().size(); i++) {
        if (shoeMesh.getVertices()[i].x < sceneMin.x ){
            sceneMin.x = shoeMesh.getVertices()[i].x;
        }
        
        if (shoeMesh.getVertices()[i].x > sceneMax.x ){
            sceneMax.x = shoeMesh.getVertices()[i].x;
        }
        
        if (shoeMesh.getVertices()[i].y < sceneMin.y ){
            sceneMin.y = shoeMesh.getVertices()[i].y;
        }
        
        if (shoeMesh.getVertices()[i].y > sceneMax.y ){
            sceneMax.y = shoeMesh.getVertices()[i].y;
        }
        
        if (shoeMesh.getVertices()[i].z < sceneMin.z ){
            sceneMin.z = shoeMesh.getVertices()[i].z;
        }
        
        if (shoeMesh.getVertices()[i].z > sceneMax.z ){
            sceneMax.z = shoeMesh.getVertices()[i].z;
        }
	}
    
    //  Load ARB Textures
    //
    ofLoadImage(terrainDepthMap, "textures/terrainDepthMap.png");
    ofLoadImage(terrainNormalMap, "textures/terrainNormalMap.png");
    ofLoadImage(terrainAreasMap,"textures/terrainAreasMap.png");
    ofLoadImage(terrainMask1, "textures/terrainMask1.png");
    ofLoadImage(terrainMask2, "textures/terrainMask2.png");
    ofLoadImage(shoeDetails1, "textures/shoeDetails1.png");
    ofLoadImage(shoeDetails2, "textures/shoeDetails2.png");
    ofLoadImage(shoeMask1, "textures/shoeMask1.png");
    ofLoadImage(shoeMask2, "textures/shoeMask2.png");
    
    //  Load SQR Textures
    //
    ofDisableArbTex();
    ofLoadImage(shoeColorMask, "textures/shoeAreasMap.png");
    ofLoadImage(shoeUVWireframe, "textures/shoe_uv_wireframe.png");
    ofLoadImage(terrainUVWireframe, "textures/terrain_uv_wireframe.png");
    ofEnableArbTex();
    
    font.loadFont("fonts/AdiHaus", 90);

}