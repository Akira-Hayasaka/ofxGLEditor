/*
 *  ClipBoard.h
 *  ofxLuaLive
 *
 *  Created by Makira on 11/07/09.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 *	Updated by Dan Wilcox <danomatika@gmail.com> 2012
 */
#pragma once

#include "ofMain.h"

/// a system clipboard object
///
/// Note: currently only supported on Mac OS X
class ClipBoard {
	
public:
	
	/// get the last text entry from the system clipboard
	/// returns "" on error
	string getText();
	
	/// set a new text entry in the system clipboard
	bool setText(const string text);
	
};
