#include "ofxPython.h"
#include "ofxPythonCallBack.h"

void ofxPythonCallBack::_call()
{
	ofxPython _python;
	_python.init();
	call(_python.getObject("CallBack","openframeworks").attr("_args"),
		 _python.getObject("CallBack","openframeworks").attr("_kwargs"));
}
