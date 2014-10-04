#include "ofxEditor.h"

ofPtr<ofxEditor::Font> ofxEditor::s_font;
int ofxEditor::s_charWidth = 1;
int ofxEditor::s_charHeight = 1;
unsigned int ofxEditor::s_tabWidth = 4;
bool ofxEditor::s_convertTabs = false;

ofxEditor::ofxEditor() {
	cursorPos = 0;
	desiredXPos = 0;
	viewport.position.set(0, 0, 0);
	setSize(ofGetWidth(), ofGetHeight());
	
	colorScheme = NULL;
	lineWrapping = false;
}

ofxEditor::~ofxEditor() {
}

bool ofxEditor::loadFont(const string &font, int size) {
	if(s_font == NULL) {
		s_font = ofPtr<ofxEditor::Font>(new ofxEditor::Font());
	}
	if(s_font->loadFont(font, size, true, true, true)) {
		s_charWidth = size;
		s_charHeight = s_font->getLineHeight();
	}
}

void ofxEditor::setTabWidth(unsigned int numSpaces) {
	s_tabWidth = numSpaces < 1 ? 1 : numSpaces;
}

unsigned int ofxEditor::getTabWidth() {
	return s_tabWidth;
}

void ofxEditor::setConvertTabsToSpaces(bool convert) {
	s_convertTabs = convert;
}

bool ofxEditor::getConvertTabsToSpaces() {
	return s_convertTabs;
}

void ofxEditor::draw() {
	ofPushStyle();
	ofPushView();
	
		ofViewport(viewport);

		// draw text
		if(colorScheme) { // with colorScheme
			ofFill();
			int x = 0, y = s_charHeight; // pixel pos
			int textPos = 0;//, textLine = 0; // char/line pos
			for(list<TextBlock>::iterator iter = textList.begin(); iter != textList.end(); iter++) {
			
				TextBlock &tb = (*iter);
				
				// line wrap
				//if(lineWrapping && textPos+tb.text.length() >= numCharsWidth) {
				if(lineWrapping && textPos >= numCharsWidth) {
					x = 0;
					y += s_charHeight;
				}
				
				switch(tb.type) {
				
					case UNKNOWN:
						ofLogWarning("ofxEditor") << "trying to draw UNKNOWN text block";
						return;
						
					case WORD:
						ofSetColor(colorScheme->getWordColor(tb.text));
						break;
						
					case STRING:
						ofSetColor(colorScheme->getStringColor());
						break;
						
					case NUMBER:
						ofSetColor(colorScheme->getNumberColor());
						break;
					
					default:
						break;
				}
				
				// draw block chars
				for(int i = 0; i < tb.text.length(); ++i) {
					
					// draw cursor
					if(textPos == cursorPos) {
						ofPushStyle();
							ofSetColor(255*0.7, 255*0.7, 0);
							ofRect(x, y-s_charHeight, floor((s_charWidth*0.25)), s_charHeight);
						ofPopStyle();
					}
					
					// draw chars
					switch(tb.type) {
						case ENDLINE:
							x = 0;
							y += s_charHeight;
							textPos++;
							break;
						case TAB:
							x += s_charWidth * s_tabWidth;
							textPos++;
							break;
						default:
							s_font->drawCharacter(tb.text[i], x, y);
							x += s_charWidth;
							textPos++;
							break;
					}
				}
			}
		}
		else { // without highlighter
			ofFill();
			int x = 0, y = s_charHeight; // pixel pos
			int textPos = 0, textLine = 0; // char/line pos
			for(int i = 0; i < text.length(); ++i) {
			
				// line wrap
				if(lineWrapping && textPos >= numCharsWidth) {
					x = 0;
					y += s_charHeight;
					textPos = 0;
					textLine++;
				}
				
				// draw cursor
				if(i == cursorPos) {
					ofSetColor(255*0.7, 255*0.7, 0);
					ofRect(x, y-s_charHeight, floor(s_charWidth*0.25), s_charHeight);
				}
			
				// endline
				if(text[i] == '\n') {
					x = 0;
					y += s_charHeight;
					textPos = 0;
					textLine++;
				}
				// tab
				else if(text[i] == '\t') {
					x += s_charWidth * s_tabWidth;
					textPos += s_tabWidth;
				}
				// everything else
				else {
					ofSetColor(255);
					s_font->drawCharacter(text[i], x, y);
					x += s_charWidth;
					textPos++;
				}
			}
		}
			
//		// grid
//		ofSetColor(100, 100, 100);
//		for(int x = 0; x < ofGetWidth(); x = x+s_charWidth) {
//			ofLine(x, 0, x, ofGetHeight());
//		}
//		for(int y = s_charHeight; y < ofGetHeight(); y = y+s_charHeight) {
//			ofLine(0, y, ofGetWidth(), y);
//		}
		
	ofPopView();
	ofPopStyle();
}

