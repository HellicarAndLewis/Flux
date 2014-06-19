//
//  UIBufferIn.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 11/19/13.
//
//

#include "UIBufferIn.h"

UIBufferIn::UIBufferIn(){
    bufferSize = 512;
    sampleRate = 44100;
    
//    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);

    audioIn = new float[bufferSize];
	audioBins.resize(fft->getBinSize());
    middleBins = new float[fft->getBinSize()];
    for(int i = 0; i < fft->getBinSize(); i++){
        middleBins[i] = 0;
    }
    
    pixels.allocate(fft->getBinSize(),1,3);
    texture.allocate(fft->getBinSize(),1, GL_RGB16F);
}

void UIBufferIn::setup(int _sampleRate, int _bufferSize){
    sampleRate = _sampleRate;
    bufferSize = _bufferSize;
}

UIBufferIn::~UIBufferIn(){
    delete []audioIn;
    delete []middleBins;
    delete fft;
}

void UIBufferIn::setupUI(){
    gui->addWaveform("Buffer", audioIn, bufferSize);
    gui->addSpectrum("FFT", middleBins, fft->getBinSize());
}

void UIBufferIn::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
//    if(name == "ENABLE"){
//        if(bEnable){
//            start();
//        } else {
//            stop();
//        }
//    }
}

void UIBufferIn::start(){
    stream.setup(0, 1, sampleRate, bufferSize, 4);
    stream.setInput(this);
    
    ofAddListener(ofEvents().draw,this,&UIBufferIn::draw);
}

void UIBufferIn::stop(){
    stream.stop();
    ofRemoveListener(ofEvents().draw, this, &UIBufferIn::draw);
}

void UIBufferIn::audioReceived(float * input, int bufferSize, int nChannels ){
    float maxValue = 0;
	for(int i = 0; i < bufferSize; i++) {
		if(abs(input[i]) > maxValue) {
			maxValue = abs(input[i]);
		}
        audioIn[i] = input[i];
	}
	for(int i = 0; i < bufferSize; i++) {
		input[i] /= maxValue;
	}
	
	fft->setSignal(input);
	
	float* curFft = fft->getAmplitude();
	memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
	
	maxValue = 0;
	for(int i = 0; i < fft->getBinSize(); i++) {
		if(abs(audioBins[i]) > maxValue) {
			maxValue = abs(audioBins[i]);
		}
	}
	for(int i = 0; i < fft->getBinSize(); i++) {
		audioBins[i] /= maxValue;
	}
	
	soundMutex.lock();
    for(int i = 0; i < fft->getBinSize(); i++){
        middleBins[i] = audioBins[i];
    }
	soundMutex.unlock();
}

void UIBufferIn::draw(ofEventArgs & args){
    for (int i = 0; i < fft->getBinSize(); i++){
        ofFloatColor color;
        
        
        color.setBrightness(1.0);
        color.setSaturation(1.0);
        color.setHue(fft->getAmplitude()[i]);//middleBins[i]);
        
        pixels.setColor(i, 0, color);
    }
    texture.loadData(pixels);
}