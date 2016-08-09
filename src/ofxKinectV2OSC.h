#pragma once
#include "ofMain.h"
#include "DataTransform/Logger.h"
#include "DataTransform/Mapper.h"
#include "Body/Skeleton.h"
#include "Draw/BodyRenderer.h"
#include "Recording.h"



class ofxKinectV2OSC {
public:
    void setup(int port, ofTrueTypeFont &_font);
    void update();
    void setSmoothing(SmoothingTechnique technique);
    void setFont(ofTrueTypeFont _font);
    vector<Skeleton>* getSkeletons();
    Skeleton* getNearestSkeleton();
    bool hasSkeletons();
    void parseOscMessages();
    void clearStaleSkeletons();
    void drawDebug();
    void toggleDebug();
    void setRecording(bool isRecording);
    
    void feedbackRecordedFile(){
        kinectRecorder.feedbackRecordedFile();
    }
    
    void feedbackRecordedFile(std::string path){
        kinectRecorder.feedbackRecordedFile(path);
    }
    
    bool isFeedbacking(){
        return kinectRecorder.isFeedback();
    }
    
protected:
    string buildDebugString();
    string parseLogger();
    
    ofxOscReceiver receiver;
    ofxOscMessage lastMessage;
    string lastParsedMessage;
    Logger logger;
    Mapper mapper;
    vector<Skeleton> skeletons;
    ofTrueTypeFont font;
    bool isDebugEnabled;
    
    Recording kinectRecorder;
    
    
};