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
    std::vector<std::string> tmpMessages;
    
public:
    ofThreadChannel<ofxOscMessage> incomingMessages;
    
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
        for (int i = 0; i < m.getNumArgs(); i++){
            if (m.getArgType(i) == OFXOSC_TYPE_STRING){
                line_recorded += "s";
            } if (m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                line_recorded += "f";
            }
        }
        line_recorded += " ";
        if (m.getNumArgs() == 4) {
            line_recorded += std::to_string(m.getArgAsFloat(0));
            line_recorded += " ";
            line_recorded += std::to_string(m.getArgAsFloat(1));
            line_recorded += " ";
            line_recorded += std::to_string(m.getArgAsFloat(2));
            line_recorded += " ";
            line_recorded += "\"" + m.getArgAsString(3) + "\"";
            line_recorded += " ";
        } else if (m.getNumArgs() == 2) {
            line_recorded += "\"" + m.getArgAsString(0) + "\"";
            line_recorded += " ";
            line_recorded += "\"" + m.getArgAsString(1) + "\"";
            line_recorded += " ";
        } else {
            line_recorded += "ERROR: unknown osc message: " + m.getAddress();
        }
        line_recorded += "\r\n";
        cout << line_recorded << endl;
        tmpMessages.push_back(line_recorded);
        recording.append(line_recorded);
    }
    
    void split(const string& s, char delim, vector<string>& elems){
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
    }
    
    void setIsRecording(bool value){
        if (isRecording == true && value == false){
            auto filename = "kinect_osc_recording_" + std::to_string(ofGetUnixTime()) + ".log";
            bool res = ofBufferToFile(filename, recording);
            if (res == 0){
                std::cout << "Couldn't write to file: " << filename << "";
            }
            ofBuffer tmp = ofBufferFromFile(filename);
        
            auto data = std::string(tmp.getData());
            vector<string> lines;
            auto delim = '\n';

            split(data, delim, lines);
            /*
            for (int i = 0; i < tmp.size(); i++){
                if (!(lines[i]+"\n" == tmpMessages[i])){
                    cout << "PROBLEM!!!!!!!" << endl;
                    cout << i << endl;
                    cout << lines[i];
                    cout << tmpMessages[i];
                }
            }
             */
            cout << lines.size() << " " << tmpMessages.size() << endl;
            
        }
        isRecording = value;
    }

};

#endif /* Recording_h */
