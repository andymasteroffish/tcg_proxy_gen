#include "ofApp.h"

string versionText = "version 0.1";

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(250);
    
    printing = false;
    
    previewX = 15;
    previewY = 150;
    previewW = 300;
    
    clearCards();
    
    //sliders
    float sliderX = 340;
    float sliderW = 240;
    borderPadSlider.setup(sliderX, previewY+30, sliderW, 0, 1, 0.071);
    cardPadSlider.setup(sliderX, borderPadSlider.pos.y+50, sliderW, 0, 1, 0);
    
    //checkbox
    fillSpacingBox.setup("Fill space between cards", sliderX, cardPadSlider.pos.y+25);
    greyscaleCheckBox.setup("Black & white only", sliderX, cardPadSlider.pos.y+60);
    
    //buttons
    float buttonW = 80;
    float buttonH = 40;
    
    //other buttons
    printButton.setup("PRINT", sliderX, cardPadSlider.pos.y+95, sliderW, buttonH);
    clearButton.setup("CLEAR ALL", sliderX, printButton.box.y+150, sliderW, buttonH);
    
    //paging
    for (int i=0; i<2; i++){
        string label = i==0 ? "prev page" : "next page";
        float xPos = previewX + previewW/2 + 100 * (i==0 ? -1 : 1);
        if (i==1)   xPos -= buttonW;
        pageButtons[i].setup(label, xPos, 45, buttonW, buttonH);
    }
    
    //feedback
    feedbackStartPos.set(previewX, ofGetHeight()-10);
    makeFeedback(versionText, false);
    makeFeedback("for best results, use images of the same size", false);
    
    loadSettings();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //get the values form sliders
    borderPaddingPrc = borderPadSlider.getVal();
    cardPaddingPrc = cardPadSlider.getVal();
    fillCardSpacing = fillSpacingBox.isChecked;
    useGreyscale = greyscaleCheckBox.isChecked;
    
    //in case the cardW changed, update the padding values
    borderPadding = cardW * borderPaddingPrc;
    cardPadding = cardW * cardPaddingPrc;
    
    //if we're printing, create the FBO and save it until we're done
    if (printing){
        fbo.begin();
        ofClear(255);
        
        ofSetColor(255);
        ofFill();
        ofDrawRectangle(0,0, fbo.getWidth(), fbo.getHeight());
        
        if (curPage >= 0){
            drawPage(curPage);
            if ( curPage >= maxPageNum ){
                printing = false;
                ofEndSaveScreenAsPDF();
            }
        }
        
        fbo.end();
        
        curPage++;
    }
    
    for (int i=feedback.size()-1; i>=0; i--){
        feedback[i].update();
        if (feedback[i].timer < 0){
            feedback.erase( feedback.begin()+i );
        }
    }
    //borderPaddingPrc = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
    //cardPaddingPrc = ofMap(mouseY, 0, ofGetHeight(), 0, 1);
    
    //cout<<"edge: "<<borderPadding<<endl;
    //cout<<"card: "<<cardPadding<<endl;

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //credits
    ofSetColor(0);
    ofDrawBitmapString("Proxy Generator by Andy Wallace\nandy@andymakes.com  @andy_makes", previewX, 15);
    
    
    
    //show a preview of the first page
    float scale = previewW/getFboWidth();
    //cout<<"scale "<<scale<<endl;
    ofPushMatrix();
    ofTranslate(previewX, previewY);
    ofScale(scale, scale);
    
    ofSetColor(255);
    drawPage(demoPageNum);
    
    ofPopMatrix();
    
    //the border
    ofNoFill();
    ofSetColor(0);
    previewH = getFboHeight()*scale;
    ofDrawRectangle(previewX,previewY,previewW, previewH);
    
    //draw some shit on top of that
    
    ofSetColor(0);
    int numPages = MAX(1, (cards.size()-1)/9 +1) ;
    if (cards.size() == 0){
        numPages = 1;
    }
    string pageText = "page: "+ofToString(demoPageNum+1)+"/"+ofToString(numPages);
    ofDrawBitmapString(pageText, previewX+previewW/2-pageText.length()*4, previewY - 50);
    
    ofDrawBitmapString("click a card to duplicate\nright click to delete", previewX, previewY - 30);
    
    string blurrText = "preview will be blurry";
    ofDrawBitmapString(blurrText, previewX, previewY - 5);
    
    //buttons
    for (int i=0; i<2; i++){
        pageButtons[i].draw();
    }
    clearButton.draw();
    
    if (!printing){
        printButton.draw();
    }else{
        string printingText ="Printing page "+ofToString(curPage+1)+" of "+ofToString(maxPageNum+1);
        
        ofDrawBitmapString(printingText, printButton.box.x+printButton.box.width/2-printingText.length()*4, printButton.box.y+printButton.box.height/2+4);
    }
    
    //options
    ofSetColor(0);
    ofDrawBitmapString("Border Padding: "+ofToString( (int)(borderPaddingPrc*100))+"%", borderPadSlider.pos.x, borderPadSlider.pos.y - 15);
    borderPadSlider.draw();
    
    ofDrawBitmapString("Card Spacing: "+ofToString( (int)(cardPaddingPrc*100))+"%", cardPadSlider.pos.x, cardPadSlider.pos.y - 15);
    cardPadSlider.draw();

    fillSpacingBox.draw();
    greyscaleCheckBox.draw();
    
    //fake drag area
    ofNoFill();
    ofSetColor(0);
    ofDrawRectangle(borderPadSlider.pos.x, 10, borderPadSlider.width, previewY-20);
    ofDrawBitmapString("drag images or folders here", borderPadSlider.pos.x+5, 25);
    
    //some feedback
    int feedbackSpacing = 15;
    float feedbackKillLine = previewY+getFboHeight()*scale+20;
    float curY = feedbackStartPos.y;
    for (int i=feedback.size()-1; i>=0; i--){
        if (curY > feedbackKillLine){
            feedback[i].draw(feedbackStartPos.x, curY);
            curY -= feedbackSpacing;
        }else{
            feedback.erase(feedback.begin()+i);
        }
        
    }
}


