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
    ofLoadImage(terrainDepthMap, "models/terrainDepthMap.png");
    ofLoadImage(terrainNormalMap, "models/terrainNormalMap.png");
    ofLoadImage(terrainAreasMap,"models/terrainMask.png");
    ofLoadImage(terrainMask1, "textures/terrainMask1.png");
    ofLoadImage(terrainMask2, "textures/terrainMask2.png");
    
    
    //  Load SQR Textures
    //
    ofDisableArbTex();
    ofLoadImage(shoeColorMask, "models/SHOE_MASKS.png");
    ofLoadImage(shoeUVWireframe, "textures/shoe_uv_wireframe.png");
    ofLoadImage(terrainUVWireframe, "textures/terrain_uv_wireframe.png");
    ofEnableArbTex();
    
    font.loadFont("fonts/AdiHaus", 90);

}