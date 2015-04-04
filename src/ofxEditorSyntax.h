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

#include "ofConstants.h"
#include <map>

/// language-specific syntax words and characters
class ofxEditorSyntax {

	public:
	
		ofxEditorSyntax();
		ofxEditorSyntax(const ofxEditorSyntax &from);
		ofxEditorSyntax& operator=(const ofxEditorSyntax &from);
		void copy(const ofxEditorSyntax &from); //< copy data from another object
	
		/// clear everything to defaults
		void clear();

	/// \section Comments

		/// set the beginning string to match for a single line comment,
		/// blank by default
		void setSingleLineComment(const wstring &begin);
		void setSingleLineComment(const string &begin);
		wstring& getWideSingleLineComment();
		string getSingleLineComment();
	
		/// set the beginning and ending strings to match for a multi line comment,
		/// blank by default
		void setMultiLineComment(const wstring &begin, const wstring &end);
		void setMultiLineComment(const string &begin, const string &end);
		wstring& getWideMultiLineCommentBegin();
		string getMultiLineCommentBegin();
		wstring& getWideMultiLineCommentEnd();
		string getMultiLineCommentEnd();
	
	/// \section Preprocessor
	
		/// preprocessor string which starts a preprocessor block until an endline
		/// blank by default
		void setPreprocessor(const wstring &begin);
		void setPreprocessor(const string &begin);
		wstring& getWidePreprocessor();
		string getPreprocessor();
	
	/// \section Words
	
		enum WordType {
			PLAIN,	  //< plain text
			KEYWORD,  //< keywords (aka class, void, public, etc)
			TYPENAME, //< typenames (aka int, float, etc)
			FUNCTION  //< function names (aka sin(), abs(), etc)
		};
	
		/// set word type directly
		void setWord(const wstring &word, WordType type);
		void setWord(const string &word, WordType type);
	
		/// set
		void setWords(const vector<wstring> &words, WordType type);
		void setWords(const vector<string> &words, WordType type);
	
		/// get type for word, returns PLAIN if not found
		WordType getWordType(const wstring &word);
		WordType getWordType(const string &word);
	
		/// clear type for word
		void clearWord(const wstring &word);
		void clearWord(const string &word);
	
		/// clear all words of a given type
		void clearWordType(WordType type);
	
		/// clear all words (keyword, typename, function)
		void clearAllWords();
	
	/// \section Parsing Chars
	
		/// common mathematical chars between highlighted text blocks when parsing
		/// default: "+-*/!|&~", string should not be empty
		void setMathChars(const wstring &chars);
		void setMathChars(const string &chars);
		wstring& getWideMathChars();
		string getMathChars();
	
		/// common punctuation chars between highlighted text blocks when parsing
		/// default: ";:,?", string should not be empty
		void setPunctuationChars(const wstring &chars);
		void setPunctuationChars(const string &chars);
		wstring& getWidePunctuationChars();
		string getPunctuationChars();
	
	protected:
	
		wstring singleLineComment;
		wstring multiLineCommentBegin;
		wstring multiLineCommentEnd;
	
		wstring preprocessor;
		map<wstring,WordType> words;
		wstring mathChars;
		wstring punctuationChars;
};
