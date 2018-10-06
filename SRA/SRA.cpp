// SRA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "YoloDetection/detector.h"

int main()
{
  detector det("bin//yolov3.cfg", 
    "bin//yolov3.weights", 
    "bin//tmp.names");

  det.set_preferable_backend();
  det.set_preferable_target();

  cv::VideoCapture cap;
  cv::Mat frame, blob;

  cap.open("D:\\V\\test1.avi");

  // Create a window
  static const std::string kWinName = "Deep learning object detection in OpenCV";
  cv::namedWindow(kWinName, cv::WINDOW_NORMAL);

  // Process frames.
  while (cv::waitKey(1) < 0)
  {
    // get frame from the video
    cap >> frame;

    // Stop the program if reached end of video
    if (frame.empty()) {
      std::cout << "Done processing !!!" << std::endl;
      cv::waitKey(3000);
      break;
    }

    det << frame;
    det.net_forward();
    det.draw_predictions();
    cv::imshow(kWinName, det.get_cur_frame());
  }


}

