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
#include <map>

/// syntax highlighter color scheme
class ofxEditorColorScheme {

	public:
	
		ofxEditorColorScheme();
		
		/// clear all colors to white
		void clear();
	
		/// clear all colors to a given color
		void clear(ofColor color);
	
	/// \section General
	
		/// default plain text color
		void setTextColor(ofColor color);
		ofColor& getTextColor();
		
		/// string coloring
		void setStringColor(ofColor color);
		ofColor& getStringColor();
		
		/// number coloring
		void setNumberColor(ofColor color);
		ofColor& getNumberColor();
	
	/// \section Language Syntax Specific
	
		/// single & multi line comment coloring
		void setCommentColor(ofColor color);
		ofColor& getCommentColor();
	
		/// preprocessor coloring (aka # to endline)
		void setPreprocessorColor(ofColor color);
		ofColor& getPreprocessorColor();
	
		/// keyword coloring (aka class, void, public, etc)
		void setKeywordColor(ofColor color);
		ofColor& getKeywordColor();
	
		/// typename coloring (aka int, float, etc)
		void setTypenameColor(ofColor color);
		ofColor& getTypenameColor();
	
		/// function name coloring (aka sin(), abs(), etc)
		void setFunctionColor(ofColor color);
		ofColor& getFunctionColor();
	
	protected:
	
		ofColor textColor;
		ofColor stringColor;
		ofColor numberColor;
		ofColor commentColor;
		ofColor preprocessorColor;
		ofColor keywordColor;
		ofColor typenameColor;
		ofColor functionColor;
};
