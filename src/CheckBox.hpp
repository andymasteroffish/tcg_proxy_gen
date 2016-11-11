//
//  CheckBox.hpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/11/16.
//
//

#ifndef CheckBox_hpp
#define CheckBox_hpp

#include "ofMain.h"

class CheckBox{
public:
    
    void setup(string _label, int x, int y);
    void draw();
    void mousePressed(int x, int y, int button);
    
    void setValue(bool newVal);
  
    ofRectangle box;
    bool isChecked;
    string label;
    
};

#endif /* CheckBox_hpp */
