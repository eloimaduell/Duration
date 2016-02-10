#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetupOpenGL(1600,940, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofSetWindowTitle("Duration");
	ofRunApp( new ofApp());

}
