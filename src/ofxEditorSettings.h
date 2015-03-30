/*
 * Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * See https://github.com/Akira-Hayasaka/ofxGLEditor for more info.
 */
#pragma once

#include "ofColor.h"
#include "ofConstants.h"

/// internal editor settings which can be shared between instances
class ofxEditorSettings {

	public:

		ofxEditorSettings();
	
		/// set the displayed tab width, must be at least 1, default: 4
		void setTabWidth(unsigned int numSpaces);
		unsigned int getTabWidth();
		
		/// convert tabs to spaces? performed when setting or editing text and
		/// *cannot* be undone
		void setConvertTabs(bool convert=true);
		bool getConvertTabs();
		
		/// set overall text alpha, clamped to 0.1 - 1.0, default: 1.0
		void setAlpha(float alpha);
		float getAlpha();
		
		/// text color, default: white
		/// overridden by color scheme if a scheme is set
		void setTextColor(ofColor color);
		ofColor& getTextColor();
	
		/// text offset shadow color, default: black
		void setTextShadowColor(ofColor color);
		ofColor& getTextShadowColor();
		
		/// cursor color, default: yellow w/ alpha
		void setCursorColor(ofColor color);
		ofColor& getCursorColor();
		
		/// selection color, default: green w/ alpha
		void setSelectionColor(ofColor color);
		ofColor& getSelectionColor();
	
		/// matching chars highlight color, default: blue w/ alpha
		void setMatchingCharsColor(ofColor color);
		ofColor& getMatchingCharsColor();
	
		/// line number color, default: gray
		void setLineNumberColor(ofColor color);
		ofColor& getLineNumberColor();
	
		/// highlight matching open/close chars?, default: true
		void setHighlightMatchingChars(bool highlight=true);
		bool getHighlightMatchingChars();
	
		/// set the matching open/close chars to highlight,
		/// default: "([<{" & ")]>}", strings should not be empty
		/// note: matching chars are not highlighted inside comments
		void setMatchingChars(wstring openChars, wstring closeChars);
		void setMatchingChars(string openChars, string closeChars);
		wstring& getWideOpenChars();
		string getOpenChars();
		wstring& getWideCloseChars();
		string getCloseChars();
	
	protected:
	
		unsigned int tabWidth; //< tab width in spaces
		bool convertTabs; //< convert tabs to spaces?
		
		float alpha;                //< overall text alpha
		ofColor textColor;		    //< general text color, overridden by color scheme
		ofColor textShadowColor;	//< text offset shadow color
		ofColor cursorColor;	    //< text pos cursor color
		ofColor selectionColor;     //< char selection highlight color
		ofColor matchingCharsColor; //< matching chars highlight color
		ofColor lineNumberColor;    //< line number color

		bool highlightMatchingChars; //< highlight matching open/close chars?
		wstring openChars;  //< open chars (parens, brackets, etc) for matching highlight
		wstring closeChars; //< close chars (parens, bracket, etc) for matching highlight
};
