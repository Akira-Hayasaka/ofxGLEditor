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
#include "ofxEditorSyntax.h"
#include "Unicode.h"
#include "ofLog.h"
#include "ofXml.h"

//--------------------------------------------------------------
ofxEditorSyntax::ofxEditorSyntax() {
	clear();
}

//--------------------------------------------------------------
ofxEditorSyntax::ofxEditorSyntax(const string& xmlFile) {
	if(!loadFile(xmlFile)) {
		clear();
	}
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
	
	lang = from.lang;
	singleLineComment = from.singleLineComment;
	multiLineCommentBegin = from.multiLineCommentBegin;
	multiLineCommentEnd = from.multiLineCommentEnd;
	preprocessor = from.preprocessor;
	hexLiteral = from.hexLiteral;
	operatorChars = from.operatorChars;
	punctuationChars = from.punctuationChars;

	// make deep copies
	clearAllFileExts();
	clearAllWords();
	for(map<u32string,WordType>::const_iterator iter = from.words.begin(); iter != from.words.end(); ++iter) {
		words[(*iter).first] = (*iter).second;
	}
	for(set<string>::const_iterator iter = from.fileExts.begin(); iter != from.fileExts.end(); ++iter) {
		fileExts.insert((*iter));
	}
}

//--------------------------------------------------------------
bool ofxEditorSyntax::loadFile(const string& xmlFile) {
	string path = ofToDataPath(xmlFile);
	ofXml xml;
	if(!xml.load(path)) {
		ofLogError("ofxEditorSyntax") << "couldn't load \""
			<< ofFilePath::getFileName(xmlFile) << "\"";
		return false;
	}
	xml.setToParent();
	if(!xml.exists("syntax")) {
		ofLogWarning("ofxEditorSyntax") << "root xml tag not \"syntax\", ignoring";
		return false;
	}
	xml.setTo("syntax");
	int numTags = xml.getNumChildren();
	clear();
	for(int i = 0; i < numTags; ++i) {
		xml.setToChild(i);
		if(xml.getName() == "lang") {setLang(xml.getValue());}
		else if(xml.getName() == "files") {
			int numExts = xml.getNumChildren();
			for(int e = 0; e < numExts; ++e) {
				xml.setToChild(e);
				if(xml.getName() == "ext") {addFileExt(xml.getValue());}
				else {
					ofLogWarning("ofxEditorSyntax") << "ignoring unknown files xml tag \"" << xml.getName() << "\"";
				}
				xml.setToParent();
			}
		}
		else if(xml.getName() == "singlecomment") {singleLineComment = string_to_wstring(xml.getValue());}
		else if(xml.getName() == "multicomment")  {
			if(xml.exists("begin")) {multiLineCommentBegin = string_to_wstring(xml.getValue("begin"));}
			if(xml.exists("end")) {multiLineCommentBegin = string_to_wstring(xml.getValue("end"));}
		}
		else if(xml.getName() == "preprocessor") {preprocessor = string_to_wstring(xml.getValue());}
		else if(xml.getName() == "hexliteral") {
			string b = xml.getValue();
			if(b == "true")       {setHexLiteral(true);}
			else if(b == "false") {setHexLiteral(false);}
			else {
				ofLogWarning("ofxEditorSyntax") << "ignoring unknown xml bool string \"" << b << "\"";
			}
		}
		else if(xml.getName() == "operator")     {operatorChars = string_to_wstring(xml.getValue());}
		else if(xml.getName() == "punctuation")  {punctuationChars = string_to_wstring(xml.getValue());}
		else if(xml.getName() == "words")  {
			int numWords = xml.getNumChildren();
			for(int w = 0; w < numWords; ++w) {
				xml.setToChild(w);
				if(xml.getName() == "keyword")       {setWord(xml.getValue(), KEYWORD);}
				else if(xml.getName() == "typename") {setWord(xml.getValue(), TYPENAME);}
				else if(xml.getName() == "function") {setWord(xml.getValue(), FUNCTION);}
				else {
					ofLogWarning("ofxEditorSyntax") << "ignoring unknown words xml tag \"" << xml.getName() << "\"";
				}
				xml.setToParent();
			}
		}
		else {
			ofLogWarning("ofxEditorSyntax") << "ignoring unknown xml tag \"" << xml.getName() << "\"";
		}
		xml.setToParent();
	}
	xml.clear();
	
	return true;
}

//--------------------------------------------------------------
void ofxEditorSyntax::clear() {
	lang = "";
	clearAllFileExts();
	singleLineComment = U"";
	multiLineCommentBegin = U"";
	multiLineCommentEnd = U"";
	preprocessor = U"#";
	hexLiteral = true;
	operatorChars = U"=+-*/!|&~^";
	punctuationChars = U";:,?";
	clearAllWords();
}

