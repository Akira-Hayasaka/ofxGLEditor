#pragma once

#include "ofMain.h"
#include "ofxEditor.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		
		ofxEditor editor;
		ofxEditorColorScheme colorScheme;
};
