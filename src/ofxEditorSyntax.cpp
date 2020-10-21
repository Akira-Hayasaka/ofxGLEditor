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
ofxEditorSyntax::ofxEditorSyntax(const std::string& xmlFile) {
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
	for(std::map<std::u32string,WordType>::const_iterator iter = from.words.begin(); iter != from.words.end(); ++iter) {
		words[(*iter).first] = (*iter).second;
	}
	for(std::set<std::string>::const_iterator iter = from.fileExts.begin(); iter != from.fileExts.end(); ++iter) {
		fileExts.insert((*iter));
	}
}

//--------------------------------------------------------------
bool ofxEditorSyntax::loadFile(const std::string& xmlFile) {
	std::string path = ofToDataPath(xmlFile);
	ofXml xml;
	if(!xml.load(path)) {
		ofLogError("ofxEditorSyntax") << "couldn't load \""
			<< ofFilePath::getFileName(xmlFile) << "\"";
		return false;
	}
	auto root = xml.getChild("syntax");
	if(!root) {
		ofLogWarning("ofxEditorSyntax") << "root xml tag not \"syntax\", ignoring";
		return false;
	}
	clear();
	for(auto & child : root.getChildren()) {
		if(child.getName() == "lang") {setLang(child.getValue());}
		else if(child.getName() == "files") {
			for(auto & file : child.getChildren()) {
				if(file.getName() == "ext") {addFileExt(file.getValue());}
				else {
					ofLogWarning("ofxEditorSyntax") << "ignoring unknown files xml tag \"" << file.getName() << "\"";
				}
			}
		}
		else if(child.getName() == "singlecomment") {singleLineComment = string_to_wstring(child.getValue());}
		else if(child.getName() == "multicomment")  {
			auto begin = child.getChild("begin");
			auto end = child.getChild("end");
			if(begin) {multiLineCommentBegin = string_to_wstring(begin.getValue());}
			if(end) {multiLineCommentBegin = string_to_wstring(end.getValue());}
		}
		else if(child.getName() == "stringliteral")  {
			auto begin = child.getChild("begin");
			auto end = child.getChild("end");
			if(begin) {stringLiteralBegin = string_to_wstring(begin.getValue());}
			if(end) {stringLiteralEnd = string_to_wstring(end.getValue());}
		}
		else if(child.getName() == "preprocessor") {preprocessor = string_to_wstring(child.getValue());}
		else if(child.getName() == "hexliteral") {
			std::string b = child.getValue();
			if(b == "true")       {setHexLiteral(true);}
			else if(b == "false") {setHexLiteral(false);}
			else {
				ofLogWarning("ofxEditorSyntax") << "ignoring unknown xml bool string \"" << b << "\"";
			}
		}
		else if(child.getName() == "operator")     {operatorChars = string_to_wstring(child.getValue());}
		else if(child.getName() == "punctuation")  {punctuationChars = string_to_wstring(child.getValue());}
		else if(child.getName() == "words")  {
			for(auto &word : child.getChildren()) {
				if(word.getName() == "keyword")       {setWord(word.getValue(), KEYWORD);}
				else if(word.getName() == "typename") {setWord(word.getValue(), TYPENAME);}
				else if(word.getName() == "function") {setWord(word.getValue(), FUNCTION);}
				else {
					ofLogWarning("ofxEditorSyntax") << "ignoring unknown words xml tag \"" << word.getName() << "\"";
				}
			}
		}
		else {
			ofLogWarning("ofxEditorSyntax") << "ignoring unknown xml tag \"" << child.getName() << "\"";
		}
	}
	
	return true;
}

//--------------------------------------------------------------
void ofxEditorSyntax::clear() {
	lang = "";
	clearAllFileExts();
	singleLineComment = U"";
	multiLineCommentBegin = U"";
	multiLineCommentEnd = U"";
	stringLiteralBegin = U"";
	stringLiteralEnd = U"";
	preprocessor = U"#";
	hexLiteral = true;
	operatorChars = U"=+-*/!|&~^";
	punctuationChars = U";:,?";
	clearAllWords();
}

// META

//--------------------------------------------------------------
void ofxEditorSyntax::setLang(const std::string &lang) {
	this->lang = lang;
}

