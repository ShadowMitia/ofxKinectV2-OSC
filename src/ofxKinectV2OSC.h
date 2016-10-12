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
  std::vector<Skeleton>* getSkeletons();
  Skeleton* getNearestSkeleton();
  bool hasSkeletons();
  void parseOscMessages();
  void clearStaleSkeletons();
  void drawDebug();
  void toggleDebug();
  void setRecording(bool isRecording);
    
  void playbackRecordedFile(){
    kinectRecorder.playbackRecordedFile();
  }
    
  void playbackRecordedFile(std::string path){
    kinectRecorder.playbackRecordedFile(path);
  }
    
  bool isPlaybacking(){
    return kinectRecorder.isPlayback();
  }
    
  std::string getRecordedFile(){
    return kinectRecorder.getLastRecordedFile();
  }
    
protected:
  std::string buildDebugString();
  std::string parseLogger();
    
  ofxOscReceiver receiver;
  ofxOscMessage lastMessage;
  std::string lastParsedMessage;
  Logger logger;
  Mapper mapper;
  std::vector<Skeleton> skeletons;
  ofTrueTypeFont font;
  bool isDebugEnabled;
    
  Recording kinectRecorder;
    
    
};