//--------------------------------------------------------------
void ofApp::drawPage(int pageNum){
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    int curCard = pageNum*9;
    
    for (int r=0; r<3; r++){
        for (int c=0; c<3; c++){
            if (curCard < cards.size()){
                
                //fill in the padding if aplicable
                if (fillCardSpacing){
                    ofSetColor(0);
                    ofFill();
                    int startX = MAX(0, (cardW+cardPadding)*c+borderPadding-cardPadding);
                    int startY = MAX(0,  (cardH+cardPadding)*r+borderPadding-cardPadding);
                    int width = MIN(getFboWidth()-startX, cardW+cardPadding*2);
                    int height = MIN(getFboHeight()-startY, cardH+cardPadding*2);
                    ofDrawRectangle(startX, startY, width, height);
                }
                
                //draw the card
                ofSetColor(255);
                
                if (!useGreyscale){
                    cards[curCard].draw((cardW+cardPadding)*c+borderPadding, (cardH+cardPadding)*r+borderPadding, cardW, cardH);
                }else{
                    ofImage greyPic = cards[curCard];
                    greyPic.setImageType(OF_IMAGE_GRAYSCALE);
                    greyPic.draw((cardW+cardPadding)*c+borderPadding, (cardH+cardPadding)*r+borderPadding, cardW, cardH);
                }
                
                curCard ++;
            }
        }
    }
    
    ofDisableSmoothing();
    ofDisableAntiAliasing();
}

