// Camera instance class
// Programmer: KL1
// Date: 19.10.2018

#ifndef __CAMERA_H
#define __CAMERA_H

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>

#include <vector>
#include <windows.h>
#include <string>
#include <iostream>

#include "../Positioning/positioning.h"
#include "../YoloDetection/utils.h"

class camera : public positioning
{
  friend class det_pool;
private:
  std::vector<std::pair<double, double>> coords;   // people positions for current camera
  std::vector<utils::detected_object> det_objects; // detected objects vector
  
  const std::string &ip;    // camera ip

  cv::VideoCapture cap;   // videocapture instance
  cv::Mat frame;          // frame matrix

  size_t index;

public:
  
  camera( const std::string &ip ) : ip(ip)
  {
    if (!cap.open(ip))
    {
      throw "Cannot open " + ip;
      return;
    }
  }

  camera(INT i = 0) : ip(ip)
  {
    if (!cap.open(i))
    {
      throw "Cannot open usb camera";
      return;
    }
  }

  camera & operator<<( const utils::detected_object &obj )
  {
    det_objects.push_back(obj);
    return *this;
  }

  BOOL operator==( camera &cam )
  {
    return (this->ip == cam.ip);
  }

  BOOL operator==( camera *cam)
  {
    return (this->ip == cam->ip);
  }

  VOID clear_obj()
  {
    det_objects.clear();
  }

  cv::Mat & GetNextFrame()
  {
    try
    {
      cap >> frame;
      return frame;
    }
    catch (const std::exception &)
    {
      throw "No frame";
    }
  }

  cv::Mat & GetCurFrame()
  {
    return frame;
  }

  VOID BBox2Coords( VOID )
  {
    coords.clear();
    std::for_each(det_objects.begin(), det_objects.end(), [&](utils::detected_object &i) {
      coords.push_back(this->GetPosition(std::make_pair(i.box.x + i.box.width / 2, i.box.y), 1.75));
    });
  }

  VOID draw_predictions( const std::vector<std::string> &classes )
  {
    utils::draw_pred(det_objects, frame, classes);
  } /* End of 'draw_predictions' function */
};

#endif