
#pragma once

#include <opencv/cv.hpp>
#include <portaudio.h>


// ここに波形を表示したい
void drawWaveSpace(InputOutputArray &img,int w,int h){

  rectangle(img, Point(w*0.1-20,h*0.5-20), Point(w*0.9+20, h*0.9-20), Scalar(255,255,0), 2, 2);
  line(img, Point(w*0.1-20,((h*0.5-20) + (h*0.9-20))/2), Point(w*0.9+20, ((h*0.5-20) + (h*0.9-20))/2), Scalar(255,255,255), 2, 2);
  putText(img, "Record", Point(w*0.1-10,h*0.175), FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,200), 3, CV_AA);

}

// drawWave
void waves(paTestData *recorded){
  paTestData *data = recorded;

  Mat waves = Mat::zeros(250,500,CV_8UC1);
  line(waves, Point(0,250/2), Point(500,250/2), Scalar(255,255,255), 2, 2);

  for(long long i = 1;i < data->recordFrameIndex;i++){

    double x0 = 500*(i-1)/data->recordFrameIndex;
    double y0 = 250/2 + data->recordedSamples[i-1]*1000;
    double x1 = 500*i/data->recordFrameIndex;
    double y1 = 250/2 + data->recordedSamples[i]*1000;

    if(y0 > 250)y0 = 250;
    else if(y0 < 0)y0 = 0;

    if(y1 > 250)y1 = 250;
    else if(y1 < 0)y1 = 0;

    line(waves, Point(x0,y0), Point(x1,y1), Scalar(255,255,255), 1, 8);

  }

  // pasteWaves
  imshow("waves",waves);
}

// 使わない
void pasteWaves(){
  int w = 40;
  int h = 275;
}
