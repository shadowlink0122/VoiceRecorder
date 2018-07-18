#include <iostream>
#include <portaudio.h>
#include <thread>
#include <opencv/cv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "mouse.hpp"
#include "button.hpp"
#include "record.hpp"
#include "play.hpp"
#include "wave.hpp"
using namespace std;
using namespace cv;


int main(int argc,const char *argv[]){

  const int w = 600;
  const int h = 600;

  mouseParam event;
  Mat img = Mat::zeros(h,w,CV_8UC3);
  string show_name = "Recorder";

  recordButton(img,w,h);
  stopButton(img,w,h);
  playButton(img,w,h);
  exitButton(img,w,h);
  drawWave(img,w,h);
  namedWindow(show_name,CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

  // prot
  imshow(show_name,img);
  setMouseCallback(show_name,CallBackFunc,&event);

  thread trecord;
  paTestData recordData;
  recordData.recontinue = STOP;
  recordData.drawPlace = &img;

  try{

    while(1){
      
      waitKey(20);

      if(event.event == EVENT_LBUTTONDOWN){
        // printf("%d,%d : %d\n",event.x,event.y,recordData.recontinue);

        if(recordData.recontinue == STOP && click_record(event,w,h)){

          if(trecord.joinable())trecord.join();

          recordData.recontinue = RECORD;
          trecord = thread(callRecord,&recordData);
          puts("Record");

        }else if(recordData.recontinue == STOP && click_play(event,w,h)){

          if(recordData.recordedSamples == NULL)continue;

          if(trecord.joinable())trecord.join();

          recordData.recontinue = PLAY;
          trecord = thread(callPlay,&recordData);
          puts("Play");

        }else if(recordData.recontinue != STOP && click_stop(event,w,h)){

          recordData.recontinue = STOP;
          if(trecord.joinable())trecord.join();
          puts("Stop");

        }else if(click_exit(event,w,h)){

          // cout << trecord.joinable() << endl;
          recordData.recontinue = EXIT;
          if(trecord.joinable())trecord.join();
          puts("Exit");
          break;

        }
      }

    }

  }
  catch(...){

    // catch a "thread::join faild"
    cout << "Catch a Error" << endl;
    return -1;

  }

  return 0;
}