//--------------------------------------------------------------
const std::string& ofxEditorSyntax::getLang() {
	return lang;
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearLang() {
	lang = "";
}

//--------------------------------------------------------------
void ofxEditorSyntax::addFileExt(const std::string &ext) {
	if(ext == "") return;
	if(ext[0] == '.') { // be helpful here
		ofLogWarning("ofxEditorSyntax") << "dropping initial . from file extension: \"" << ext << "\"";
		fileExts.insert(ext.substr(1, ext.size()-1));
	}
	else {
		fileExts.insert(std::string(ext)); // make deep copy to avoid possible BAD_ACCESS
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::addFileExt(const std::vector<std::string> &exts) {
	for(int i = 0; i < exts.size(); ++i) {
		addFileExt(exts[i]);
	}
}

//--------------------------------------------------------------
bool ofxEditorSyntax::hasFileExt(const std::string &ext) {
	return (fileExts.find(ext) != fileExts.end());
}

//--------------------------------------------------------------
const std::set<std::string>& ofxEditorSyntax::getFileExts() {
	return fileExts;
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearFileExt(const std::string &ext) {
	fileExts.erase(ext);
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearAllFileExts() {
	fileExts.clear();
}

// COMMENTS

//--------------------------------------------------------------
void ofxEditorSyntax::setSingleLineComment(const std::u32string &begin) {
	singleLineComment = begin;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setSingleLineComment(const std::string &begin) {
	singleLineComment = string_to_wstring(begin);
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWideSingleLineComment() {
	return singleLineComment;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getSingleLineComment() {
	return wstring_to_string(singleLineComment);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMultiLineComment(const std::u32string &begin, const std::u32string &end) {
	multiLineCommentBegin = begin;
	multiLineCommentEnd = end;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setMultiLineComment(const std::string &begin, const std::string &end) {
	multiLineCommentBegin = string_to_wstring(begin);
	multiLineCommentEnd = string_to_wstring(end);
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWideMultiLineCommentBegin() {
	return multiLineCommentBegin;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getMultiLineCommentBegin() {
	return wstring_to_string(multiLineCommentBegin);
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWideMultiLineCommentEnd() {
	return multiLineCommentEnd;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getMultiLineCommentEnd() {
	return wstring_to_string(multiLineCommentEnd);
}




//--------------------------------------------------------------
void ofxEditorSyntax::setStringLiteral(const std::u32string &begin, const std::u32string &end) {
	stringLiteralBegin = begin;
	stringLiteralEnd = end;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setStringLiteral(const std::string &begin, const std::string &end) {
	stringLiteralBegin = string_to_wstring(begin);
	stringLiteralEnd = string_to_wstring(end);
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWideStringLiteralBegin() {
	return stringLiteralBegin;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getStringLiteralBegin() {
	return wstring_to_string(stringLiteralBegin);
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWideStringLiteralEnd() {
	return stringLiteralEnd;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getStringLiteralEnd() {
	return wstring_to_string(stringLiteralEnd);
}




// PREPROCESSOR

//--------------------------------------------------------------
void ofxEditorSyntax::setPreprocessor(const std::u32string &begin) {
	preprocessor = begin;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPreprocessor(const std::string &begin) {
	preprocessor = string_to_wstring(begin);
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWidePreprocessor() {
	return preprocessor;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getPreprocessor() {
	return wstring_to_string(preprocessor);
}

// WORDS

//--------------------------------------------------------------
void ofxEditorSyntax::setWord(const std::u32string &word, WordType type) {
	if(word == U"") return;
	words[word] = type;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWord(const std::string &word, WordType type) {
	setWord(string_to_wstring(word), type);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWords(const std::vector<std::u32string> &words, WordType type) {
	for(int i = 0; i < words.size(); ++i) {
		setWord(words[i], type);
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::setWords(const std::vector<std::string> &words, WordType type) {
	for(int i = 0; i < words.size(); ++i) {
		setWord(words[i], type);
	}
}

//--------------------------------------------------------------
ofxEditorSyntax::WordType ofxEditorSyntax::getWordType(const std::u32string &word) {
	std::map<std::u32string,WordType>::iterator iter = words.find(word);
	if(iter != words.end()) { // already exists
		return (*iter).second;
	}
	return PLAIN;
}

//--------------------------------------------------------------
ofxEditorSyntax::WordType ofxEditorSyntax::getWordType(const std::string &word) {
	return getWordType(string_to_wstring(word));
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearWord(const std::u32string &word) {
	std::map<std::u32string,WordType>::iterator iter = words.find(word);
	if(iter != words.end()) { // already exists
		words.erase(iter);
	}
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearWord(const std::string &word) {
	clearWord(word);
}

//--------------------------------------------------------------
void ofxEditorSyntax::clearWordType(WordType type) {
	std::map<std::u32string,WordType>::iterator iter = words.begin();
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
void ofxEditorSyntax::setOperatorChars(const std::u32string &chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSyntax") << "empty operator string";
		return;
	}
	operatorChars = chars;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setOperatorChars(const std::string &chars) {
	setOperatorChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWideOperatorChars() {
	return operatorChars;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getOperatorChars() {
	return wstring_to_string(operatorChars);
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPunctuationChars(const std::u32string &chars) {
	if(chars.length() == 0) {
		ofLogWarning("ofxEditorSyntax") << "empty punctuation string";
		return;
	}
	punctuationChars = chars;
}

//--------------------------------------------------------------
void ofxEditorSyntax::setPunctuationChars(const std::string &chars) {
	setPunctuationChars(string_to_wstring(chars));
}

//--------------------------------------------------------------
const std::u32string& ofxEditorSyntax::getWidePunctuationChars() {
	return punctuationChars;
}

//--------------------------------------------------------------
std::string ofxEditorSyntax::getPunctuationChars() {
	return wstring_to_string(punctuationChars);
}
