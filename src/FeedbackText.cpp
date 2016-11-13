//
//  FeedbackText.cpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/10/16.
//
//

#include "FeedbackText.hpp"

void FeedbackText::setup(string _text, bool isError){
    
    text = _text;
    
    if (isError){
        col.set(200,0,0);
    }else{
        col.set(0,0,0);
    }
    
    timer = 10;
}

void FeedbackText::update(float deltaTime){
    timer -= deltaTime;
}

void FeedbackText::draw(int x, int y){
    ofSetColor(col);
    ofDrawBitmapString(text, x, y);
}
