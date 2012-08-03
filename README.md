ofxGLEditor
===================================

![image](https://github.com/Akira-Hayasaka/ofxGLEditor/raw/master/doc/screenshot.png)

Copyright © [Akira Hayasaka](http://www.ampontang.com/) & [Dan Wilcox](http://danomatika.com) 2011-2012

GNU Public License v2.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

Main functionality and sources from [Fluxus](http://www.pawfal.org/fluxus/)
Copyright © 2005 Dave Griffiths

Clipboard code from [here](http://forum.openframeworks.cc/index.php?topic=2296.0)

Droid Sans Mono Font by the Open Handset Alliance (Android)

See https://github.com/Akira-Hayasaka/ofxGLEditor for documentation as well as the [OF forum post on ofxGLEditor](http://forum.openframeworks.cc/index.php/topic,10425.0.html)

Description
-----------

ofxGLEditor provides a version the [Fluxus live coding environment](http://www.pawfal.org/fluxus/)'s OpenGL-based text editor for OpenFrameworks. There are 9 editor buffers which are edited and accessed as std::strings, so it should be easy to use this as a basis for live coding, scripting, parameter tweaking, etc

Build Requirements
------------------

To use ofxGLEditor, first you need to download and install Open Frameworks. Development is against the latest version of Open Frameworks on github. Checkout a tag if you need an earlier, stable version.

[OF github repository](https://github.com/openframeworks/openFrameworks)

On Mac OSX, you will need to install Xcode.

On Linux, you can use the Makefile and/or Codeblocks project files (without the "_win" suffix).

On Win, you will need either Microsoft Visual Studio C++ or [Codeblocks+MiniGW](http://www.codeblocks.org/downloads/26) and the [Win Codeblocks OF package](http://www.openframeworks.cc/download). Use the Codeblocks projects files with the "_win" suffix.

Installation
------------

Place ofxGLEditor within a folder in the apps folder of the OF dir tree:
<pre>
openframeworks/addons/ofxGLEditor
</pre>

#### Which version to use?

If you are using a stable version (0062, 007, ...) of OpenFrameworks then you want to use a git tag of ofxGLEditor for that version. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

For example, the following commands will clone ofxGLEditor and switch to the OF 007 tagged version:
<pre>
git://github.com/Akira-Hayasaka/ofxGLEditor.git
cd ofxGLEditor
git checkout 007
</pre>

Usage
-----

See the example in `EditorExample` and `src/ofxGLEditor.h`.

Note: You will need a monospaced font for best effect. DroidSansMono.ttf is included with the example application.