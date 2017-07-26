#include "ofApp.h"
#include <opencv2/opencv.hpp>

using namespace ofxCv;
using namespace cv;

Mat frame;
ofImage thisImage;


//definied as extern in ofApp.h
float camThresh;
float camSoftness;
float camInvert;
float camOpacity;
int camShaderActive;

int CAM_DEV_ID;
int CAM_W;
int CAM_H;
int THUMB_W;
int THUMB_H;
int CAM_FPS;

//recording layers
int LAYER_COUNT;
int FRAME_COUNT;

//render window
int WINDOW_W;
int WINDOW_H;

//MIDI Controller Numbers
int MIDI_PORT;
int MIDI_REC;
int MIDI_SCALE;
int MIDI_XPOS;
int MIDI_YPOS;

int MIDI_SHADER_BYPASS;
int MIDI_SHADER_THRESH;
int MIDI_SHADER_SOFT;
int MIDI_SHADER_INVERT_ON;
int MIDI_SHADER_INVERT_OFF;
int MIDI_SHADER_OPACITY;
int MIDI_SHADER_CAM_OPACITY;
int MIDI_SHADER_CAM_THRESH;
int MIDI_SHADER_CAM_SOFT;
int MIDI_SHADER_CAM_INVERT_ON;
int MIDI_SHADER_CAM_INVERT_OFF;

//default layer settings
float LAYER_SCALE;
float SHADER_THRESH;
float SHADER_INVERT;
float SHADER_SOFT;
float SHADER_OPACITY;
int SHADER_ACTIVE;
string SHADER_MASK;

//debugging
int USE_OFXCVPI = 1;

