/*
 *  ClipBoard.h
 *  ofxLuaLive
 *
 *  Created by Makira on 11/07/09.
 *  Copyright 2011 ・‥…―━━━―…‥・yesMAYBEno・‥…―━━━―…‥・. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class ClipBoard {
	
public:
	char* getTextFromPasteboard();
	OSStatus setTextToPasteboard(const char* byteArrayIndex);
	
};