// META

//--------------------------------------------------------------
void ofxEditorSyntax::setLang(const string &lang) {
	this->lang = lang;
}

//--------------------------------------------------------------
const string& ofxEditorSyntax::getLang() {
	return lang;
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearLang() {
	lang = "";
}

//--------------------------------------------------------------
void ofxEditorSyntax::addFileExt(const string &ext) {
	if(ext == "") return;
	if(ext[0] == '.') { // be helpful here
		ofLogWarning("ofxEditorSyntax") << "dropping initial . from file extension: \"" << ext << "\"";
		fileExts.insert(ext.substr(1, ext.size()-1));
	}
	else {
		fileExts.insert(string(ext)); // make deep copy to avoid possible BAD_ACCESS
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::addFileExt(const vector<string> &exts) {
	for(int i = 0; i < exts.size(); ++i) {
		addFileExt(exts[i]);
	}
}

//--------------------------------------------------------------
bool ofxEditorSyntax::hasFileExt(const string &ext) {
	return (fileExts.find(ext) != fileExts.end());
}

//--------------------------------------------------------------
const set<string>& ofxEditorSyntax::getFileExts() {
	return fileExts;
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearFileExt(const string &ext) {
	fileExts.erase(ext);
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearAllFileExts() {
	fileExts.clear();
}

// COMMENTS

//--------------------------------------------------------------
void ofxEditorSyntax::setSingleLineComment(const u32string &begin) {
	singleLineComment = begin;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setSingleLineComment(const string &begin) {
	singleLineComment = string_to_wstring(begin);
}

//--------------------------------------------------------------
const u32string& ofxEditorSyntax::getWideSingleLineComment() {
	return singleLineComment;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getSingleLineComment() {
	return wstring_to_string(singleLineComment);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMultiLineComment(const u32string &begin, const u32string &end) {
	multiLineCommentBegin = begin;
	multiLineCommentEnd = end;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMultiLineComment(const string &begin, const string &end) {
	multiLineCommentBegin = string_to_wstring(begin);
	multiLineCommentEnd = string_to_wstring(end);
}

//--------------------------------------------------------------
const u32string& ofxEditorSyntax::getWideMultiLineCommentBegin() {
	return multiLineCommentBegin;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getMultiLineCommentBegin() {
	return wstring_to_string(multiLineCommentBegin);
}

//--------------------------------------------------------------
const u32string& ofxEditorSyntax::getWideMultiLineCommentEnd() {
	return multiLineCommentEnd;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getMultiLineCommentEnd() {
	return wstring_to_string(multiLineCommentEnd);
}

// PREPROCESSOR

//--------------------------------------------------------------
void ofxEditorSyntax::setPreprocessor(const u32string &begin) {
	preprocessor = begin;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPreprocessor(const string &begin) {
	preprocessor = string_to_wstring(begin);
}

//--------------------------------------------------------------
const u32string& ofxEditorSyntax::getWidePreprocessor() {
	return preprocessor;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getPreprocessor() {
	return wstring_to_string(preprocessor);
}

// WORDS

//--------------------------------------------------------------
void ofxEditorSyntax::setWord(const u32string &word, WordType type) {
	if(word == U"") return;
	words[word] = type;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWord(const string &word, WordType type) {
	setWord(string_to_wstring(word), type);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWords(const vector<u32string> &words, WordType type) {
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
ofxEditorSyntax::WordType ofxEditorSyntax::getWordType(const u32string &word) {
	map<u32string,WordType>::iterator iter = words.find(word);
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
void ofxEditorSyntax::clearWord(const u32string &word) {
	map<u32string,WordType>::iterator iter = words.find(word);
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
	map<u32string,WordType>::iterator iter = words.begin();
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

// PARSING CHARS

//--------------------------------------------------------------
void ofxEditorSyntax::setHexLiteral(bool hex) {
	hexLiteral = hex;
}

//--------------------------------------------------------------
bool ofxEditorSyntax::getHexLiteral() {
	return hexLiteral;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setOperatorChars(const u32string &chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSyntax") << "empty operator string";
		return;
	}
	operatorChars = chars;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setOperatorChars(const string &chars) {
	setOperatorChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
const u32string& ofxEditorSyntax::getWideOperatorChars() {
	return operatorChars;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getOperatorChars() {
	return wstring_to_string(operatorChars);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPunctuationChars(const u32string &chars) {
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
const u32string& ofxEditorSyntax::getWidePunctuationChars() {
	return punctuationChars;
}

//--------------------------------------------------------------
string ofxEditorSyntax::getPunctuationChars() {
	return wstring_to_string(punctuationChars);
}