//--------------------------------------------------------------
void ofApp::setup(){

    
    //-------------------------------------
    //--XML-settings-----------------------
    //-------------------------------------
    //read all initial values into extern variables from XML settings file
    XML.loadFile("mySettings.xml");
 
    //render window
    WINDOW_W = XML.getValue("WINDOW_W", 0);
    WINDOW_H = XML.getValue("WINDOW_H", 0);
    
    
    CAM_DEV_ID = XML.getValue("CAM_DEV_ID", 0);
    CAM_W = XML.getValue("CAM_W", 0);
    CAM_H = XML.getValue("CAM_H", 0);
    CAM_FPS = XML.getValue("CAM_FPS", 0);
    
    THUMB_W = XML.getValue("THUMB_W", 0);
    THUMB_H = XML.getValue("THUMB_H", 0);
    
    
    //recording layers
    LAYER_COUNT = XML.getValue("LAYER_COUNT", 0);
    FRAME_COUNT = XML.getValue("FRAME_COUNT", 0);

    
    //MIDI Controller Numbers
    MIDI_PORT = XML.getValue("MIDI_PORT", 0);
    MIDI_REC = XML.getValue("MIDI_REC", 0);
    MIDI_SCALE = XML.getValue("MIDI_SCALE", 0);
    MIDI_XPOS = XML.getValue("MIDI_XPOS", 0);
    MIDI_YPOS = XML.getValue("MIDI_YPOS", 0);

    
    MIDI_SHADER_BYPASS = XML.getValue("MIDI_SHADER_BYPASS", 0);
    MIDI_SHADER_THRESH = XML.getValue("MIDI_SHADER_THRESH", 0);
    MIDI_SHADER_SOFT = XML.getValue("MIDI_SHADER_SOFT", 0);
    MIDI_SHADER_INVERT_ON = XML.getValue("MIDI_SHADER_INVERT_ON", 0);
    MIDI_SHADER_INVERT_OFF = XML.getValue("MIDI_SHADER_INVERT_OFF", 0);
    MIDI_SHADER_OPACITY = XML.getValue("MIDI_SHADER_OPACITY", 0);
    MIDI_SHADER_CAM_OPACITY = XML.getValue("MIDI_SHADER_CAM_OPACITY", 0);
    MIDI_SHADER_CAM_THRESH = XML.getValue("MIDI_SHADER_CAM_THRESH", 0);
    MIDI_SHADER_CAM_SOFT = XML.getValue("MIDI_SHADER_CAM_SOFT", 0);
    MIDI_SHADER_CAM_INVERT_ON = XML.getValue("MIDI_SHADER_CAM_INVERT_ON", 0);
    MIDI_SHADER_CAM_INVERT_OFF = XML.getValue("MIDI_SHADER_CAM_INVERT_OFF", 0);
    
    //default layer settings
    LAYER_SCALE = XML.getValue("LAYER_SCALE", 0.0);
    SHADER_THRESH = XML.getValue("SHADER_THRESH", 0.0);
    SHADER_INVERT = XML.getValue("SHADER_INVERT", 0.0);

    SHADER_SOFT = XML.getValue("SHADER_SOFT", 0.0);
    SHADER_OPACITY = XML.getValue("SHADER_OPACITY", 0.0);

    SHADER_ACTIVE = XML.getValue("SHADER_ACTIVE", 0);

    //how do you retrieve a string from xml?
    //SHADER_MASK = XML.getValue("SHADER_MASK");


    
    //ofSetLogLevel(OF_LOG_VERBOSE);

    
    //-------------------------------------
    //--SETUP Shader-----------------------
    //-------------------------------------
    
    // setup Shader
    #ifdef TARGET_OPENGLES
        camShader.load("shadersES2/livedraw");
    #else
        if(ofIsGLProgrammableRenderer()){
            camShader.load("shadersGL3/livedraw");
        }else{
            camShader.load("shadersGL2/livedraw");
        }
    #endif
    
    camShaderMask.load("img_mask_1080c.png");
    camShaderMask.resize(CAM_W,CAM_H);
    camShaderMaskTex = camShaderMask.getTexture();
    camThresh = SHADER_THRESH;
    camSoftness = SHADER_SOFT;
    camInvert = SHADER_INVERT;
    camOpacity = SHADER_OPACITY;
    camShaderActive = SHADER_ACTIVE;
    
    //shader loading is done in the vidLayer class's setup method
    
    
    //---------------------------------------
    //--SETUP CAMERA-------------------------
    //---------------------------------------

    
    #ifdef TARGET_LINUX_ARM
    
    if (USE_OFXCVPI==1) {
        ofLog(OF_LOG_NOTICE, "---------setting up grabber for ofxCvPi "+ofToString(CAM_W)+" "+ofToString(CAM_H));
        videoGrabber2.setup(CAM_W,CAM_H,true);//setup camera (w,h,color = true,gray = false);

    } else {
         //consoleListener.setup(this);
        omxCameraSettings.width = CAM_W;
        omxCameraSettings.height = CAM_H;
        omxCameraSettings.framerate = CAM_FPS;
        omxCameraSettings.enableTexture = true;
        omxCameraSettings.enablePixels = true;
        videoGrabber.setup(omxCameraSettings);
    }
    

    
    #elif defined(TARGET_OSX)
    
        vector<ofVideoDevice> devices = videoGrabber.listDevices();
        
        for(int i = 0; i < devices.size(); i++){
            if(devices[i].bAvailable){
                ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
            }else{
                ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
            }
        }
        
        videoGrabber.setDeviceID(CAM_DEV_ID);
        videoGrabber.setDesiredFrameRate(CAM_FPS);
        videoGrabber.initGrabber(CAM_W, CAM_H);
    
        //this doesnt work
        //videoGrabber.getTexture().setAlphaMask(camShaderMaskTex);

        ofSetVerticalSync(true);
        ofLog(OF_LOG_NOTICE, "---------setting up grabber like it's os X");

    
    #endif
    //-------------------------------------------
    //-------------------------------------------
    //-------------------------------------------
    
    
    
    //-------------------------------------
    //--SETUP MIDI-------------------------
    //-------------------------------------
    // print input ports to console
    midiIn.listPorts(); // via instance
    //ofxMidiIn::listPorts(); // via static as well
    
    // open port by number (you may need to change this)
    midiIn.openPort(MIDI_PORT);
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
    //-------------------------------------------
    //-------------------------------------------
    //-------------------------------------------


    
    //-------------------------------------------
    //--SETUP VID Layers-------------------------
    //-------------------------------------------
    
    //initialize video layers
    vidLayers.resize(LAYER_COUNT);
    
    for (int k=0;k<LAYER_COUNT;k++){
        //ofLog(OF_LOG_NOTICE, "Initializing layer " + ofToString(k)+ " with frame-count "+ ofToString(FRAME_COUNT));
        vidLayers[k].setup(k,FRAME_COUNT);
    }
    
    //-------------------------------------------
    //-------------------------------------------
    //-------------------------------------------
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);


    // gate latest camera frame, assign it to a texture
    
    #ifdef TARGET_LINUX_ARM
    if (USE_OFXCVPI==1) {
        //ofLog(OF_LOG_NOTICE, "---------update of grabber for RPi with ofxCvPiCamp");
        frame = videoGrabber2.grab();
        ofxCv::toOf(frame, thisImage);
        thisImage.update();
        nowTexture = thisImage.getTexture();
    } else {
        //ofLog(OF_LOG_NOTICE, "---------update of grabber for RPi with ofxRPiCameraVideoGrabber");
        nowTexture = videoGrabber.getTextureReference();
    }
    #elif defined(TARGET_OSX)
        if(videoGrabber.isFrameNew()){
            videoGrabber.update();
            nowTexture = videoGrabber.getTexture();
        }
    #endif

    
    
    // update video layers

    for (int k=0;k<LAYER_COUNT;k++){
        
        //first method: use textures
        //nowPixels = videoGrabber.getPixels();
        //vidLayers[k].update(nowPixels);
    
        //second method: use FBOs
        vidLayers[k].update2(nowTexture);
        
        }

    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    
    //drawCam(0,0);
    
    //draw processed cam
    if (SHADER_ACTIVE == 1) {
        //ofLog(OF_LOG_NOTICE, "---------drawing up within Shader");

        camShader.begin();
        camShader.setUniformTexture("maskTex", camShaderMask.getTexture(), 1);
        camShader.setUniform1f("thresh", camThresh);
        camShader.setUniform1f("softness", camSoftness);
        camShader.setUniform1f("invert", camInvert);
        camShader.setUniform1f("opacity", camOpacity);
        drawCam(0,0);
        camShader.end();
    } else {
        //ofLog(OF_LOG_NOTICE, "---------drawing up WITHOUT Shader");
        drawCam(0,0);
    }

    
    //itereate through all recording buffers
    for (int k=0;k<LAYER_COUNT;k++){
        vidLayers[k].draw(nowTexture);
        
    }
    
    drawMIDI();

    
}

