ofxGLEditor
===================================

![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/editor.png)

Copyright © [Dan Wilcox](http://danomatika.com) & [Akira Hayasaka](http://www.ampontang.com) 2011-2023

GNU Public License v2.

For information on usage and redistribution, and for a DISCLAIMER OF ALL WARRANTIES, see the file, "LICENSE.txt," in this distribution.

This is a rewrite of the GLEditor from [Fluxus](http://www.pawfal.org/fluxus) Copyright © 2005 Dave Griffiths

Uses:

* [fontstash](https://github.com/memononen/fontstash) for text rendering
* PrintChar21 font from Kreative Software (used in examples)

See https://github.com/Akira-Hayasaka/ofxGLEditor for documentation as well as the [OF forum post on ofxGLEditor](http://forum.openframeworks.cc/t/ofxgleditor/10425)

Description
-----------

Sample Lua syntax highlighting:  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/syntax_highlighting.png)

ofxEditor provides a rewrite of the [Fluxus live coding environment](http://www.pawfal.org/fluxus)'s OpenGL-based text editor for openFrameworks including simple syntax highlighting, a file browser, and a Read-Eval-Print Loop console. The ofxGLEditor provides all of the above including 9 editor buffers, so it should be easy to use this as a basis for live coding, scripting, parameter tweaking, etc. Further, ofxEditorSyntax & ofxEditorColorScheme provide simple language specific syntax highlighting.

[See a video of the livecodingExample which demonstrates the basic ofxEditor class with syntax highlighting using ofxLua.](https://vimeo.com/116370247)

Variable width font and full unicode glyph support (here using Arial Unicode.tff):  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/arial_unicode.png)

Open file dialog:  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/file_dialog.png)

Read-Eval-Print Loop console:  
![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/repl_console.png)

Build Requirements
------------------

To use ofxGLEditor, first you need to download and install openFrameworks. Development is against the latest version of openFrameworks on Github. Checkout a tag if you need an earlier, stable version.

[OF Github repository](https://github.com/openframeworks/openFrameworks)

On macOS, you will need to install Xcode.

On Linux, you can use Makefiles and/or QT Creator project files.

On Win, you will need either Microsoft Visual Studio C++ or [Msys2](https://www.msys2.org/).

Installation
------------

Place ofxGLEditor within a folder in the addons folder of the OF dir tree:
~~~
openframeworks/addons/ofxGLEditor
~~~

Usage
-----

See the examples in `editorSyntaxExample`, `glEditorExample`, `glslExample`, `livecodingExample`, and the source headers in `ofxGLEditor/src`.

### Examples

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the openFrameworks distribution.

To (re)generate project files for an *existing* project:

* Click the "Import" button in the ProjectGenerator
* Navigate to the project's parent folder ie. "ofxGLEditor", select the base folder for the example project ie. "glEditorExample", and click the Open button
* Click the "Update" button

If everything went Ok, you should now be able to open the generated project and build/run the example.

#### glslExample

This is a simple GLSL fragment (pixel) shaders editor including GLSL syntax highlighting. The shader is reloaded whenever it is saved or evaluated (MOD key + e).

#### liveCodingExample

This is a simple livecoding example using ofxLua including lua keyword syntax highlighting. Also, you will need to select `ofxLua` from the addons list when you generate the project files for this example.

### Syntaxes

A growing set of language syntax xml files can be found in the `syntaxes` folder. Additions or updates are welcome. 

### Font

Both monospaced and variable width fonts are supported. PrintChar21.ttf is included with the example projects. Unicode glyphs are supported if your font has them.

Most *real* text editors load multiple fonts to support different language character sets. ofxGLEditor is *simple*, so it only loads 1 font at a time and you'll need to use an expanded character set font for multiple languages. Another option is to load a specific font for a specific language or locale in your app. Giant full character set fonts do exist (eg. [Unifont](http://www.unifoundry.com/unifont.html])), but they are generally too large to be useful as they may take a large amount of resources. Your mileage may vary.
