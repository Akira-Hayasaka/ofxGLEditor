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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
#include "ofXml.h"
#include <map>

/// syntax highlighter color scheme
class ofxEditorColorScheme {

	public:
	
		ofxEditorColorScheme();
		ofxEditorColorScheme(const std::string& xmlFile); //< load from an xml file
	
		/// load colors from an xml file
		///
		/// root tag must be "colorscheme", available tags are: text, string,
		/// number, comment, preprocessor, keyword, typename, function
		///
		/// colors can be set using rgb(a):
		/// <text>
		///   <r>255</r>
		///   <g>0</g>
	    ///   <b>255</b>
		///   <a>255</a>
		/// </text>
		///
		/// grayscale:
		/// <text>
		///   <gray>200</gray>
		/// </text>
		///
		/// or hex:
		/// <text>
		///   <hex>FF00FF</hex>
		/// </text>
		///
		/// alpha (a) can be set for all methods (rgb, grayscale, & hex)
		///
		/// short example:
		/// <colorscheme>
		///   <text>
		///     <gray>200></gray>
		///   </text>
		///   <string>
		///     <r>255</r>
		///     <g>255></g>
		///     <b>0></b>
		///   </string>
		///   <number>
		///     <hex>080200</hex>
		///   </number>
		/// </colorcheme>
		///
		/// returns false on read or parse error
		bool loadFile(const std::string& xmlFile);
	
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
	
		/// set color values from current xml element,
		/// available tags (all int values): r, g, b, a, gray, hex
		void setColorFromXml(ofXml &xml, ofColor &color);
	
		ofColor textColor;
		ofColor stringColor;
		ofColor numberColor;
		ofColor commentColor;
		ofColor preprocessorColor;
		ofColor keywordColor;
		ofColor typenameColor;
		ofColor functionColor;
};
