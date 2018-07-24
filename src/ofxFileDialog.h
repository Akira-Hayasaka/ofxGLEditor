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
 *
 * A rewrite of the Fluxus FileDialog http://www.pawfal.org/fluxus for OF
 * Copyright (C) Dave Griffiths
 */
#pragma once

#include "ofxEditor.h"

/// key events
///
/// ARROWS: select file to open
/// RETURN: open/save file
/// ESC: exit file dialog
///
class ofxFileDialog : public ofxEditor {

	public:

		/// file dialog mode types
		enum Mode {
			OPEN,
			SAVEAS,
			NEWFOLDER
		};

		ofxFileDialog();
		ofxFileDialog(ofxEditorSettings &sharedSettings); //< share settings object
		virtual ~ofxFileDialog();
	
	/// \section Main

		/// draw the file dialog, pushes view and applies viewport
		virtual void draw();
	
		/// handles key events
		///
		/// Note: ESC to exit must be disabled as ESC is used to exit
		///
		virtual void keyPressed(int key);
	
		/// set the current path in the file browser, reads directory contents
		void setPath(std::string path);

		/// refresh directory contents
		void refresh();

		/// get the currently selected path
		string getSelectedPath();
	
		/// clear the currently selected path
		void clearSelectedPath();
	
		/// set the current mode, sets active to true
		void setMode(Mode mode);
	
		/// get current mode
		Mode getMode();
	
		/// is the dialog currently active? set to true when setting the current mode,
		/// set to false after dialog exit action detected
		bool isActive();
	
		/// activate/deactivate the dialog
		void setActive(bool active=true);
	
		bool openFile(std::string filename); //< dummy implementation
		bool saveFile(std::string filename); //< dummy implementation
		void undo(); //< no undo, dummy implementation
		void redo(); //< no undo, dummy implementation
	
	/// \section Static Utils
	
		/// set/get the save as info text, default: "Save as (esc to exit)"
		static void setSaveAsText(const std::u32string &text);
		static void setSaveAsText(const std::string &text);
		static std::u32string& getWideSaveAsText();
		static std::string getSaveAsText();
	
		/// set/get the new folder info text, default: "New folder (esc to exit)"
		static void setNewFolderText(const std::u32string &text);
		static void setNewFolderText(const std::string &text);
		static std::u32string& getWideNewFolderText();
		static std::string getNewFolderText();
	
		/// set/get the new folder button text in the save as dialog, default: "New Folder"
		static void setNewFolderButton(const std::u32string &text);
		static void setNewFolderButton(const std::string &text);
		static std::u32string& getWideNewFolderButton();
		static std::string getNewFolderButton();

	protected:

		void drawSaveAs();
		void drawOpen();
		void drawNewFolder();
		void keyPressedSaveAs(int key);
		void keyPressedOpen(int key, bool saveAs=false);
		void keyPressedNewFolder(int key, bool saveAs=false);
	
		/// draw filenames & directory list centered on the current file
		/// topOffset = num vert chars from top constraint
		/// bottomOffset = num vert chars from bottom constraint
		/// highlight = highlight current file?
		void drawFilenames(int topOffset=2, int bottomOffset=0, bool highlight=true);

		/// handle text input into the buffer
		void keyPressedText(int key);

		Mode m_mode; //< current dialog mode
		bool m_active; //< is the dialog active?
	
		unsigned int m_currentFile;    //< index of the current file
		std::vector<std::u32string> m_filenames; //< filenames & directories
		std::set<int> m_directories;        //< which indexes are directories
		std::u32string m_path;              //< current path
		std::u32string m_selectedPath;      //< selected path on enter
		std::u32string m_prevBasename;      //< previous path basename
	
		/// number of files to show above and below open file cursor
		static std::u32string s_saveAsText; //< save as info text
		static std::u32string s_newFolderText; //< new folder info text
		static std::u32string s_newFolderButtonText; //< save as new folder "button"
	
		/// save as dialog states
		enum SaveAsState {
			FILENAME,      //< entering filename
			BROWSER,       //< using filebrowser
			FOLDER,        //< on new folder button
			FOLDER_DIALOG  //< in the new folder dialog
		};
		SaveAsState m_saveAsState; //< current save as state
};
