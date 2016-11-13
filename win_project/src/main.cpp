#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(600,700,OF_WINDOW);			// <-------- setup the GL context

	//hides the console
	HWND handleWindow; AllocConsole(); handleWindow = FindWindowA("ConsoleWindowClass", NULL); ShowWindow(handleWindow, 0);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

	
}
