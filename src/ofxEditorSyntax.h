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

#include "ofConstants.h"
#include <set>
#include <map>

/// language-specific syntax words and characters
class ofxEditorSyntax {

	public:
	
		ofxEditorSyntax();
		ofxEditorSyntax(const std::string& xmlFile); //< load from an xml file
		ofxEditorSyntax(const ofxEditorSyntax &from);
		ofxEditorSyntax& operator=(const ofxEditorSyntax &from);
		void copy(const ofxEditorSyntax &from); //< copy data from another object
	
		/// load syntax from an xml file
		///
		/// root tag must be "syntax", available tags are: lang, files, text, string,
		/// singlecomment, multicomment, preprocessor, math, punctuation, words
		///
		/// short GLSL example:
		/// <syntax>
		///
		///   <!-- syntax language string (optional) -->
		///   <lang>GLSL</lang>
		///
		///   <!-- associated file extensions (minus .) -->
		///   <files>
		///     <ext>frag</ext>
		///     <ext>vert</ext>
		///   <files>
		///
		///   <!-- single line comment begin -->
		///	  <singlecomment>//</singlecomment>
		///
		///   <!-- multi line comment begin and end -->
		///   <multicomment>
		///     <begin>/*</begin>
		///     <end>*/</end>
		///   </multicomment>
		///
		///   <!-- multi line string literal begin and end -->
		///   <stringliteral>
		///     <begin>R"</begin>
		///     <end>"</end>
		///   </stringliteral>
		///
		///   <!-- preprocessor begin -->
		///   <preprocessor>#</preprocessor>
		///
		///   <!--
		///        mathematical characters,
		///        note "&amp;" since "&" needs to be escaped
		///   -->
		///   <math>+-*/!|&amp;~</math>
		///
		///   <!-- punctuation characters -->
		///   <punctuation>;:,?</punctuation>
		///
		///   <!-- parse 0x123 as a number? either "true" or "false" -->
		///   <hexliteral>true</hexliteral>
		///
		///   <!-- syntax type for specific words -->
		///   <words>
		///     <keyword>void</keyword>
		///     <typename>int</typename>
		///     <function>gl_FragCoord</function>
		///   </words>
		///
		/// </syntax>
		///
		/// returns false on read or parse error
		bool loadFile(const std::string& xmlFile);
	
		/// clear everything to defaults
		void clear();
	
	/// \section Meta
	
		/// set the language string for this syntax aka "GLSL", "Lua", etc
		void setLang(const std::string &lang);
	
		/// get language string, returns "" if not set
		const std::string& getLang();
	
		/// clear language string to ""
		void clearLang();
	
		/// add file extension associated to this syntax
		/// note: file extensions do not include the period, ex: "lua" not ".lua"
		void addFileExt(const std::string &ext);
		void addFileExt(const std::vector<std::string> &exts);
	
		/// is a given file extension associated with this syntax
		bool hasFileExt(const std::string &ext);
	
		/// get file extensions
		const std::set<std::string>& getFileExts();
	
		/// clear language string for a file extension
		void clearFileExt(const std::string &ext);
	
		/// clear all file extension language strings
		void clearAllFileExts();

	/// \section Comments

		/// set the beginning string to match for a single line comment,
		/// blank by default
		void setSingleLineComment(const std::u32string &begin);
		void setSingleLineComment(const std::string &begin);
		const std::u32string& getWideSingleLineComment();
		std::string getSingleLineComment();
	
		/// set the beginning and ending strings to match for a multi line comment,
		/// blank by default
		void setMultiLineComment(const std::u32string &begin, const std::u32string &end);
		void setMultiLineComment(const std::string &begin, const std::string &end);
		const std::u32string& getWideMultiLineCommentBegin();
		std::string getMultiLineCommentBegin();
		const std::u32string& getWideMultiLineCommentEnd();
		std::string getMultiLineCommentEnd();
	
	/// \section String Literals
	
		/// set the beginning and ending strings to match for a multi line string literal,
		/// blank by default
		void setStringLiteral(const std::u32string &begin, const std::u32string &end);
		void setStringLiteral(const std::string &begin, const std::string &end);
		const std::u32string& getWideStringLiteralBegin();
		std::string getStringLiteralBegin();
		const std::u32string& getWideStringLiteralEnd();
		std::string getStringLiteralEnd();
	
	/// \section Preprocessor
	
		/// preprocessor string which starts a preprocessor block until an endline
		/// blank by default
		void setPreprocessor(const std::u32string &begin);
		void setPreprocessor(const std::string &begin);
		const std::u32string& getWidePreprocessor();
		std::string getPreprocessor();
	
	/// \section Words
	
		enum WordType {
			PLAIN,	  //< plain text
			KEYWORD,  //< keywords (aka class, void, public, etc)
			TYPENAME, //< typenames (aka int, float, etc)
			FUNCTION  //< function names (aka sin(), abs(), etc)
		};
	
		/// set type for a given word
		void setWord(const std::u32string &word, WordType type);
		void setWord(const std::string &word, WordType type);
	
		/// set type for a vector of words
		void setWords(const std::vector<std::u32string> &words, WordType type);
		void setWords(const std::vector<std::string> &words, WordType type);
	
		/// get type for word, returns PLAIN if not found
		WordType getWordType(const std::u32string &word);
		WordType getWordType(const std::string &word);
	
		/// clear type for word
		void clearWord(const std::u32string &word);
		void clearWord(const std::string &word);
	
		/// clear all words of a given type
		void clearWordType(WordType type);
	
		/// clear all words (keyword, typename, function)
		void clearAllWords();
	
	/// \section Parsing Chars
	
		/// parse "0x123" as a number?
		/// default: true, not all languages support this
		void setHexLiteral(bool hex);
		bool getHexLiteral();
	
		/// common math & comparison operator chars between highlighted text blocks when parsing
		/// default: "=+-*/!|&^~", string should not be empty
		void setOperatorChars(const std::u32string &chars);
		void setOperatorChars(const std::string &chars);
		const std::u32string& getWideOperatorChars();
		std::string getOperatorChars();
	
		/// common punctuation chars between highlighted text blocks when parsing
		/// default: ";:,?", string should not be empty
		void setPunctuationChars(const std::u32string &chars);
		void setPunctuationChars(const std::string &chars);
		const std::u32string& getWidePunctuationChars();
		std::string getPunctuationChars();
	
	protected:
	
		std::string lang; //< langauge string aka "GLSL", "Lua", etc
		std::set<std::string> fileExts; //< associated file extensions (minus .)
	
		std::u32string singleLineComment; //< single line comment begin
		std::u32string multiLineCommentBegin; //< multi line comment begin
		std::u32string multiLineCommentEnd; //< multi line comment end
	
		std::u32string stringLiteralBegin; //< multi line string literal begin
		std::u32string stringLiteralEnd; //< multi line string literal end
	
		std::u32string preprocessor; //< preprocessor begin
		std::map<std::u32string,WordType> words; //< synatx types for specific words
		bool hexLiteral; //< parse hex literals (0x123) as numbers?
		std::u32string operatorChars; //< common operator chars
		std::u32string punctuationChars; //< punctuation chars
};
