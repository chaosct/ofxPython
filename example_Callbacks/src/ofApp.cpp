#include "ofApp.h"
#include "ofxPythonCallBack.h"


class CallBackS : public ofxPythonCallBack{
public:
	void (*cb)();
	void call(ofxPythonObject args, ofxPythonObject kwargs)
	{
		cb();}
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

//--------------------------------------------------------------
void ofApp::setup(){
	python.init();
	CallBackS * c = new CallBackS();
	c->cb = &p;

	python.setObject("cb",CallBack2Python(c));
	python.executeString("cb.call()");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

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
