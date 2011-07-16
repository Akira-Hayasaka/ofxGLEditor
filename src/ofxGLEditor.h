/*
 *  ofxGLEditor.h
 *  LiveCoding
 *
 *  Created by Makira on 11/07/05.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxKeyMap.h"
#include "GLEditor.h"
#include "Unicode.h"
#include "ClipBoard.h"

using namespace fluxus;

class ofxGLEditor {

public:
	
	void setup(string fontFile = "fonts/DroidSansMono.ttf");
	void keyPressed(int key);
	void draw();	
	void reShape();
	bool isAltPressed(); // should be used in keypress callback
	void pasteFromClipBoard();
	void copyToClipBoard();
	void saveToFile();
	
	
	ofxKeyMap kmap;	
	vector<GLEditor*> glEditor;
	int currentEditor;
	ClipBoard clipBoard;
	
	ofEvent<string> doCompileEvent;
	
};