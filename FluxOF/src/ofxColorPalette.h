#pragma once
#include "ofMain.h"
#include "ofxCv.h"

namespace ofxColorPalette {
    vector<ofColor> getColorPalette(ofImage &_img, int numColors);
    ofColor getComplement(ofColor color);
};