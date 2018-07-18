
#pragma once

#include <opencv/cv.hpp>
#include <portaudio.h>

void drawWave(InputOutputArray &img,int w,int h){

  rectangle(img, Point(w*0.1-20,h*0.5-20), Point(w*0.9+20, h*0.9-20), Scalar(255,255,0), 2, 2);

  line(img, Point(w*0.1-20,((h*0.5-20) + (h*0.9-20))/2), Point(w*0.9+20, ((h*0.5-20) + (h*0.9-20))/2), Scalar(255,255,255), 2, 2);
  
  putText(img, "Record", Point(w*0.1-10,h*0.175), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,200), 3, CV_AA);

}