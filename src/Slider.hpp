//
//  Slider.hpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/8/16.
//
//

#ifndef Slider_hpp
#define Slider_hpp

#include "ofMain.h"

class Slider{
public:
    
    void setup(float x, float y, float w, float min, float max, float startVal);
    void draw();
    
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased();
    
    float setVal(float val);
    float getVal();
    
    ofVec2f pos;
    float width;
    
    float handleX;
    
    float minVal, maxVal;
    
    float handleDrawSize;
    float handleClickSize;
    
    bool beingDragged;
    
};

#endif /* Slider_hpp */
