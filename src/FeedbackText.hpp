//
//  FeedbackText.hpp
//  proxy_gen
//
//  Created by Andrew Wallace on 11/10/16.
//
//

#ifndef FeedbackText_hpp
#define FeedbackText_hpp

#include "ofMain.h"

class FeedbackText{
public:
    
    void setup(string _text, bool isError);
    void update(float deltaTime);
    void draw(int x, int y);
    
    string text;
    ofColor col;
    
    float timer;
};

#endif /* FeedbackText_hpp */
