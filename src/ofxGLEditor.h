/*
 *  ofxGLEditor.h
 *  LiveCoding
 *
 *  Created by Makira on 11/07/05.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 *	Updated by Dan Wilcox <danomatika@gmail.com> 2012
 */

#pragma once

#include "ofMain.h"
#include "GLEditor.h"
#include "Unicode.h"
#include "ClipBoard.h"

using namespace fluxus;

class ofxGLEditor {

public:
	
	ofxGLEditor();
	
	void setup(string fontFile = "fonts/DroidSansMono.ttf");
	void keyPressed(int key);
	void draw();	
	void reShape();
	void pasteFromClipBoard();
	void copyToClipBoard();
	void saveToFile();
	
	
	vector<GLEditor*> glEditor;
	int currentEditor;
	ClipBoard clipBoard;
	
	ofEvent<string> doCompileEvent;
	
	///
	/// keep track of pressed modifer keys
	///
	/// from Rick Companje's ofxKeyMap 2009.09.17
	/// https://github.com/companje/ofxKeyMap
	///
	inline bool isAltPressed()		{return bAltPressed;}
	inline bool isShiftPressed()	{return bShiftPressed;}
	inline bool isControlPressed()	{return bControlPressed;}
	
private:

	bool bAltPressed;
	bool bShiftPressed;
	bool bControlPressed;
};