#pragma once

#include "ofxAssimpModelLoader.h"
#include "ofxIO.h"

using namespace ofx::IO;

class RenderAssets {
public:
    void load();
    int terrainResolution();
    
    ofVboMesh       terrainMesh,shoeMesh;
    ofTexture       terrainDepthMap, terrainNormalMap, terrainAreasMap, terrainMask1, terrainMask2, terrainUVWireframe;
    ofTexture       shoeColorMask, shoeUVWireframe;
    
    ofTexture       shoeDetails1, shoeDetails2;
    ofPoint         sceneMin,sceneMax;
    
    ofTrueTypeFont  font;
    
    void onDirectoryWatcherItemAdded(const DirectoryWatcherManager::DirectoryEvent& evt)
    {
      //  ofLogError("Added:    " + evt.item.path());
    }
    
    void onDirectoryWatcherItemRemoved(const DirectoryWatcherManager::DirectoryEvent& evt)
    {
      //  ofLogError("Removed:  " + evt.item.path());
    }
    
    void onDirectoryWatcherItemModified(const DirectoryWatcherManager::DirectoryEvent& evt);
    
    void onDirectoryWatcherItemMovedFrom(const DirectoryWatcherManager::DirectoryEvent& evt)
    {
      //  ofLogNotice("ofApp::onDirectoryWatcherItemMovedFrom") << "Moved From: " << evt.item.path();
    }
    
    void onDirectoryWatcherItemMovedTo(const DirectoryWatcherManager::DirectoryEvent& evt)
    {
      //  ofLogNotice("ofApp::onDirectoryWatcherItemMovedTo") << "Moved To: " << evt.item.path();
    }
    
    void onDirectoryWatcherError(const Poco::Exception& exc)
    {
        //ofLogError("ofApp::onDirectoryWatcherError") << "Error: " << exc.displayText();
    }
    
    DirectoryWatcherManager watcher;
    
    HiddenFileFilter fileFilter; // an example file filter
    
    std::deque<std::string> messages;

};