ofxPython
=========

Generating bindings for newer OF versions.

OSX
---

1. install homebrew
Go here [http://brew.sh](http://brew.sh) and copy the script which looks like this:
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
Paste the script in to Terminal.app.
Hit enter.

2. install SWIG
Paste `brew install swig` into Terminal.app and hit enter.

3. get newest swig-of

navigate the terminal to the addon folder, and type:

```
cd swig-of
git pull
```

4. navigate the terminal to the addon folder
cd /Applications/of_v0.9.8_osx_release/addons/ofxPython 
then run the script provided with the addon   ./generatebindings.sh

It looks like generatebindings.sh also makes changes in each of the example folders.
I had to change ofCircle to ofDrawCircle in bin/data/myTest.sh.
I have tested this on OF v0.9.8 on OSX 10.11.6

I also was able to take this version of addon for 0.9.8 and just drop it in to 0.9.3. Works.


LINUX - Raspberry PI 3 armv6
----------------------------

1. install swig
sudo apt-get install swig

But the examples do not yet compile.