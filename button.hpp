
#pragma once

#include "button_params.hpp"

using namespace cv;
using namespace std;

void recordButton(InputOutputArray &img,int w,int h){

  rectangle(img, Point(w*0.1-20,h*0.1-20), Point(w*0.5-20, h*0.25-20), Scalar(0,255,0), 5, 8);
  putText(img, "Record", Point(w*0.1-10,h*0.175), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,200), 3, CV_AA);

}

void stopButton(InputOutputArray &img,int w,int h){

  rectangle(img, Point(w*0.5+20,h*0.1-20), Point(w*0.9+20, h*0.25-20), Scalar(0,255,0), 5, 7);
  putText(img, "Stop", Point(w*0.6+10,h*0.175), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,200), 3, CV_AA);

}

void playButton(InputOutputArray &img,int w,int h){

  rectangle(img, Point(w*0.1-20,h*0.3-20), Point(w*0.5-20, h*0.45-20), Scalar(0,255,0), 5, 8);
  putText(img, "Play", Point(w*0.15,h*0.375), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,200), 3, CV_AA);

}

void exitButton(InputOutputArray &img,int w,int h){

  rectangle(img, Point(w*0.5+20,h*0.3-20), Point(w*0.9+20, h*0.45-20), Scalar(0,255,0), 5, 8);
  putText(img, "Exit", Point(w*0.6+20,h*0.375), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,200), 3, CV_AA);

}
