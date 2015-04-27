#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	python.init();
	python.executeScript("mytest.py");
	ofxPythonObject klass = python.getObject("myApp");
	if(klass)
		script = klass();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (script)
	{	
		ofxPythonObject at = script.attr("update");
		if (at) at();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (script)
	{
		ofxPythonObject at = script.attr("draw");
		if (at) at();
	}
	ofSetColor(0,0,0);
	ofDrawBitmapString("Press 'R' to reload script",10,20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (script)
	{
		ofxPythonObject at = script.attr("keyPressed");
		if (at)
			at(ofxPythonObject::fromInt(key));
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (script)
	{
		ofxPythonObject at = script.attr("keyReleased");
		if (at)
			at(ofxPythonObject::fromInt(key));
	}
	if (key == 'R' || key == 'r')
	{
		python.reset();
		python.executeScript("mytest.py");
		ofxPythonObject klass = python.getObject("myApp");
		if(klass)
		{
			script = klass();
		}
		else
		{
			script = ofxPythonObject::_None();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (script)
	{
		ofxPythonObject at = script.attr("mouseMoved");
		if (at)
			at( ofxPythonObject::fromInt(x),
				ofxPythonObject::fromInt(y));
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (script)
	{
		ofxPythonObject at = script.attr("mouseDragged");
		if (at)
			at( ofxPythonObject::fromInt(x),
				ofxPythonObject::fromInt(y),
				ofxPythonObject::fromInt(button));
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (script)
	{
		ofxPythonObject at = script.attr("mousePressed");
		if (at)
			at( ofxPythonObject::fromInt(x),
				ofxPythonObject::fromInt(y),
				ofxPythonObject::fromInt(button));
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (script)
	{
		ofxPythonObject at = script.attr("mouseReleased");
		if (at)
			at( ofxPythonObject::fromInt(x),
				ofxPythonObject::fromInt(y),
				ofxPythonObject::fromInt(button));
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	if (script)
	{
		ofxPythonObject at = script.attr("windowResized");
		if (at)
			at( ofxPythonObject::fromInt(w),
				ofxPythonObject::fromInt(h));
	}
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
