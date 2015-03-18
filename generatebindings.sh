#/bin/sh

cd swig-of
make desktop LANG=python SWIG=swig3.0 DEST_LANG_DIR=../lib
cd ..
cd swig-addicional
make
cd ..
echo copying openframeworks.py to examples
cp lib/* example_ScriptTester/bin/data/
cp lib/* example_simpleScript/bin/data/
cp lib/* example_CallBacks/bin/data/

