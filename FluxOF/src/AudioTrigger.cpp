#include "AudioTrigger.h"

void AudioTrigger::setup(){
    osc.setup("localhost", 8888);
}

void AudioTrigger::setupUI(){
    gui->addSlider("test", 0., 100., 0.);
}

void AudioTrigger::guiEvent(ofxUIEventArgs &e){
    if(e.getName() == "test"){
        ofxOscMessage msg;
        msg.setAddress("/test");
        msg.addFloatArg(e.getSlider()->getValue() );
    }
}