void ofxEditor::keyPressed(int key) {
	switch(key) {
		case OF_KEY_UP:
			if((int)lineStart(cursorPos) > 0) { // if we're not on the first line
				unsigned int previouslinelength = previousLineLength(cursorPos);
				if(previouslinelength < desiredXPos) {
					cursorPos = lineStart(cursorPos)-1; // end of previous
				}
				else {
					cursorPos = lineStart(lineStart(cursorPos)-1) + desiredXPos; // start of previous+offset
				}
				//if(cursorPos < m_TopTextPosition) m_TopTextPosition=LineStart(cursorPos);
			}
			break;
		case OF_KEY_DOWN:
			if(lineEnd(cursorPos) < text.size()) { // if we're not on the last line
				unsigned int nextlinelength = nextLineLength(cursorPos);
				if(nextlinelength < desiredXPos) {
					cursorPos = lineEnd(lineEnd(cursorPos)+1); // end of next
				}
				else {
					cursorPos = lineStart(lineEnd(cursorPos)+1) + desiredXPos; // start of next+offset
				}
				//if (m_Position>=m_BottomTextPosition) m_TopTextPosition=LineEnd(m_TopTextPosition)+1;
			}
			break;
		case OF_KEY_LEFT:
			if(cursorPos > 0) {
				cursorPos--;
			}
			desiredXPos = offsetToCurrentLineStart();
			break;
		case OF_KEY_RIGHT:
			if(!text.empty()) {
				cursorPos++;
			}
			desiredXPos = offsetToCurrentLineStart();
			break;
	}
}

void ofxEditor::setSize(int width, int height) {
	viewport.width = width;
	viewport.height = height;
	
	numCharsWidth = width/s_charWidth;
	numLinesHeight = height/s_charHeight;
	
	ofLogVerbose("ofxEditor") << "pixel size: " << width << " " << height;
	ofLogVerbose("ofxEditor") << "text size: " << numCharsWidth << " " << numLinesHeight;
}

string ofxEditor::getText() {
	return text;
}

void ofxEditor::setText(const string& text) {
	this->text = text;
	if(s_convertTabs) {
		processTabs();
	}
	if(colorScheme) {
		parseTextToList();
	}
}

void ofxEditor::clearText() {
	text = "";
	if(colorScheme) {
		clearList();
	}
}

void ofxEditor::setColorScheme(ofxEditorColorScheme &colorScheme) {
	
	// clear to save memory
	if(&colorScheme == NULL) {
		clearList();
		return;
	}
	
	bool shouldReparse = (this->colorScheme == NULL);
	this->colorScheme = &colorScheme;
	
	// reparse if enabling highlighting
	if(shouldReparse) {
		parseTextToList();
	}
}

void ofxEditor::clearColorScheme() {
	this->colorScheme = NULL;
	clearList();
}

ofxEditorColorScheme* ofxEditor::getColorScheme() {
	return colorScheme;
}

void ofxEditor::setLineWrapping(bool wrap) {
	lineWrapping = wrap;
}

bool ofxEditor::getLineWrapping() {
	return lineWrapping;
}

// PROTECTED

