# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file 
# system
# 
# The PG can be used to detect errors in this file, just create a new project with this addon 
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxEditor
	ADDON_DESCRIPTION = simple text editor for live coding with syntax highlighting, file browser, and Read-Eval-Print-Loop console
	ADDON_AUTHOR = Dan Wilcox & Akira Hayasaka
	ADDON_TAGS = "text editor" "live coding" "syntax highlighting" "file browser" "console" "REPL"
	ADDON_URL = http://github.com/Akira-Hayasaka/ofxGLEditor
