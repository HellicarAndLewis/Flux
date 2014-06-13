#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
	
    project = new Prototype01();
	project->setup();
	project->play();
 
    string path = "images";
    ofDirectory backgroundsDir(path);
    if(backgroundsDir.exists()){
		backgroundsDir.listDir();
        int total = backgroundsDir.getFiles().size();
        for (int i = 0; i < total; i++) {
            images.push_back( backgroundsDir.getName(i) );
        }
    }
    
    cout << images[0] << endl;
    ((Prototype01*)project)->startTransitionTo("@addidas", "images/"+images[0]);
    ((Prototype01*)project)->getTimeline()->setDurationInSeconds(10);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 't'){
        ((Prototype01*)project)->startTransitionTo("@addidas", "images/"+images[ofRandom(images.size()-1.0)]);
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