//--------------------------------------------------------------
void ofApp::startPrinting(){
	cout << "start to print" << endl;
    if (cards.size() == 0){
        makeFeedback("Nothing to print", true);
        return;
    }
    
    ofFileDialogResult result = ofSystemSaveDialog("proxies.pdf", "select where to save");
    if (result.bSuccess){
    
        printing = true;
        
        curPage = -1;   //needs a frame with nothing before it starts saving
        maxPageNum = floor(cards.size()/9);
        
        fbo.allocate(getFboWidth(), getFboHeight(), GL_RGB);
        
        //make sure it has the .pdf extension
        string fileName = result.filePath;
        if ( fileName.substr( fileName.length() - 4 ) != ".pdf"){
            fileName += ".pdf";
        }
        
        //save
        ofBeginSaveScreenAsPDF(fileName, true, false, ofRectangle(0,0,fbo.getWidth(),fbo.getHeight()));
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_LEFT){
        changePage(-1);
    }
    if (key == OF_KEY_RIGHT){
        changePage(1);
    }
    
    if (key == 'p' || key == 'P'){
        startPrinting();
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	for (int i=0; i<2; i++){
        pageButtons[i].mouseMoved(x, y);
    }
    printButton.mouseMoved(x, y);
    clearButton.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    borderPadSlider.mouseDragged(x, y, button);
    cardPadSlider.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (printing){
        return;
    }

	//dumb fix for Windows which sometimes stops firing mouseMoved after a pdf has been printed
	mouseMoved(x, y);
    
    borderPadSlider.mousePressed(x, y, button);
    cardPadSlider.mousePressed(x, y, button);
    
    fillSpacingBox.mousePressed(x, y, button);
    greyscaleCheckBox.mousePressed(x, y, button);
    
    if (pageButtons[0].mousePressed(x,y,button)){
        changePage(-1);
    }
    if (pageButtons[1].mousePressed(x,y,button)){
        changePage(1);
    }
    
    if (printButton.mousePressed(x, y, button)){
        startPrinting();
    }
    if (clearButton.mousePressed(x, y, button)){
        clearCards();
    }
    
    //did they click in the preview window?
    if (x > previewX && x < previewX+previewW && y > previewY && y < previewY+previewH){
        int newX = mouseX - previewX;
        int newY = mouseY - previewY;
        int col = newX/(previewW/3);
        int row = newY/(previewH/3);
        
        //get the card number for that
        int num = demoPageNum*9 + row*3 + col;
        if (num < cards.size()){
            //left click duplciates
            if (button == 0){
                cards.insert(cards.begin()+num, cards[num]);
            }
            //right click kills
            if (button == 2){
                cards.erase(cards.begin()+num);
                if (demoPageNum > (cards.size()-1)/9){
                    changePage(-1);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    borderPadSlider.mouseReleased();
    cardPadSlider.mouseReleased();
    
    saveSettings();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    filesAddedThisDrag = 0;

    if (dragInfo.files.size() > 0){
        for(int i = 0; i < dragInfo.files.size(); i++){
            cout<<dragInfo.files[i]<<endl;
            ofFile file;
            file.open(dragInfo.files[i]);
            if (file.exists()){
                if (file.isFile()){
                    loadFile(file);
                }
                if (file.isDirectory()){
                    loadDirectory(file.path());
                }
            }
        }
    }
    
    makeFeedback("files added: "+ofToString(filesAddedThisDrag), false);
}


//--------------------------------------------------------------
void ofApp::clearCards(){
    cardW = 35;
    cardH = 50;
    cards.clear();
    demoPageNum = 0;
}
    
//--------------------------------------------------------------
void ofApp::loadDirectory(string path){
    
    ofDirectory dir;
    dir.listDir(path);
    
    makeFeedback("opening "+path, false);
    
    
    if (!dir.isDirectory()){
        cout<<"no good"<<endl;
    }
    
    for (int i=0; i<dir.size(); i++){
        if(dir.getFile(i).isDirectory()){
            loadDirectory(dir.getFile(i).path());
        }else{
            loadFile(dir.getFile(i));
        }
    }
    
    if (dir.size() == 0){
        makeFeedback("No card images found in folder", true);
    }
    
}

//--------------------------------------------------------------
void ofApp::loadFile(ofFile file){
    if (fileIsOK(file)) {
        //cout << "load " << file.path() << endl;
        ofImage newCard;
        newCard.load(file.path());
        cards.push_back(newCard);
        
        //is this the biggest image so far?
        if (newCard.getWidth() > cardW){
            cardW = newCard.getWidth();
            cardH = newCard.getHeight();
        }
        
        //bring us to the last page
        int maxPages = (cards.size()-1)/9;
        demoPageNum = maxPages;
        
        filesAddedThisDrag++;
    }else{
        cout<<file.path()<<" is no good"<<endl;
    }
}


//--------------------------------------------------------------
bool ofApp::fileIsOK(ofFile thisFile) {
    string ext = thisFile.getExtension();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "gif") {
        return true;
    }
    
    makeFeedback(thisFile.getFileName()+" was rejected because it is not a recognized file type", true);
    makeFeedback("use .jpg .jpeg .png or .gif", true);
    return false;
}

//--------------------------------------------------------------
void ofApp::changePage(int dir){
    demoPageNum += dir;
    
    if (demoPageNum < 0)    demoPageNum = 0;
    
    int maxPages = (cards.size()-1)/9;
    if (demoPageNum > maxPages)    demoPageNum = maxPages;

	if (cards.size() == 0) {
		demoPageNum = 0;
	}
}

//--------------------------------------------------------------
float ofApp::getFboWidth(){
    return cardW*3 + borderPadding*2 + cardPadding*2;
}
//--------------------------------------------------------------
float ofApp::getFboHeight(){
    return cardH*3 + borderPadding*2 + cardPadding*2;
}

//--------------------------------------------------------------
void ofApp::makeFeedback(string text, bool isError){
    FeedbackText fee;
    fee.setup(text, isError);
    feedback.push_back(fee);
}

//--------------------------------------------------------------
void ofApp::saveSettings(){
    
#ifdef TARGET_OSX
    string filePath = ofFilePath::getCurrentExeDir() + "../Resources/proxy_gen_settings.xml";
#else
    string filePath = "../proxy_gen_settings.xml";
#endif
    
    ofxXmlSettings xml;
    xml.clear();
    
    xml.setValue("border_padding", borderPadSlider.getVal());
    xml.setValue("card_padding", cardPadSlider.getVal());
    xml.setValue("fill_spacing", fillSpacingBox.isChecked ? 1 : 0);
    xml.setValue("use_greyscale", greyscaleCheckBox.isChecked ? 1 : 0);
    
    xml.save(filePath);
}

//--------------------------------------------------------------
void ofApp::loadSettings(){
    
#ifdef TARGET_OSX
    string filePath = ofFilePath::getCurrentExeDir() + "../Resources/proxy_gen_settings.xml";
#else
    string filePath = "../proxy_gen_settings.xml";
#endif
    
    ofxXmlSettings xml;
    xml.clear();
    
    if (xml.loadFile(filePath)){
        borderPadSlider.setVal( xml.getValue("border_padding", 0.071));
        cardPadSlider.setVal( xml.getValue("card_padding", 0.0));
        fillSpacingBox.setValue( xml.getValue("fill_spacing", 0) == 1);
        greyscaleCheckBox.setValue( xml.getValue("use_greyscale", 0) == 1);
    }else{
        cout<<"no settings to load, using defaults"<<endl;
    }
}


