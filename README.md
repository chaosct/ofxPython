ofxPython
=========

(c) 2017 Carles F. Julià <carles@protopixel.net>

Introduction
------------
May be you are like me and love Python and openFrameworks, or maybe you just need to embed a scripting engine and dislike Lua. Lua is nice but Python comes full of batteries.

This addon allows you to embed a Python interpreter into an openFrameworks application, and offers Python bindings to many OF functions, so you can draw from Python code.

This addon uses the [common swig bindings for OF](https://github.com/danomatika/swig-openframeworks).

License
-------
See license.md. Spoiler alert: it's MIT

Installation
------------
Drop the folder into the `OF/addons/` folder. If your project generator is clever enough it will copy the `OF/addons/ofxPython/lib/*.py` files into the `data/` folder, copy it manually otherwise.

You will need Python (2.7 tested) installed in order to compile your program.

Compatibility
------------
It is now compiling both in Linux64, Windows (Code::Blocks and Visual Studio) and OSX. Generated for OF 0.8.9

###Linux:###
Make sure the python-dev package is installed:

```
$ apt-get install python-dev
```

###Windows/Visual Studio###
The Visual Studio projects assume Python 2.7 is installed in `C:\Python27`. If you have installed python in a different location, you can set an environment variable named `PYTHON` with the correct path.

Re-generating the bindings
--------------------------
See [README_SWIG.md](README_SWIG.md).

Known issues
------------
- Not fully tested

Contributing
------------
Please do! Open issues, send pull requests etc.
