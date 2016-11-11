//
//  Slider.cpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/8/16.
//
//

#include "Slider.hpp"


void Slider::setup(float x, float y, float w, float min, float max, float startVal){
    pos.set(x,y);
    width = w;
    
    minVal = min;
    maxVal = max;
    
    handleDrawSize = 5;
    handleClickSize = 20;
    
    setVal(startVal);
}
void Slider::draw(){
    ofSetColor(0);
    ofDrawLine(pos.x, pos.y, pos.x+width, pos.y);
    ofFill();
    ofDrawCircle(handleX, pos.y, handleDrawSize);
}

void Slider::mousePressed(int x, int y, int button){
    if (ofDistSquared(x,y, handleX, pos.y) < handleClickSize*handleClickSize){
        beingDragged = true;
    }
    
}
void Slider::mouseDragged(int x, int y, int button){
    if (beingDragged){
        handleX = CLAMP(x, pos.x, pos.x+width);
    }
}
void Slider::mouseReleased(){
    beingDragged = false;
}

void Slider::setVal(float val){
    handleX = ofMap(val, minVal, maxVal, pos.x, pos.x+width);
}

float Slider::getVal(){
    return ofMap(handleX, pos.x, pos.x+width, minVal, maxVal);
}
