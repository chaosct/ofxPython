#include "ofApp.h"
#include "ofxPythonCallBack.h"


void p()
{
	ofDrawBitmapString("ofxPythonCallBackSimple",10,20);
}

//This is a custom callback
class CustomCallBack: public ofxPythonCallBack
{
public:
	void call(ofxPythonObject args, ofxPythonObject kwargs)
	{
		ofDrawBitmapString("CustomCallBack "+ofToString(args.asVector()[0].asInt()),10,60);
	}
};

//--------------------------------------------------------------
void ofApp::setup(){
	counter = 0;
	python.init();

	ofxPythonCallBackSimple * cb0 = new ofxPythonCallBackSimple();
	cb0->cb = &p;
	python.setObject("cb0",CallBack2Python(cb0));
	python.executeString("cb0.call()");

	ofxPythonCallBackEvent1Arg * cb1 = new ofxPythonCallBackEvent1Arg(myevent);
	python.setObject("cb1",CallBack2Python(cb1));
	ofAddListener(myevent,this,&ofApp::printsomething);
	python.executeString("cb1.call(10)");

	CustomCallBack * cb2 = new CustomCallBack();
	python.setObject("cb2",CallBack2Python(cb2));
	python.executeString("cb2.call(0)");

}

//--------------------------------------------------------------
void ofApp::update(){
	counter++;
}

//--------------------------------------------------------------
void ofApp::draw(){
	python.getObject("cb0").attr("call")();
	python.getObject("cb1").attr("call")(ofxPythonObject::fromInt(counter));
	python.getObject("cb2").attr("call")(ofxPythonObject::fromInt(counter));
}

void ofApp::printsomething(ofxPythonObject & o){
	ofDrawBitmapString("ofxPythonCallBackEvent1Arg "+ofToString(o.asInt()),10,40);
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
