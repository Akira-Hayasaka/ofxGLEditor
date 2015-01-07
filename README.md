ofxEditor
===================================

![image](https://github.com/danomatika/ofxEditor/raw/master/doc/editor.png)

Copyright © [Dan Wilcox](http://danomatika.com) 2015

GNU Public License v2.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

This is a rewrite of the GLEditor from [Fluxus](http://www.pawfal.org/fluxus) (Copyright © 2005 Dave Griffiths) and the [ofxGLEditor](https://github.com/Akira-Hayasaka/ofxGLEditor) wrapper addon for OpenFrameworks by Akira Hayasaka & Dan Wilcox for better looks and new functionality.

PrintChar21 font from Kreative Software

See https://github.com/danomatika/ofxEditor for documentation

Description
-----------

![image](https://github.com/danomatika/ofxEditor/raw/master/doc/syntax_highlighting.png)
<p style="text-align: center;"><small>Sample Lua syntax highlighting</small></p>

ofxEditor provides a rewrite of the [Fluxus live coding environment](http://www.pawfal.org/fluxus)'s OpenGL-based text editor for OpenFrameworks including simple syntax highlighting, a file browser, and a Read-Eval-Print Loop console. The ofxMultiEditor provides all of the above including 9 editor buffers, so it should be easy to use this as a basis for live coding, scripting, parameter tweaking, etc

![image](https://github.com/danomatika/ofxEditor/raw/master/doc/file_dialog.png)
<p style="text-align: center;"><small>Open file dialog</small></p>

![image](https://github.com/danomatika/ofxEditor/raw/master/doc/repl_console.png)
<p style="text-align: center;"><small>Read-Eval-Print Loop console</small></p>

Build Requirements
------------------

To use ofxEditor, first you need to download and install Open Frameworks. Development is against the latest version of Open Frameworks on github. Checkout a tag if you need an earlier, stable version.

[OF github repository](https://github.com/openframeworks/openFrameworks)

On Mac OSX, you will need to install Xcode.

On Linux, you can use Makefiles and/or Codeblocks project files (without the "_win" suffix).

On Win, you will need either Microsoft Visual Studio C++ or [Codeblocks+MiniGW](http://www.codeblocks.org/downloads/26) and the [Win Codeblocks OF package](http://www.openframeworks.cc/download). Use the Codeblocks projects files with the "_win" suffix.

Installation
------------

Place ofxEditor within a folder in the addons folder of the OF dir tree:
<pre>
openframeworks/addons/ofxEditor
</pre>

Usage
-----

See the examples in `editorExample`, `multiEditorExample`, and the source headers in `ofxEditor/src`.

### Examples

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the OpenFrameworks distribution.

Point the ProjectGenerator to `addons/ofxEditor`, change the project name to the **exact** name of the example you want to generate (ie `editorExample`), and make sure to choose `ofxEditor` from the addons. Hitting "Generate Project" will populate that example with the project files you will need to build it.

### Font

You will need a monospaced font as variable width fonts are not currently supported. PrintChar21.ttf is included with the example projects.
