//
//  Button.cpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/8/16.
//
//

#include "Button.hpp"


void Button::setup(string _label, float x, float y, float w, float h){
    box.set(x,y, w,h);
    label = _label;
    
    mouseInside = false;
    
    regColor.set(255);
    hoverColor.set(200);
}

void Button::draw(){
    if (mouseInside){
        ofSetColor(hoverColor);
    }else{
        ofSetColor(regColor);
    }
    
    ofFill();
    ofDrawRectangle(box);
    
    ofNoFill();
    ofSetColor(0);
    ofDrawRectangle(box);
    
    ofDrawBitmapString(label, box.x+box.width/2-label.length()*4, box.y+box.height/2+4);
}

void Button::mouseMoved(int x, int y ){
	mouseInside = box.inside(x, y);
}
bool Button::mousePressed(int x, int y, int button){
    return mouseInside;
}