//--------------------------------------------------------------

void ofApp::drawCam(int x, int y) {
    #ifdef TARGET_LINUX_ARM
        if (USE_OFXCVPI==1) {
            if (!frame.empty()) {
                //ofLog(OF_LOG_NOTICE, "---------drawing up grabber for RPi with ofxCvPiCamp");
                //this works
                //drawMat(frame,x,y);
                //thisImage.draw(x+THUMB_W,y,THUMB_W,THUMB_H);
                nowTexture.draw(0,0,CAM_W,CAM_H);

            } else {
                //ofLog(OF_LOG_NOTICE, "---------drawing found empty frame");
            }
            
        } else {
            //ofLog(OF_LOG_NOTICE, "---------drawing up grabber for RPi with ofxRPiCameraVideoGrabber");
            videoGrabber.draw(0,0,CAM_W,CAM_H);
        }
    #elif defined(TARGET_OSX)
        
        //ofLog(OF_LOG_NOTICE, "---------drawing up grabber like it's os X");
        videoGrabber.draw(0,0,CAM_W,CAM_H);
        
    #endif

}

//--------------------------------------------------------------
void ofApp::drawMIDI() {
    // midi input test
    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 400);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 410);
    text.str(""); // clear
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 440);
    text.str(""); // clear
    //ofDrawRectangle(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 460);
    text.str(""); // clear

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '1'){
        vidLayers[0].setState(1);
    }
    if(key == '2'){
        vidLayers[1].setState(1);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '1'){
        vidLayers[0].setState(2);
    }
    if(key == '2'){
        vidLayers[1].setState(2);
    }
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

//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------

