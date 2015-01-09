ofxGLEditor
===================================

![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/editor.png)

Copyright © [Dan Wilcox](http://danomatika.com) & [Akira Hayasaka](http://www.ampontang.com) 2011-2015

GNU Public License v2.

For information on usage and redistribution, and for a DISCLAIMER OF ALL WARRANTIES, see the file, "LICENSE.txt," in this distribution.

This is a rewrite of the GLEditor from [Fluxus](http://www.pawfal.org/fluxus) Copyright © 2005 Dave Griffiths

PrintChar21 font from Kreative Software

See https://github.com/Akira-Hayasaka/ofxGLEditor for documentation as well as the [OF forum post on ofxGLEditor](http://forum.openframeworks.cc/t/ofxgleditor/10425)

Description
-----------

Sample Lua syntax highlighting:  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/syntax_highlighting.png)

ofxEditor provides a rewrite of the [Fluxus live coding environment](http://www.pawfal.org/fluxus)'s OpenGL-based text editor for OpenFrameworks including simple syntax highlighting, a file browser, and a Read-Eval-Print Loop console. The ofxMultiEditor provides all of the above including 9 editor buffers, so it should be easy to use this as a basis for live coding, scripting, parameter tweaking, etc

[See a video of the livecodingExample which demonstrates the basic ofxEditor class with syntax highlighting using ofxLua.](https://vimeo.com/116370247)

Open file dialog:  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/file_dialog.png)

Read-Eval-Print Loop console:  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/repl_console.png)

Build Requirements
------------------

To use ofxGLEditor, first you need to download and install OpenFrameworks. Development is against the latest version of OpenFrameworks on github. Checkout a tag if you need an earlier, stable version.

[OF github repository](https://github.com/openframeworks/openFrameworks)

On Mac OSX, you will need to install Xcode.

On Linux, you can use Makefiles and/or Codeblocks project files (without the "_win" suffix).

On Win, you will need either Microsoft Visual Studio C++ or [Codeblocks+MiniGW](http://www.codeblocks.org/downloads/26) and the [Win Codeblocks OF package](http://www.openframeworks.cc/download). Use the Codeblocks projects files with the "_win" suffix.

Installation
------------

Place ofxGLEditor within a folder in the addons folder of the OF dir tree:
<pre>
openframeworks/addons/ofxGLEditor
</pre>

Usage
-----

See the examples in `editorExample`, `editorSyntaxExample`, `livecodingExample`, and the source headers in `ofxGLEditor/src`.

### Examples

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the OpenFrameworks distribution.

Point the ProjectGenerator to `addons/ofxGLEditor`, change the project name to the **exact** name of the example you want to generate (ie `editorExample`), and make sure to choose `ofxGLEditor` from the addons. Hitting "Generate Project" will populate that example with the project files you will need to build it.

#### liveCodingExample

This is a simple livecoding example using ofxLua including lua keyword syntax hilighting. **It currently uses the ofxLua `swig` branch, so make sure you checkout the right branch.** Also, you will need to select `ofxLua` from the addons list when you generate the project files for this example.

### Font

You will need a monospaced font as variable width fonts are not currently supported. PrintChar21.ttf is included with the example projects.
