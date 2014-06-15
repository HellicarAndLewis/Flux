//
//  RenderEngine.cpp
//  FluxOF
//
//  Created by Patricio Gonzalez Vivo on 6/14/14.
//
//

#include "RenderEngine.h"

void RenderEngine::setCalibration(CalibrationLoader *_calibration){
    calibration = _calibration;
}

void RenderEngine::addUiClass(UIClass *_ui){
    guiAdd( *_ui );
}

void RenderEngine::selfSceneTransformation(){
    if(!simulatorMode){
        calibration->shoe[currentViewPort].begin();
    }
}

vector<ofColor> getColorPalette(ofImage &_img, int _nColors){
    
    vector<ofColor> palette;
    
    //  Extract Color Palette
    //
    ofImage smallImg;
    smallImg = _img;
    smallImg.resize(_img.getWidth()*0.25, _img.getHeight()*0.25);
    const int colorCount = 5;
    const int sampleCount = smallImg.getHeight() * smallImg.getWidth();
    cv::Mat colorSamples( sampleCount, 1, CV_32FC3 );
    
    // get our pixels
    unsigned char * pixels = smallImg.getPixels();
    
    // build our matrix of samples
    cv::MatIterator_<cv::Vec3f> sampleIt = colorSamples.begin<cv::Vec3f>();
    for(int i=0; i<sampleCount; i++){
        int pos = i * 3;
        *sampleIt = cv::Vec3f( pixels[pos], pixels[pos+1], pixels[pos+2] );
        sampleIt++;
    }
    
    // call kmeans
    cv::Mat labels, clusters;
    cv::kmeans( colorSamples, _nColors, labels, cv::TermCriteria(), 2, cv::KMEANS_RANDOM_CENTERS, clusters ); //cv::TermCriteria::COUNT, 8, 0
    
    for( int i = 0; i < _nColors; ++i ){
        ofColor clusterColor = ofColor( clusters.at<cv::Vec3f>(i,0)[0], clusters.at<cv::Vec3f>(i,0)[1], clusters.at<cv::Vec3f>(i,0)[2] );
        palette.push_back(clusterColor);
    }
    
    return palette;
}

void RenderEngine::startTransitionTo(QueueItem queueItem){
    text = queueItem.username;
    
    //  Load Image
    //
    colorPalette = getColorPalette(queueItem.image, 5);
    
    //  Keep the image as a destinationTexture
    //
    shoeTexB = queueItem.image.getTextureReference();
    
    //  Start Animation
    //
    timeline->setPercentComplete(0.0);
    timeline->play();
}

bool RenderEngine::transitionDone(){
    return !timeline->getIsPlaying();
}