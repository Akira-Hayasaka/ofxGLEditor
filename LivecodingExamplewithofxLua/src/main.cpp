/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxLua for documentation
 *
 */
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {

    ofSetupOpenGL(1024,768, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
