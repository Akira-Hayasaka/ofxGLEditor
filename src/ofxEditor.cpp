#include "ofxEditor.h"

#define FLASH_RATE 1
#define HALF_FLASH_RATE (FLASH_RATE*0.5)

// max time in secs for cursor to "blow up"
#define BLOWUP_FLASHES 1.8

// max cursor blow up size in chars
#define CURSOR_MAX_WIDTH 40
#define CURSOR_MAX_HEIGHT 40

ofPtr<ofxEditor::Font> ofxEditor::s_font;
int ofxEditor::s_charWidth = 1;
int ofxEditor::s_charHeight = 1;
int ofxEditor::s_cursorWidth = 1;

unsigned int ofxEditor::s_tabWidth = 4;
bool ofxEditor::s_convertTabs = false;

float ofxEditor::s_alpha = 1.0f;
ofColor ofxEditor::s_textColor = ofColor(255);
ofColor ofxEditor::s_cursorColor = ofColor(255, 255, 0);

ofxEditor::ofxEditor() {
	cursorPos = 0;
	desiredXPos = 0;
	
	colorScheme = NULL;
	lineWrapping = false;
	
	m_time = 0;
	m_delta = 0;
	m_cursorFlash = 0;
	m_blowupCursor = false;
	m_blowup = 0;

	viewport.position.set(0, 0, 0);
	setSize(ofGetWidth(), ofGetHeight());
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
		s_cursorWidth = floor(s_charWidth*0.3);
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

void ofxEditor::setAlpha(float alpha) {
	s_alpha = ofClamp(alpha, 0.0f, 1.0f);
}

float ofxEditor::getAlpha() {
	return s_alpha;
}

void ofxEditor::setTextColor(ofColor &color) {
	s_textColor = color;
}

ofColor& ofxEditor::getTextColor() {
	return s_textColor;
}

void ofxEditor::setCursorColor(ofColor &color) {
	s_cursorColor = color;
}

ofColor& ofxEditor::getCursorColor() {
	return s_cursorColor;
}

void ofxEditor::draw() {
	ofPushStyle();
	ofPushView();
	
		ofViewport(viewport);

		// draw text
		if(colorScheme) { // with colorScheme
			ofFill();
			int x = 0, y = s_charHeight; // pixel pos
			int textPos = 0; // char/line pos
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
						break;
						
					case WORD: {
						ofColor &c = colorScheme->getWordColor(tb.text);
						ofSetColor(c.r, c.g, c.b, c.a * s_alpha);
						break;
					}
						
					case STRING: {
						ofColor &c = colorScheme->getStringColor();
						ofSetColor(c.r, c.g, c.b, c.a * s_alpha);
						break;
					}
						
					case NUMBER: {
						ofColor &c = colorScheme->getNumberColor();
						ofSetColor(c.r, c.g, c.b, c.a * s_alpha);
						break;
					}
					
					default:
						break;
				}
				
				// draw block chars
				for(int i = 0; i < tb.text.length(); ++i) {
					
					// draw cursor
					if(textPos == cursorPos) {
						drawCursor(x, y);
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
		else { // without syntax highlighting
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
					drawCursor(x, y);
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
					ofSetColor(s_textColor.r, s_textColor.g, s_textColor.b, s_textColor.a * s_alpha);
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
	
	// update animation timestamps
	m_delta = ofGetElapsedTimef() - m_time;
	m_time = ofGetElapsedTimef();
	if (m_delta > 100.0f) {
		m_delta = 0.000001f;
	}
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
				
				m_cursorFlash = HALF_FLASH_RATE; // show cursor after moving
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
				
				m_cursorFlash = HALF_FLASH_RATE; // show cursor after moving
			}
			break;
		case OF_KEY_LEFT:
			if(cursorPos > 0) {
				cursorPos--;
			}
			desiredXPos = offsetToCurrentLineStart();
			m_cursorFlash = HALF_FLASH_RATE; // show cursor after moving
			break;
		case OF_KEY_RIGHT:
			if(!text.empty()) {
				cursorPos++;
			}
			desiredXPos = offsetToCurrentLineStart();
			m_cursorFlash = HALF_FLASH_RATE; // show cursor after moving
			break;
		case 'b' :
			blowupCursor();
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

void ofxEditor::blowupCursor() {
	m_blowupCursor = true;
	m_blowup = 0;
}

void ofxEditor::reset() {
//	m_PosX=m_PosY=0;
//	m_Scale=1;
//	m_TextWidth=1;
//	m_TextColourRed=1;
//	m_TextColourGreen=1;
//	m_TextColourBlue=1;
	m_blowupCursor = false;
	m_blowup = 0.0f;
}

// PROTECTED

void ofxEditor::drawCursor(int x, int y) {
	
	if(m_blowupCursor) {
		
		// set this to zero when starting
		m_blowup += m_delta;
		if (m_blowup >= BLOWUP_FLASHES) {
			m_blowupCursor = false;
		}
		else {
			float maxCW = (BLOWUP_FLASHES - m_blowup) / BLOWUP_FLASHES * (CURSOR_MAX_WIDTH * s_cursorWidth * 0.5f) + s_cursorWidth * 0.5f;
			float maxCH = (BLOWUP_FLASHES - m_blowup) / BLOWUP_FLASHES * (CURSOR_MAX_HEIGHT * s_charHeight) + s_charHeight;
			ofPushStyle();
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofSetColor(s_cursorColor.r, s_cursorColor.g, s_cursorColor.b, s_cursorColor.a * s_alpha * m_blowup/BLOWUP_FLASHES);
				ofRect(x+maxCW/2, y-s_charHeight+maxCH/2, maxCW, maxCH);
				ofSetRectMode(OF_RECTMODE_CORNER);
			ofPopStyle();
		}
	}
	else {
		m_cursorFlash += m_delta;
		if (m_cursorFlash > FLASH_RATE) {
			m_cursorFlash = 0;
		}
		if (m_cursorFlash > HALF_FLASH_RATE) {
			ofPushStyle();
				ofSetColor(s_cursorColor.r, s_cursorColor.g, s_cursorColor.b, s_cursorColor.a * s_alpha);
				ofRect(x, y-s_charHeight, s_cursorWidth, s_charHeight);
			ofPopStyle();
		}
	}
}

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
