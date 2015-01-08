ofxPython
=========

(c) 2014 Carles F. Julià <carles@protopixel.net>

Introduction
------------
May be you are like me and love Python and openFrameworks, or maybe you just need to embed a scripting engine and dislike Lua. Lua is nice but Python comes full of batteries.

This addon allows you to embed a Python interpreter into an openFrameworks application, and offers Python bindings to many OF functions, so you can draw from Python code.

This addon borrows ideas from [pyof](https://github.com/johnglover/pyof), which I was unable to replicate in Linux.

License
-------
See license.md. Spoiler alert: it's MIT

Installation
------------
Drop the folder into the `OF/addons/` folder. If your project generator is clever enough it will copy the `OF/addons/ofxPython/lib/openframeworks.py` file into the `data/` folder, copy it manually otherwise.

You will need Python (2.7 tested) installed in order to compile your program.

Compatibility
------------
It is now compiling both in Linux64 and Windows (Code::Blocks)

Re-generating the bindings
--------------------------
In case that you want to regenerate the bindings, do so with [SWIG](http://www.swig.org/):

```
$ cd swig
$ make desktop LANG=python DEST_DIR=../src/bindings SWIG=swig3.0 DEST_LANG_DIR=../lib
```

Known issues
------------
- Not fully tested

Contributing
------------
Please do! Open issues, send pull requests etc.
