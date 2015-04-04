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
#include "ofxEditorSyntax.h"
#include "Unicode.h"
#include "ofLog.h"

//--------------------------------------------------------------
ofxEditorSyntax::ofxEditorSyntax() {
	clear();
}

//--------------------------------------------------------------
ofxEditorSyntax::ofxEditorSyntax(const ofxEditorSyntax &from) {
	copy(from);
}

//--------------------------------------------------------------
ofxEditorSyntax& ofxEditorSyntax::operator=(const ofxEditorSyntax &from) {
	copy(from);
	return *this;
}

//--------------------------------------------------------------
void ofxEditorSyntax::copy(const ofxEditorSyntax &from) {
	
	singleLineComment = from.singleLineComment;
	multiLineCommentBegin = from.multiLineCommentBegin;
	multiLineCommentEnd = from.multiLineCommentEnd;
	preprocessor = from.preprocessor;
	mathChars = from.mathChars;
	punctuationChars = from.punctuationChars;

	// make deep copies
	words.clear();
	for(map<wstring,WordType>::const_iterator iter = from.words.begin(); iter != from.words.end(); ++iter) {
		words[(*iter).first] = (*iter).second;
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::clear() {
	clearAllWords();
	singleLineComment = L"";
	multiLineCommentBegin = L"";
	multiLineCommentEnd = L"";
	preprocessor = L"#";
	mathChars = L"+-*/!|&~";
	punctuationChars = L";:,?";
}

// COMMENTS

//--------------------------------------------------------------
void ofxEditorSyntax::setSingleLineComment(const wstring &begin) {
	singleLineComment = begin;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setSingleLineComment(const string &begin) {
	singleLineComment = string_to_wstring(begin);
}

//--------------------------------------------------------------
wstring& ofxEditorSyntax::getWideSingleLineComment() {
	return singleLineComment;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getSingleLineComment() {
	return wstring_to_string(singleLineComment);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMultiLineComment(const wstring &begin, const wstring &end) {
	multiLineCommentBegin = begin;
	multiLineCommentEnd = end;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMultiLineComment(const string &begin, const string &end) {
	multiLineCommentBegin = string_to_wstring(begin);
	multiLineCommentEnd = string_to_wstring(end);
}

//--------------------------------------------------------------
wstring& ofxEditorSyntax::getWideMultiLineCommentBegin() {
	return multiLineCommentBegin;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getMultiLineCommentBegin() {
	return wstring_to_string(multiLineCommentBegin);
}

//--------------------------------------------------------------
wstring& ofxEditorSyntax::getWideMultiLineCommentEnd() {
	return multiLineCommentEnd;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getMultiLineCommentEnd() {
	return wstring_to_string(multiLineCommentEnd);
}

// PREPROCESSOR

//--------------------------------------------------------------
void ofxEditorSyntax::setPreprocessor(const wstring &begin) {
	preprocessor = begin;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPreprocessor(const string &begin) {
	preprocessor = string_to_wstring(begin);
}

//--------------------------------------------------------------
wstring& ofxEditorSyntax::getWidePreprocessor() {
	return preprocessor;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getPreprocessor() {
	return wstring_to_string(preprocessor);
}

// WORDS

//--------------------------------------------------------------
void ofxEditorSyntax::setWord(const wstring &word, WordType type) {
	words[word] = type;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWord(const string &word, WordType type) {
	setWord(string_to_wstring(word), type);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWords(const vector<wstring> &words, WordType type) {
	for(int i = 0; i < words.size(); ++i) {
		setWord(words[i], type);
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWords(const vector<string> &words, WordType type) {
	for(int i = 0; i < words.size(); ++i) {
		setWord(words[i], type);
	}
}

//--------------------------------------------------------------
ofxEditorSyntax::WordType ofxEditorSyntax::getWordType(const wstring &word) {
	map<wstring,WordType>::iterator iter = words.find(word);
	if(iter != words.end()) { // already exists
		return (*iter).second;
	}
	return PLAIN;
}

//--------------------------------------------------------------
ofxEditorSyntax::WordType ofxEditorSyntax::getWordType(const string &word) {
	getWordType(string_to_wstring(word));
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearWord(const wstring &word) {
	map<wstring,WordType>::iterator iter = words.find(word);
	if(iter != words.end()) { // already exists
		words.erase(iter);
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearWord(const string &word) {
	clearWord(word);
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearWordType(WordType type) {
	map<wstring,WordType>::iterator iter = words.begin();
	while(iter != words.end()) {
		if((*iter).second == type) {
			words.erase(++iter);
		}
		else {
			++iter;
		}
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearAllWords() {
	words.clear();
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMathChars(const wstring &chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSyntax") << "empty math string";
		return;
	}
	mathChars = chars;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMathChars(const string &chars) {
	setMathChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
wstring& ofxEditorSyntax::getWideMathChars() {
	return mathChars;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getMathChars() {
	return wstring_to_string(mathChars);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPunctuationChars(const wstring &chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSyntax") << "empty punctuation string";
		return;
	}
	punctuationChars = chars;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPunctuationChars(const string &chars) {
	setPunctuationChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
wstring& ofxEditorSyntax::getWidePunctuationChars() {
	return punctuationChars;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getPunctuationChars() {
	return wstring_to_string(punctuationChars);
}
