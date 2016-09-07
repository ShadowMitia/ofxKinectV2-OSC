//
//  Recording.h
//  AnimataController
//
//  Created by Dimitri Belopopsky on 8/3/16.
//
//

#ifndef Recording_h
#define Recording_h

#include "ofMain.h"

// Copied over from liblo.
#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include <time.h>
#define JAN_1970 0x83aa7e80     /* 2208988800 1970 - 1900 in seconds */

class Recording : public ofThread {
  ofBuffer recording;
  std::string recordingDir;
  std::string lastRecordedFile;
  bool isPlaybacking;
    
  void threadedFunction() override {
    if (lastRecordedFile.size() > 0) {
      auto exe = ofToDataPath("./oscsend");
      lock();
      auto command = exe + " \"osc.udp://:12345\" " + lastRecordedFile;
      unlock();
      std::system(command.c_str());
    }
    isPlaybacking = false;
  }
        
public:
    
  Recording() {
    isPlaybacking = false;
  }
    
  bool isPlayback(){
    return isPlaybacking;
  }
        
  void setRecordingDir(std::string path){
    // TODO: check if directory exists
    // TODO: check if directory
    recordingDir = path;
  }
        
  bool isRecording = false;
        
  void addToRecording(ofxOscMessage m){
    std::string line_recorded;
    // Calculate time values, copied over from liblo
    // Uses casts for approximations
#if defined(WIN32) || defined(_MSC_VER)
    /*
      FILETIME is the time in units of 100 nsecs from 1601-Jan-01
      1601 and 1900 are 9435484800 seconds apart.
    */
    FILETIME ftime;
    GetSystemTimeAsFileTime(&ftime);
    uint32_t dtime =
      ((ftime.dwHighDateTime * 4294967296.e-7) - 9435484800.) +
      (ftime.dwLowDateTime * 1.e-7);
            
    uint32_t sec = (uint32_t) dtime;
    uint32_t frac = (uint32_t) ((dtime - t->sec) * 4294967296.);
#else
            
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint32_t sec = tv.tv_sec + JAN_1970;
    uint32_t frac = tv.tv_usec * 4294.967295;
#endif
    line_recorded += std::to_string(sec) + "." + std::to_string(frac);
    line_recorded += " ";
    line_recorded += m.getAddress();
    line_recorded += " ";
    for (auto i = 0; i < m.getNumArgs(); i++){
      if (m.getArgType(i) == OFXOSC_TYPE_STRING){
	line_recorded += "s";
      } else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT){
	line_recorded += "f";
      } else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT){
	line_recorded += "i";
      } else {
	line_recorded += "_";
      }
    }
    line_recorded += " ";
    for (auto i = 0; i < m.getNumArgs(); i++){
      if (m.getArgType(i) == OFXOSC_TYPE_FLOAT){
	line_recorded += std::to_string(m.getArgAsFloat(i));
	line_recorded += " ";
      } else if (m.getArgType(i) == OFXOSC_TYPE_STRING){
	line_recorded += "\"" + m.getArgAsString(i) + "\"";
	line_recorded += " ";
      } else {
	line_recorded += "UNKNOWN ";
      }
    }
    line_recorded += "\r\n";
    recording.append(line_recorded);
  }
        
  void setIsRecording(bool value) {
    if (isRecording == true && value == false) {
      auto filename = "kinect_osc_recording_" + std::to_string(ofGetUnixTime()) + ".log";
      bool res = ofBufferToFile(filename, recording);
      if (res == 0) {
	std::cout << "Couldn't write to file: " << filename << "";
	lastRecordedFile.clear();
      } else {
	lastRecordedFile = filename;
      }
      recording.clear();
    }
    isRecording = value;
  }
        
  void playbackRecordedFile(){
    if (!isPlaybacking){
      isPlaybacking = true;
      startThread();
    }
  }
        
  void playbackRecordedFile(std::string path){
    if (!isPlaybacking) {
      isPlaybacking = true;
      ofFile file(path);
      if (file.exists()) {
	lastRecordedFile = path;
	startThread();
      } else {
	std::cout << "File for playback not found" << '\n';
      }
    }
  }
};
        
#endif /* Recording_h */
