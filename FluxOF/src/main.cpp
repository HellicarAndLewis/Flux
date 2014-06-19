#include "ofMain.h"
#include "ofApp.h"
#include "ofxMultiGLFWWindow.h"
#include "ofGLProgrammableRenderer.cpp"

//========================================================================
int main( ){
//    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    
    ofxMultiGLFWWindow glfw;
    glfw.setOpenGLVersion(2,1); // must be set
    
    glfw.windowCount = 1;
	ofSetupOpenGL(&glfw,1024,768,OF_WINDOW);
    
	ofRunApp(new ofApp());
    
}
