#include "ofxPython.h"
#include "ofxPythonCallBack.h"

void _setCallBackPointer(CallBack * );


ofxPythonObject CallBack2Python(CallBack * c)
{
	_setCallBackPointer(c);
	return ofxPython::getObject("_getCallBackPointer","openframeworks_extra")();
}


void ofxPythonCallBack::_call()
{
	call(ofxPython::getObject("CallBack","openframeworks_extra").attr("_args"),
		 ofxPython::getObject("CallBack","openframeworks_extra").attr("_kwargs"));
}

void ofxPythonCallBackSimple::call(ofxPythonObject args, ofxPythonObject kwargs)
{
	cb();
}

ofxPythonCallBackEvent1Arg::ofxPythonCallBackEvent1Arg( EventType & e):event(e)
{

}

void ofxPythonCallBackEvent1Arg::call(ofxPythonObject args, ofxPythonObject kwargs)
{
	ofNotifyEvent(event,args.asVector()[0]);
}
