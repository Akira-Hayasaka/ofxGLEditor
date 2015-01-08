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
 *
 * A rewrite of the Fluxus FileDialog http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#pragma once

#include "ofxEditor.h"

class ofxFileDialog : public ofxEditor {

	public:

		ofxFileDialog();
		ofxFileDialog(ofxEditorSettings &sharedSettings); //< share settings object
		virtual ~ofxFileDialog();

		/// draw the file dialog, pushes view and applies viewport
		virtual void draw();
	
		/// handles key events
		virtual void keyPressed(int key);
	
		/// set the current path in the file browser, reads directory contents
		void setPath(string path);

		/// refresh directory contents
		void refresh();

		/// get the currently selected path
		string getSelectedPath();
	
		/// clear the currently selected path
		void clearSelectedPath();
	
		/// enable/disable save as mode
		void setSaveAsMode(bool saveAs);
	
		/// currently in save as mode?
		bool getSaveAsMode();

	protected:

		void drawSaveAs();
		void drawOpen();
		void keyPressedSaveAs(int key);
		void keyPressedOpen(int key);

		unsigned int m_currentFile; //< index of the current file
		vector<string> m_filenames; //< filenames & directories
		set<int> m_directories;     //< which indexes are directories
		string m_path;              //< current path
		string m_selectedPath;      //< selected path on enter
	
		/// number of files to show above and below open file cursor
		static const unsigned int s_fileDisplayRange;
		static const string s_saveAsInfoText; //< save as greeting text
		bool m_saveAs; //< are we in save as mode?
};
