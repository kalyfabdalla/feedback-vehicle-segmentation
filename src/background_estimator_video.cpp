#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include "background_estimator.hpp"

using namespace cv;

int main()
{
  PixelBackgroundModel model(0.001f,40,9.0f,0.97f,11.0f,0.05f,7.0f,4,1,0.5f,3,640,480,640*480,0);
  VideoCapture cap(0);
  Mat frame, background = Mat(480,640,CV_8UC1),values = Mat(480,640,CV_32FC3);
  cap >> frame;

  while(waitKey(1)!=27)
  {
    cap >> frame;
    model.backgroundModel(frame.data,background.data,values.data);
    imshow("camera",frame);
    imshow("background",background);
    imshow("values",values);
  }

  return 0;
}
