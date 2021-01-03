#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /*
    ofSetLogLevel(OF_LOG_VERBOSE);
    grabber.setVerbose(true);
    grabber.listDevices();
    */
    grabber.setDeviceID(1);
    grabber.initGrabber(320, 240);
    
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    colorImage.allocate(320, 240);
    grayImage.allocate(320, 240);
    grayBackground.allocate(320, 240);
    grayDiff.allocate(320, 240);
    bLearnBackground = true;
    threshold = 100;
    
    box2d.init();
    box2d.setGravity(0, 1);
    box2d.createBounds(0, 0, colorImage.width, colorImage.height);
    box2d.setFPS(30);
    box2d.checkBounds(true);
    
    static const int NUM = 1000;
    for (int i=0; i<NUM; i++) {
        auto circle = make_shared<CustomCircle>();
        circle->setPhysics(1.0, 0.8, 0.0);
        circle->setup(box2d.getWorld(), ofRandom(colorImage.width), ofRandom(colorImage.height), 3);
        circles.push_back(circle);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
    bool bNewFrame = false;
    grabber.update();
    bNewFrame = grabber.isFrameNew();
    if (bNewFrame) {
        colorImage.setFromPixels(grabber.getPixels());
        grayImage = colorImage;
        
        if (bLearnBackground == true) {
            grayBackground = grayImage;
            bLearnBackground = false;
        }
        grayDiff.absDiff(grayBackground, grayImage);
        grayDiff.threshold(threshold);
        contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, false);
        
        for (int i=0; i<contourCircles.size(); i++) {
            contourCircles[i]->destroy();
        }
        contourCircles.clear();
        
        for (int i=0; i<contourFinder.nBlobs; i++) {
            for (int j=0; j<contourFinder.blobs[i].pts.size(); j+=4) {
                glm::vec2 pos = contourFinder.blobs[i].pts[j];
                auto circle = make_shared<ofxBox2dCircle>();
                circle->setup(box2d.getWorld(), pos.x, pos.y, 4);
                contourCircles.push_back(circle);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float ratioX = ofGetWidth() / 320;
    float ratioY = ofGetHeight() / 240;
    
    ofPushMatrix();
    ofScale((float)ofGetWidth()/(float)grayDiff.width, (float)ofGetHeight()/(float)grayDiff.height);
    ofSetColor(255, 255, 255);
    colorImage.draw(0, 0);
    contourFinder.draw();
    ofNoFill();
    ofSetColor(255, 0, 0);
    
    for (int i=0; i<contourCircles.size(); i++) {
        contourCircles[i]->draw();
    }
    
    for (int i=0; i<circles.size(); i++) {
        circles[i]->draw();
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            bLearnBackground = true;
            break;
        case '+':
            threshold++;
            if (threshold > 255) {
                threshold = 255;
            }
            break;
        case '-':
            threshold--;
            if (threshold < 0) {
                threshold = 0;
            }
            break;
    }
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
