//
//  ColorPallete.h
//
//  Taked from openFramework implementation of ToxyLibs https://github.com/borg/ofxColourTheory
//
//

#pragma once

#include "ofMain.h"

#include "ofxCv.h"

namespace ColorTheory{
    
    float   luminance(ofColor col);
    ofColor getDarkest(vector<ofColor> &colors);
    float   distanceBetween(ofColor a,ofColor b);
    void    sortByDistance(vector<ofColor> &colors);
    vector<ofVec2f> getRYB_WHEEL();
    ofColor rotateRYB(ofColor col,int theta);
    ofColor getComplement(ofColor col);
    vector<ofColor> getColorPalette(ofImage &_img, int _nColors);
   
};
