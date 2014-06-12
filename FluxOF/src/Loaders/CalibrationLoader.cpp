#include "CalibrationLoader.h"

//--------------------------

void CalibrationLoader::setup(){
    shoe[0].load("shoe_calib_1");
}



//--------------------------


void CalibrationLoaderInstance::load(string path){
    // load the calibration-advanced yml
    cv::FileStorage fs(ofToDataPath( path+"/calibration-advanced.yml", true), cv::FileStorage::READ);
    
    cv::Mat cameraMatrix;
    cv::Size2i imageSize;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["imageSize"][0] >> imageSize.width;
    fs["imageSize"][1] >> imageSize.height;
    fs["rotationVector"] >> rvec;
    fs["translationVector"] >> tvec;
    
    intrinsics.setup(cameraMatrix, imageSize);
    modelMatrix = ofxCv::makeMatrix(rvec, tvec);
}

void CalibrationLoaderInstance::begin(){
    glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
    
    intrinsics.loadProjectionMatrix(10, 2000);
    ofxCv::applyMatrix(modelMatrix);
}


void CalibrationLoaderInstance::end(){
    glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}