#pragma once
#include "ofMain.h"
#include "ofxPythonCallBackBase.h"

class ofxPythonObject;

//we can convert a CallBack* to a ofxPythonObject with this method
ofxPythonObject CallBack2Python(CallBack * c);


//Subclass this to get positional and keyword arguments in your callback
class ofxPythonCallBack: public CallBack
{
public:
	void _call();
	virtual void call(ofxPythonObject args, ofxPythonObject kwargs)=0;
	virtual ~ofxPythonCallBack(){}
};

//use this to send a free function as a callback
class ofxPythonCallBackSimple : public ofxPythonCallBack{
public:
	void (*cb)();
	void call(ofxPythonObject args, ofxPythonObject kwargs);
};

//use this to register a callback with one argument to a ofEvent<ofxPythonObject>
class ofxPythonCallBackEvent1Arg : public ofxPythonCallBack{
public:
	typedef ofEvent<ofxPythonObject> EventType;
	EventType & event;
	ofxPythonCallBackEvent1Arg( EventType & e);
	void call(ofxPythonObject args, ofxPythonObject kwargs);
};