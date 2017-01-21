#pragma once

#include "ofMain.h"
#include "Slider.hpp"
#include "Button.hpp"
#include "FeedbackText.hpp"
#include "CheckBox.hpp"

#include "ofxXmlSettings.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawPage(int pageNum);
    
    void startPrinting();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void clearCards();
    void loadDirectory(string path);
    void loadFile(ofFile file);
    bool fileIsOK(ofFile thisFile);
    
    void changePage(int dir);
    
    float getFboWidth();
    float getFboHeight();
    
    void makeFeedback(string text, bool isError);
    
    string getPrcString(float prc);
    
    void saveSettings();
    void loadSettings();
    void restoreDefaults();
    
	float prevFrameTime;

    int cardW, cardH;
    
    //what's being printed
    vector<ofImage> cards;
    int curPage;
    int maxPageNum;
    
    int filesAddedThisDrag;
    
    
    bool printing;
    ofFbo fbo;
    
    //int pdfW, pdfH;
    
    //showing a preview
    int demoPageNum;
    float previewX;
    float previewY;
    float previewW;
    float previewH;
    
    //feedback
    vector<FeedbackText> feedback;
    ofVec2f feedbackStartPos;
    
    //options n shit
    int borderPadding, cardPadding;
    float borderPaddingPrc;
    float cardPaddingPrc;
    bool fillCardSpacing;
    bool useGreyscale;
    
    Slider borderPadSlider, cardPadSlider;
    CheckBox fillSpacingBox, greyscaleCheckBox, restoreDefaultsBox;
    
    Button pageButtons[2];
    Button clearButton, printButton;
    
		
};
