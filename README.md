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
Drop the folder into the `OF/addons/` folder. If your project generator is clever enough it will copy the `OF/addons/ofxPython/lib/python_openframeworks/_YOUR_OS_/openframeworks.py` file into the `data/` folder, copy it manually otherwise.

You will need Python (2.7 tested) installed in order to compile your program.

Compatibility
------------
This has been designed for OF v0.8.4. I don't have a way of creating per-version bindings, but you can try to generate them using swig (see next section).

I've generated the bindings and tested them on Linux64, OSX and Windows.

Re-generating the bindings
--------------------------
In case that you are using another version of OF or another OS, you can generate the bindings with [SWIG](http://www.swig.org/). You may have to modify the interface files (*.i), and then:

```
$ swig3.0 -c++ -python -outdir lib/python_openframeworks/linux/ src/bindings/linux/openframeworks.i
```

Known issues
------------
- It crashes on exit.
- It is awfully incomplete.
- No clear error reporting

Contributing
------------
Please do! Open issues, send pull requests etc.
