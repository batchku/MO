//
//  vidLayer.cpp
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/16/17.
//
//

#include "ofApp.h"
#include "vidLayer.h"



void vidLayer::setup(int thisID, int bufSize){
    //array of textures
    //vidFrames.resize(bufSize);
    
    //array of FBOs
    vidFrames2.resize(bufSize);
    
    myID = thisID;
    state = 0;                //stop all playheads
    playHead = 0;             //set all playheads to 0
    recHead = 0;              //set all playheads to 0
    recCount = 0;             //set all record framecounts to 0
    recMax = bufSize;
    opacity = 1.;
    scale = LAYER_SCALE;
    thresh = SHADER_THRESH;
    invert = SHADER_INVERT;
    softness = SHADER_SOFT;
    opacity = SHADER_OPACITY;
    shaderActive = SHADER_ACTIVE;
    //x = THUMB_W * scale * myID +THUMB_W * scale/2;
    //y = THUMB_H * scale + THUMB_H/2;
    x = WINDOW_W/2;
    y = WINDOW_H/2;
    
    //does this allocate the FBO?
    for (int k = 0;k<bufSize;k++){
        vidFrames2[k].allocate(CAM_W, CAM_H);
        vidFrames2[k].setAnchorPercent(0.5, 0.5);
    }

    
    // setup Shader
    #ifdef TARGET_OPENGLES
        shader.load("shadersES2/livedraw");
    #else
        if(ofIsGLProgrammableRenderer()){
            shader.load("shadersGL3/livedraw");
        }else{
            shader.load("shadersGL2/livedraw");
        }
    #endif
    
    imageMask.load("img_mask_1080c.png");
    imageMask.resize(CAM_W,CAM_H);
    

}

void vidLayer::draw(ofTexture thisTexture){

    if (state==2 && recCount>0) {
        //ofLog(OF_LOG_NOTICE, "playing at " + ofToString(myID)+ " at "+ ofToString(playHead));
        
        if (shaderActive == 1) {
            shader.begin();
            shader.setUniformTexture("maskTex", imageMask.getTexture(), 1);
            shader.setUniform1f("thresh", thresh);
            shader.setUniform1f("softness", softness);
            shader.setUniform1f("invert", invert);
            shader.setUniform1f("opacity", opacity);
            vidFrames2[playHead].draw(x, y , CAM_W * scale, CAM_H * scale);
     
            shader.end();
        } else {
            vidFrames2[playHead].draw(x, y , CAM_W * scale, CAM_H * scale);
        }

        
         } else if (state == 1) {
        
        //while recording, draw live feed
             ofTexture livefeed;
             livefeed = thisTexture;
             livefeed.setAnchorPercent(0.5, 0.5);
         if (shaderActive == 1) {
             shader.begin();
             shader.setUniformTexture("maskTex", imageMask.getTexture(), 1);
             shader.setUniform1f("thresh", thresh);
             shader.setUniform1f("softness", softness);
             shader.setUniform1f("invert", invert);
             
             livefeed.draw(x, y, CAM_W * scale, CAM_H * scale);
             shader.end();
         } else {

             livefeed.draw(x, y , CAM_W * scale, CAM_H * scale);
         }
     
             
             
    }
    

}

/*
void vidLayer::update(ofTexture thisTexture){
    ofLog(OF_LOG_NOTICE, "Running update for layer " + ofToString(myID));
    if (state == 1) {
        ofLog(OF_LOG_NOTICE, "recording update " + ofToString(myID)+ " at "+ ofToString(recHead));
        //vidFrames[recHead].allocate( thisPixels);
        vidFrames[recHead] = thisTexture;

        //advance record head
        recHead = (recHead + 1 ) % recMax;
        
    } else if (state == 2) {
        //advance playhead
        playHead = (playHead + 1) % recCount;
    }
}
*/

void vidLayer::update2(ofTexture theTexture){
    if (state == 1) {
        //ofLog(OF_LOG_NOTICE, "recording update2 " + ofToString(myID)+ " at "+ ofToString(recHead));
        
        
        //draw texture onto FBO
        vidFrames2[recHead].begin();
        theTexture.draw(0,0);
        vidFrames2[recHead].end();
        
        
        //advance record head
        recHead = (recHead + 1 ) % recMax;
        
    } else if (state == 2) {
        //advance playhead
        playHead = (playHead + 1) % recCount;
    }
}

void vidLayer::setState(int thisState){
    //if we're not already recording, start recording
    
    switch (thisState) {
        //stop
        case 0:
        //record
        case 1:
            if (state != thisState) {
                
                //match shader params to camera input's shader
                
                //try extern
                
                thresh = camThresh;
                softness = camSoftness;
                invert = camInvert;
                
                
                
                //ofLog(OF_LOG_NOTICE, "RECORD: State for layer " + ofToString(myID) + " is " + ofToString(state));
                //ofLog(OF_LOG_NOTICE, "Starting recording on layer " + ofToString(myID));
                state=1;            //set state to 1
                recHead = 0;        //reset record head
            }
        //play
        case 2:
            if (state != thisState) {
                ofLog(OF_LOG_NOTICE, "PLAY: State for layer " + ofToString(myID) + " is " + ofToString(state));
                ofLog(OF_LOG_NOTICE, "Starting playing on layer " + ofToString(myID));
                state=2;
                recCount = recHead;     //set record frame count to last frame recorded
                playHead = 0;           //set playhead to 0
            }
    }
    
}

/*
int vidLayer::setFrame(int thisFrame, ofPixels thisPixels) {
    vidFrames[thisFrame].allocate( thisPixels);
}
 */

int vidLayer::getState() {
    return(state);
}

void vidLayer::setSpeed(float speed){
    
}

void vidLayer::setShaderParams(float thisThresh, float thisSoftness, float thisInvert){
    thresh = thisThresh;
    softness = thisSoftness;
    invert = thisInvert;
}

void vidLayer::setScale(float thisScale){
    scale =thisScale;
}

void vidLayer::setOpacity(float thisOpacity){
    opacity =thisOpacity;
}

void vidLayer::setThresh(float thisThresh){
    thresh =thisThresh;
}

void vidLayer::setInvert(float thisInvert){
    invert =thisInvert;
}

void vidLayer::setPos(int positionX, int positionY){

    x = positionX ;
    y = positionY;
    //ofLog(OF_LOG_NOTICE, "---setting position of " + ofToString(myID) + " to  " + ofToString(x) + " " + ofToString(y));

}

void vidLayer::setXPos(int positionX){
    x = positionX ;
    //ofLog(OF_LOG_NOTICE, "---setting X position of " + ofToString(myID) + " to  " + ofToString(x));


}

void vidLayer::setYPos(int positionY){

    y = positionY;
    //ofLog(OF_LOG_NOTICE, "---setting X position of " + ofToString(myID) + " to  " + ofToString(y));

}