/*
float ofApp::getCamThresh(){
    return(camThresh);
}

float ofApp::getCamSoftness(){
    return(camSoftness);
}

float ofApp::getCamInvert(){
    return (camInvert);
}
*/


//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    midiMessage = msg;
    
    //update midi messages
    
    //record on/off messages
    if (midiMessage.control >=MIDI_REC && midiMessage.control <=(MIDI_REC+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_REC;
        if (midiMessage.value==127) {vidLayers[layNum].setState(1);} else {vidLayers[layNum].setState(2);}
    }
    
    //opacity messages
    if (midiMessage.control >=MIDI_SHADER_OPACITY && midiMessage.control <=(MIDI_SHADER_OPACITY+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_SHADER_OPACITY;
        float scaleVal = 1.;
        scaleVal = Utils::scale(midiMessage.value,0,127,0,1);
        //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
        
        vidLayers[layNum].setOpacity(scaleVal);
    }

    //thresh messages
    if (midiMessage.control >=MIDI_SHADER_THRESH && midiMessage.control <=(MIDI_SHADER_THRESH+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_SHADER_THRESH;
        float scaleVal = 1.;
        scaleVal = Utils::scale(midiMessage.value,0,127,0,1);
        //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
        
        vidLayers[layNum].setThresh(scaleVal);
    }
    //invert messages

    if (midiMessage.control >=MIDI_SHADER_INVERT_ON && midiMessage.control <=(MIDI_SHADER_INVERT_ON+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_SHADER_INVERT_ON;
        if (midiMessage.value == 127) {vidLayers[layNum].setInvert(1.);}
    }
    
    if (midiMessage.control >=MIDI_SHADER_INVERT_OFF && midiMessage.control <=(MIDI_SHADER_INVERT_OFF+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_SHADER_INVERT_ON;
        if (midiMessage.value == 127) {vidLayers[layNum].setInvert(0.);}
    }
    
    //scale messages
    if (midiMessage.control >=MIDI_SCALE && midiMessage.control <=(MIDI_SCALE+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_SCALE;
        float scaleVal = 1.;
        if (midiMessage.value < 64) {  scaleVal = Utils::scale(midiMessage.value,0,64,0,1);} else { scaleVal = Utils::scale(midiMessage.value,64,127,1,5);}
        //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
        
        vidLayers[layNum].setScale(scaleVal);
    }
    

    
    //X position messages
    if (midiMessage.control >=MIDI_XPOS && midiMessage.control <=(MIDI_XPOS+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_XPOS ;
        float xPosition =Utils::scale(midiMessage.value,0,127,0,WINDOW_W);
        vidLayers[layNum].setXPos(xPosition);
    }
    
    //Y position messages
    if (midiMessage.control >=MIDI_YPOS && midiMessage.control <=(MIDI_YPOS+LAYER_COUNT) ) {
        int layNum = midiMessage.control - MIDI_YPOS;
        float yPosition = Utils::scale(midiMessage.value,0,127,WINDOW_H,0);
        
        vidLayers[layNum].setYPos(yPosition);
    }
    
    //Shader messages
    
    
    if (midiMessage.control ==MIDI_SHADER_BYPASS ) {
        camShaderActive = Utils::scale(midiMessage.value,0,127,0., 1.);
    }
    
    if (midiMessage.control ==MIDI_SHADER_CAM_OPACITY ) {
        camOpacity = Utils::scale(midiMessage.value,0,127,0., 1.);
    }
    
    if (midiMessage.control ==MIDI_SHADER_CAM_THRESH ) {
        camThresh = Utils::scale(midiMessage.value,0,127,0., 1.);
    }
    
    if (midiMessage.control ==MIDI_SHADER_CAM_SOFT ) {
        camSoftness = Utils::scale(midiMessage.value,0,127,0., 1.);
    }
    
    if (midiMessage.control ==MIDI_SHADER_CAM_INVERT_ON ) {
        if (midiMessage.value == 127) {camInvert = 1.;}
    }
    
    if (midiMessage.control ==MIDI_SHADER_CAM_INVERT_OFF ) {
        if (midiMessage.value == 127) {camInvert = 0.;}
    }
    
    
    
}


