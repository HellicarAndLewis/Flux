#pragma once

#include "ofxOsc.h"
#include "UIClass.h"

class AudioTrigger : public UIClass{
public:
    void setup();
    
    void update();

    string  getClassName(){return "AUDIO-TRIGGER";};
    
    

protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
};