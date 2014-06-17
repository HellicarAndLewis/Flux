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


void RenderAssets::load(){
    //  Load modeles
    //
    ofxAssimpModelLoader shoeModel;
    ofxAssimpModelLoader terrainModel;
    shoeModel.loadModel("models/adidas_unwrap_001.obj");//LOCKED_SHOE_ROTATION.obj");
    terrainModel.loadModel("models/LOCKED_TERRAFORM.obj");
    
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
    
    //  Load Textures
    //
    ofLoadImage(terrainDepthMap, "models/terrainDepthMap.png");
    ofLoadImage(terrainNormalMap, "models/terrainNormalMap.png");
    ofLoadImage(terrainMask1, "models/terrainMask1.png");
    ofLoadImage(terrainMask2, "models/terrainMask2.png");
    
    ofDisableArbTex();
    ofLoadImage(shoeMask, "models/SHOE_MASKS.png");
    ofEnableArbTex();

}