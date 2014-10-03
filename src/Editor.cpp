#include "Editor.h"

ofPtr<Editor::Font> Editor::s_font;
int Editor::s_charWidth = 1;
int Editor::s_charHeight = 1;
unsigned int Editor::s_tabWidth = 4;

Editor::Editor() {
	cursorXPos = 0;
	viewport.position.set(0, 0, 0);
	setSize(ofGetWidth(), ofGetHeight());
}

Editor::~Editor() {
}

bool Editor::loadFont(const string &font, int size) {
	if(s_font == NULL) {
		s_font = ofPtr<Editor::Font>(new Editor::Font());
	}
	if(s_font->loadFont(font, size, true, true, true)) {
		s_charWidth = size;
		s_charHeight = s_font->getLineHeight();
	}
}

void Editor::setTabWidth(unsigned int numSpaces) {
	s_tabWidth = numSpaces;
}

unsigned int Editor::getTabWidth() {
	return s_tabWidth;
}

void Editor::draw() {
	ofPushStyle();
	ofPushView();
	
		ofViewport(viewport);
		
		// draw text
		ofSetColor(255);
		ofFill();
		int x = 0, y = s_charHeight; // pixel pos
		int curPos = 0, curLine = 0; // char/line pos
		for(int i = 0; i < text.length(); ++i) {
		
			// line wrap
			if(curPos >= numCharsWidth) {
				x = 0;
				y += s_charHeight;
				curPos = 0;
				curLine++;
			}
		
			// endline
			if(text[i] == '\n') {
				x = 0;
				y += s_charHeight;
				curPos = 0;
				curLine++;
			}
			// tab
			else if(text[i] == '\t') {
				x += s_charWidth * s_tabWidth;
				curPos += s_tabWidth;
			}
			// everything else
			else {
				s_font->drawCharacter(text[i], x, y);
				x += s_charWidth;
				curPos++;
			}
		}
		
		// grid
		ofSetColor(100, 100, 100);
		for(int x = 0; x < ofGetWidth(); x = x+s_charWidth) {
			ofLine(x, 0, x, ofGetHeight());
		}
		for(int y = s_charHeight; y < ofGetHeight(); y = y+s_charHeight) {
			ofLine(0, y, ofGetWidth(), y);
		}
		
	ofPopView();
	ofPopStyle();
}

void Editor::keyPressed(int key) {
	switch(key) {
		case OF_KEY_UP:
			viewport.y -= s_charHeight;
			break;
		case OF_KEY_DOWN:
			viewport.y += s_charHeight;
			break;
		case OF_KEY_LEFT:
			viewport.x -= s_charWidth;
			break;
		case OF_KEY_RIGHT:
			viewport.x += s_charWidth;
			break;
	}
}

void Editor::setSize(int width, int height) {
	viewport.width = width;
	viewport.height = height;
	
	numCharsWidth = width/s_charWidth;
	numLinesHeight = height/s_charHeight;
	
	ofLogVerbose("Editor") << "pixel size: " << width << " " << height;
	ofLogVerbose("Editor") << "text size: " << numCharsWidth << " " << numLinesHeight;
}

string Editor::getText() {
	return text;
}

void Editor::setText(const string& text) {
	this->text = text;
}

void Editor::clearText() {
	text = "";
}
