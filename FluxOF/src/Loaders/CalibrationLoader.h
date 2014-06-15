#pragma once

//#include "ofxOpenCV.h"
#include "ofxCv.h"

class CalibrationLoaderInstance {
public:
    cv::Mat rvec, tvec;
	ofMatrix4x4 modelMatrix;
	ofxCv::Intrinsics intrinsics;
    
    void load(string path);
    
    void begin();
    void end();
    
};


class CalibrationLoader {
public:
    void load();
    
    CalibrationLoaderInstance shoe[2];
    CalibrationLoaderInstance ground[2];
    
};




