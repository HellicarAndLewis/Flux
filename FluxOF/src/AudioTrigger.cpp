#include "AudioTrigger.h"

void AudioTrigger::setup(){
    ofxMidiOut::listPorts();
    midiOut.openPort("IAC Driver Bus 1");
    
}

void AudioTrigger::setupUI(){
    gui->addButton("radarPing", false);
}

void AudioTrigger::guiEvent(ofxUIEventArgs &e){
    if(e.getName() == "radarPing"){
        if(e.getButton()->getValue()){
            e.getButton()->setValue(false);
            midiOut.sendNoteOn(1, 1);
        }
//        ofxOscMessage msg;
  //      msg.setAddress("/test");
    //    msg.addFloatArg(e.getSlider()->getValue() );

    }
}