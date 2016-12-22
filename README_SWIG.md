ofxPython
=========

It took me a bit to work out how to make this addon work with newer OF versions.

1. install homebrew
Go here [http://brew.sh](http://brew.sh) and copy the script which looks like this:
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
Past the script in to Terminal.app.
Hit enter.

2. install SWIG
Paste     brew install swig     in to Terminal.app and hit enter.

3. get newest swig-of
go to https://github.com/danomatika/swig-openframeworks and download the zip
place the content of the swig-of-master folder inside the already exisiting swig-of folder.

4. navigate the terminal to the addon folder
cd /Applications/of_v0.9.8_osx_release/addons/ofxPython 
then run the script provided with the addon   ./generatebindings.sh

It looks like generatebindings.sh also makes changes in each of the example folders.
I had to change ofCircle to ofDrawCircle in bin/data/myTest.sh.
I have tested this on OF v0.9.8 on OSX 10.11.6