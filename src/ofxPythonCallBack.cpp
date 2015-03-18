#include "ofxPython.h"
#include "ofxPythonCallBack.h"

void _setCallBackPointer(CallBack * );


ofxPythonObject CallBack2Python(CallBack * c)
{
	ofxPython _python;
	_python.init();
	_setCallBackPointer(c);
	return _python.getObject("_getCallBackPointer","openframeworks_extra")();
}


void ofxPythonCallBack::_call()
{
	ofxPython _python;
	_python.init();
	call(_python.getObject("CallBack","openframeworks_extra").attr("_args"),
		 _python.getObject("CallBack","openframeworks_extra").attr("_kwargs"));
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
