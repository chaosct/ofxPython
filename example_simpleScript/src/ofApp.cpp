#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	python.init();
	ofxPythonObject klass = python.getObject("myApp","mytest");
	if(klass)
		python_program = klass();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(python_program)
		python_program.method("update");
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(python_program)
		python_program.method("draw");
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
