ofxPython
=========

Generating bindings for newer OF versions.

1. Install [SWIG](http://www.swig.org/)
    * OSX
        1. install homebrew
            Go here [http://brew.sh](http://brew.sh) and copy the script which looks like this: `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
        Paste the script in to Terminal.app.
        Hit enter.

        2. install SWIG
        Paste `brew install swig` into Terminal.app and hit enter.
    
    * Linux
    `sudo apt-get install swig`

3. Get newest swig-of
Navigate the terminal to the addon folder: `cd /Applications/of_v0.9.8_osx_release/addons/ofxPython`
And pull the newest version of the swig files
`(cd swig-of && git pull)`

4. Re-generate bindings
`./generatebindings.sh`
generatebindings.sh also makes changes in each of the example folders, making sure they have the needed files.
