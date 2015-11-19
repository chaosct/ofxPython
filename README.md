swig-openframeworks
-------------------

a SWIG interface for openFrameworks with included Makefile

Copyright (c) [Dan Wilcox](danomatika.com) 2015

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See <https://github.com/danomatika/swig-openframeworks> for documentation

Description
-----------

swig-openframeworks provides a centralized OpenFrameworks API interface file for the Simple Wrapper and Interface Generator (SWIG) tool which can be used to generate C++ bindings for various scripting languages, including Lua, Python, and Javascript.

[SWIG](http://www.swig.org) is a software development tool that connects programs written in C and C++ with a variety of high-level programming languages.

Installation
------------

This repository is designed to be used as a submodule within OpenFrameworks addons which provide scripting access to OF.

The suggested installation is to the `swig` folder in your main repo:

    git submodule add https://github.com/danomatika/swig-openframeworks.git swig

Git users of your addon will then need to checkout this submodule when they clone your addon:

    git submodule init
    git submodule update

### Which version to use?

If you are using a stable version (0.8.4, 0.9.0, ...) of OpenFrameworks then you want to use a git tag of swig-openframeworks for that version. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

The master branch of swig-openframeworks will work with the current stable version.

OF API Bindings
---------------

Currently the swig interface file covers *most* of the api while leaving out base classes. It creates a module containing all the imported functions, classes, constants & enums.

To see the main differences with the OF C++ API run the following:

    grep DIFF swig/openFrameworks.i
     
To see work to be done on the bindings run:

    grep TODO swig/openFrameworks.i

Currently supported (aka tested) scripting languages are:

* Lua
* Python

Other language bindgins supported by swig can be generated. Feel free to create a PR adding required updates to the Makefile and interface file.

### Lua

In Lua, all wrapped functions, classes, defines, and enums are added to an "of" module (aka table) like a Lua library.

The contents of the module are renamed by default:

* **function**: ofBackground -> of.background
* **class**: ofColor -> of.Color
* **constant**: OF_LOG_VERBOSE -> of.LOG_VERBOSE
* **enum**: ofShader::POSITION_ATTRIBUTE -> of.Shader.POSITION_ATTRIBUTE

### Python

In Python, the module (aka library) is called "openframeworks" and its members retain the "of" prefix:

    from openframeworks import *
    
    ofBackground(255)
    color = ofColor()
    ...

Usage
-----

A Makefile is provided to handle SWIG and platform specific details. It generates both a .cpp binding implementation as well as a .h header for SWIG helper functions.

Basic usage is:

    make

which generates Lua bindings for desktop OSs and places them in `../src/bindings/desktop`.	

### Languages

Available scripting languages are listed by the SWIG help: `swig -h`. The langauge can be set by the LANG makefile variable and is `lua` by default.

Example: To generate python bindings for desktop:

    make LANG=python

### Platforms

The Makefile current supports generating bindings for the following target platforms:

* **desktop**: win, linux, & mac osx
* **ios**: apple iOS using OpenGL ES
* **linuxarm**: embedded linux using OpenGL ES

Generated bindings are placed within platform subfolders in the destination directory.

Example: To generate python bindings for iOS:

    make ios LANG=python

### Destination Dir

The destination directory where the bindings are installed can be set by the DEST_DIR makefile variable and is `../src/bindings` by default.

Example: To generate python bindings for desktop and place them in a nearby test dir:

    make desktop LANG=python DEST_DIR=test

### Bindings Filename

The Makefile generates bindings files using the NAME makefile variable and is `openFrameworks_wrap` by default.

Example: To generate python bindings for desktop with the name "ofxPythonBindings":

    make desktop LANG=python NAME=ofxPythonBindings

### Module Name

The scripting language bindings use the "of" module by default. In the case of Lua, this refers to the parent "of" table that contains all warped functions, classes, and defines. This may not be desirable for particular scripting languages (ie. Python), so the module name can be set using the MODULE_NAME makefile variable.

Example: To egenrate python bindings with the module name "openframeworks":

    make LANG=python MODULE_NAME=openframeworks

### Renaming

By default, functions, classes, enums, & defines are reamed to strip the "of" prefix. This may not be desirable for particular scripting languages (ie. Python) and can be disabled by setting the MODULE_NAME makefile variable to false:

    make RENAME=false

### Scripting

Generating bindings for all platform targets can be done easily using a script which calls the Makefile. See [generate_bindings.sh](https://github.com/danomatika/ofxLua/blob/swig/scripts/generate_bindings.sh) in ofxLua for an example.

### Debugging

Debugging scripting language bindings can be a pain, so SWIG can output the languages debug symbols. The Makefile provides a target that creates a `debug.txt` so you can see which classes, functions, & enums are being wrapped and how.

Example: To debug python bindings:

    make debug LANG=python

Example
-------

swig-openframeworks was originally developed for ofxLua which is using it as a submodule in the "swig" branch: <https://github.com/danomatika/ofxLua/tree/swig>

Developing
----------

You can help develop swig-openframeworks on GitHub: <https://github.com/danomatika/swig-openframeworks>

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.