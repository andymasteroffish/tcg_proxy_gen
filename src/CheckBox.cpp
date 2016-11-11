//
//  CheckBox.cpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/11/16.
//
//

#include "CheckBox.hpp"


void CheckBox::setup(string _label, int x, int y){
    label = _label;
    isChecked = false;
    
    int boxSize = 16;
    box.set(x, y, boxSize, boxSize);
    
}

void CheckBox::draw(){
    ofSetColor(0);
    ofNoFill();
    ofDrawRectangle(box);
    
    ofDrawBitmapString(label, box.x+box.width+10, box.y+box.height-2);
    
    if (isChecked){
        ofDrawLine(box.x, box.y, box.x+box.width, box.y+box.height);
        ofDrawLine(box.x+box.width, box.y, box.x, box.y+box.height);
    }
    
}

void CheckBox::mousePressed(int x, int y, int button){
    if (box.inside(x, y)){
        isChecked = !isChecked;
    }
}

void CheckBox::setValue(bool newVal){
    isChecked = newVal;
}
