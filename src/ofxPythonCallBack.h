#pragma once

class ofxPythonObject;

class CallBack{
public:
	virtual void _call(){}
};

class ofxPythonCallBack: public CallBack
{
public:
	void _call();
	virtual void call(ofxPythonObject args, ofxPythonObject kwargs)=0;
};

