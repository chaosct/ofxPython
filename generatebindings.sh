#/bin/sh

cd swig
make desktop LANG=python SWIG=swig3.0 DEST_LANG_DIR=../lib
cd ..
echo copying openframeworks.py to examples
cp lib/openframeworks.py example_ScriptTester/bin/data/
cp lib/openframeworks.py example_simpleScript/bin/data/