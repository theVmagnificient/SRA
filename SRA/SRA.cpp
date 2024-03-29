// SRA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//#include "YoloDetection/detector.h"
#include "DetectorPool/det_pool.h"
#include "Camera/camera.h"

int main()
{
  /*detector det("bin//net//yolov3_416.cfg", 
    "bin//net//yolov3.weights", 
    "bin//tmp.names");
*/  
  det_pool pool("bin//net//yolov3_416.cfg",
    "bin//net//yolov3.weights",
    "bin//tmp.names");
  /*det.set_preferable_backend();
  det.set_preferable_target();
*/  
  pool.set_backend();
  pool.set_target();

 /* cv::VideoCapture cap;
  cv::Mat frame, blob;
*/
  camera cam("D:\\V\\test1.avi");
  camera cam1("D:\\V\\kirill.avi");
  camera cam2("D:\\V\\test.mp4");
  //cap.open("D:\\V\\test1.avi");

  // Create a window
  static const std::string kWinName = "First", n = "Second", k = "Third";
  cv::namedWindow(kWinName, cv::WINDOW_NORMAL);
  cv::namedWindow(n, cv::WINDOW_NORMAL);
  cv::namedWindow(k, cv::WINDOW_NORMAL);

  pool << &cam << &cam1 << &cam2;

  // Process frames.
  while (cv::waitKey(1) < 0)
  {
    pool.camera_handler();
    pool.draw_predictions();
    pool.output_from_camera(0, "First");
    pool.output_from_camera(1, "Second");
    pool.output_from_camera(2, "Third");
    // get frame from the video
  //  cap >> frame;

    // Stop the program if reached end of video
    /*if (frame.empty()) {
      std::cout << "Done processing !!!" << std::endl;
      cv::waitKey(3000);
      break;
    }*/

    /*det << frame;
    det.net_forward();
    det.draw_predictions();
    cv::imshow(kWinName, det.get_cur_frame());*/
  }


}

