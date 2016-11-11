//
//  Button.hpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/8/16.
//
//

#ifndef Button_hpp
#define Button_hpp

#include "ofMain.h"

class Button{
public:
    
    void setup(string _label, float x, float y, float w, float h);
    void draw();
    
    void mouseMoved(int x, int y );
    bool mousePressed(int x, int y, int button);
    
    ofRectangle box;
    string label;
    
    ofColor regColor, hoverColor;
    bool mouseInside;
    
};

#endif /* Button_hpp */
