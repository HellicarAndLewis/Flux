#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    renderEngine = NULL;
    
    guiShown = true;
    
    ofxMultiGLFWWindow *glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();

    //Look for number of monitors
    //
    int numberOfMonitors;
	GLFWmonitor** monitors = glfwGetMonitors(&numberOfMonitors);
    cout<<"Num windows: "<<numberOfMonitors<<endl;
    
    
    //Find the positions of the monitors
    //
    
    for (int iC=0; iC < numberOfMonitors; iC++){
        int xM; int yM;
        glfwGetMonitorPos(monitors[iC], &xM, &yM);
        const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[iC]);
        ofRectangle monitorRect(xM, yM, desktopMode->width, desktopMode->height);
        monitorSizes.push_back(monitorRect);
    }
    

    guiWindow = glfw->windows[0];
    
    // Setup the render views
    //
    if(numberOfMonitors == 3){
        
        //Set the main window to full HD
        ofSetWindowShape(1920, 1080);
        
              //Run through the windows, and create the additional render views
        for(int i=0;i<2;i++){
            GLFWwindow * window = glfw->createWindow();
            glfw->setWindow(window);
            
            if(i==0){
                ofSetWindowPosition(monitorSizes[0].width + 200, 100);
            } else {
                ofSetWindowPosition(monitorSizes[0].width + monitorSizes[1].width + 200, 100);
                ofToggleFullscreen();
            }
            ofToggleFullscreen();
        }
        
        glfw->setWindow(glfw->windows.at(0));
        glfw->showWindow(glfw->windows.at(0));
        
        ofSetWindowPosition(monitorSizes[0].width + monitorSizes[1].width +monitorSizes[2].width, 0);
        guiShown = false;

        
    }
    
    else if(numberOfMonitors == 2){
        
        
        //Find the positions of the monitors
        //
        vector<ofRectangle> monitorSizes;
        for (int iC=0; iC < numberOfMonitors; iC++){
            int xM; int yM;
            glfwGetMonitorPos(monitors[iC], &xM, &yM);
            const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[iC]);
            ofRectangle monitorRect(xM, yM, desktopMode->width, desktopMode->height);
            monitorSizes.push_back(monitorRect);
        }
        
        ofSetWindowPosition(monitorSizes[0].width + 200, 100);
        ofSetFullscreen(true);
        
           
        
        
    } else {
        ofToggleFullscreen();
    }
    
    
    //  Load STUFF
    //
    calibration.load();
    renderAssets.load();
    
    imageQueue.loadQueueFromFile();
    
    //  SETUP Node
    //
    nodeCommunication.imageQueue = &imageQueue;
    nodeCommunication.setup();
    
    // SETUP audio trigger
    audioTrigger.setup();
    
    //  start the RENDER
    //
    loadAnimation();
}


void ofApp::loadAnimation(){
    
    if (renderEngine != NULL){
        renderEngine->stop();
        delete renderEngine;
        renderEngine = NULL;
    }
    
    
    renderEngine = new RenderRadar();
    
    //  Link renderEngine to ImageQueue
    //
    imageQueue.renderEngine = renderEngine;

    //  Loading Resources (this could be pointers)
    //
    renderEngine->assets = &renderAssets;
        
    //  Setup The RenderEngine
    //
	renderEngine->setup();

    renderEngine->addUIClass(&imageQueue);
    renderEngine->addUIClass(&nodeCommunication);
    renderEngine->addUIClass(&audioTrigger);
    renderEngine->setCalibration(&calibration);
    
    //  Ready to GO
    //
	renderEngine->play();
    renderEngine->getTimeline()->setDurationInSeconds(5);
}

//--------------------------------------------------------------
void ofApp::update(){
    nodeCommunication.update();
    imageQueue.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'i'){

        ofxMultiGLFWWindow *glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();

        glfw->setWindow(guiWindow);

        if(guiShown){
            ofSetWindowPosition(monitorSizes[0].width + monitorSizes[1].width +monitorSizes[2].width, 0);
            guiShown = false;
        } else {
            ofSetWindowPosition(0, 0);
            guiShown = true;
        }

    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
