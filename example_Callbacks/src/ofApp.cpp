#include "ofApp.h"

class CallBack{
public:
	virtual void _call(){}
};

class ofxPythonCallBack: public CallBack
{
public:
	void _call()
	{
		ofxPython _python;
		_python.init();
		call(_python.getObject("CallBack","openframeworks").attr("_args"),
			 _python.getObject("CallBack","openframeworks").attr("_kwargs"));
	}
	virtual void call(ofxPythonObject args, ofxPythonObject kwargs)=0;
};

class CallBackS : public ofxPythonCallBack{
public:
	void (*cb)();
	void call(ofxPythonObject args, ofxPythonObject kwargs)
	{
		cb();
	}
};

class CallBack1ArgEvent : public ofxPythonCallBack{
public:
	typedef ofEvent<ofxPythonObject> EventType;
	EventType & event;
	CallBack1ArgEvent( EventType & e):event(e){}
	void call(ofxPythonObject args, ofxPythonObject kwargs)
	{
		ofNotifyEvent(event,args.asVector()[0]);
	}
};

void _setCallBackPointer(CallBack * );

ofxPythonObject CallBack2Python(CallBack * c)
{
	ofxPython _python;
	_python.init();
	_setCallBackPointer(c);
	return _python.getObject("_getCallBackPointer","openframeworks")();
}

void p()
{
	ofLog() << "LALALLALAL";
}

class NotSoSimpleCallBack: public ofxPythonCallBack
{
public:
	void call(ofxPythonObject args, ofxPythonObject kwargs)
	{
		ofLog() << args.asVector()[0].asInt();
	}
};

//--------------------------------------------------------------
void ofApp::setup(){
	counter = 0;
	python.init();

	NotSoSimpleCallBack * cb0 = new NotSoSimpleCallBack();
	python.setObject("cb0",CallBack2Python(cb0));
	python.executeString("cb0.call(0)");

	CallBack1ArgEvent * cb1 = new CallBack1ArgEvent(myevent);
	python.setObject("cb1",CallBack2Python(cb1));
	ofAddListener(myevent,this,&ofApp::printsomething);
	python.executeString("cb1.call(10)");

	CallBackS * cb2 = new CallBackS();
	cb2->cb = &p;
	python.setObject("cb2",CallBack2Python(cb2));
	python.executeString("cb2.call()");

}

//--------------------------------------------------------------
void ofApp::update(){
	counter++;
	python.getObject("cb0").attr("call")(ofxPythonObject::fromInt(counter));
	python.getObject("cb1").attr("call")(ofxPythonObject::fromInt(counter));
	python.getObject("cb2").attr("call")();
}

//--------------------------------------------------------------
void ofApp::draw(){

}

void ofApp::printsomething(ofxPythonObject & o){
	ofLog() << "LOLOLOLOLO " << o.asInt();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