void ofxEditor::processTabs() {
	size_t pos = text.find("\t", 0);
	while(pos != string::npos) {
		text.erase(pos, 1);
		text.insert(pos, string(s_tabWidth, ' '));
		pos = text.find("\t",pos);
	}
}

int ofxEditor::offsetToCurrentLineStart() {
	return cursorPos - lineStart(cursorPos);
}

int ofxEditor::nextLineLength(int pos) {
	size_t nextlinestart = text.find("\n", cursorPos);
	if(nextlinestart != string::npos) {
		return lineLength(nextlinestart+1);
	}
	return 0;

}

int ofxEditor::previousLineLength(int pos) {
	size_t previouslineend = string::npos;
	if(pos > 0) {
		previouslineend = text.rfind("\n", pos-1);
	}
	if(previouslineend != string::npos) {
		return lineLength(previouslineend);
	}
	return 0;
}

int ofxEditor::lineLength(int pos) {
	unsigned int linestart = lineStart(pos);
	unsigned int lineend = lineEnd(pos);
	return lineend-linestart;
}

unsigned int ofxEditor::lineStart(int pos) {
	unsigned int linestart = string::npos;

	if(pos > 0) {
		// take one off if we're over a newline
		if(text[pos] == '\n') {
			linestart = text.rfind("\n", pos-1);
		}
		else {
			linestart = text.rfind("\n", pos);
		}
	}
	
	if(linestart != string::npos) {
		linestart++; // move the start off the newline
	}
	else {
		linestart = 0; // if are on the first line, set the start to 0
	}
	
	return linestart;
}

unsigned int ofxEditor::lineEnd(int pos) {
	if(text.empty()) {
		return 0;
	}
	size_t end = text.find("\n", pos);
	if(end == string::npos) {
		end = text.size()-1;
	}
	return end;
}

unsigned int ofxEditor::countTabs(int startPos, int endPos) {
	if(text.empty()) {
		return 0;
	}
	unsigned int found = 0;
	for(int i = startPos; i < endPos; ++i) {
		if(text[i] == '\t') {
			found++;
		}
	}
	return found;
}

// PRIVATE

void ofxEditor::parseTextToList() {
	
	clearList();
	
	TextBlock tb;
	for(int i = 0; i < text.length(); ++i) {
		
		// space
		switch(text[i]) {
		
			case ' ':
				if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = SPACE;
				tb.text = text[i];
				textList.push_back(tb);
				tb.clear();
				break;
		
			case '\n':
				if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = ENDLINE;
				tb.text = text[i];
				textList.push_back(tb);
				tb.clear();
				break;
				
			case '\t':
				if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = TAB;
				tb.text = text[i];
				textList.push_back(tb);
				tb.clear();
				break;
				
			case '"': case '\'':
				if(tb.type == STRING) {
					tb.text += text[i];
					textList.push_back(tb);
					tb.clear();
					break;
				}
				else if(tb.type != UNKNOWN) {
					textList.push_back(tb);
					tb.clear();
				}
				tb.type = STRING;
				tb.text += text[i];
				break;
				
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				if(tb.type != UNKNOWN) {
					if(tb.type == STRING) {
						tb.text += text[i];
						break;
					}
					else if(tb.type == WORD) {
						// detect words after punctuation aka (, [, etc
						if(i > 0 && ispunct(text[i-1]) ) {
							textList.push_back(tb);
							tb.clear();
						}
					}
					else if(tb.type != NUMBER) {
						textList.push_back(tb);
						tb.clear();
					}
				}
				if(tb.type != WORD) {
					tb.type = NUMBER;
				}
				tb.text += text[i];
				break;
		
			case '.': // could be number decimal point
				tb.text += text[i];
				break;
		
			default: // everything else
				switch(tb.type) {
					case NUMBER:
						textList.push_back(tb);
						tb.clear();
					case UNKNOWN:
						tb.type = WORD;
					case WORD: case STRING:
						tb.text += text[i];
						break;
					default:
						break;
				}
				break;
		}
	}
}

void ofxEditor::clearList() {
	textList.clear();
}
