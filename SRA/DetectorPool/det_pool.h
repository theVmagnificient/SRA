// Detector pool class file
// Programmer: KL1
// Date: 19.10.2018

#ifndef __DET_POOL_H
#define __DET_POOL_H

#include <thread>
#include <algorithm>
#include <array>
#include <future>      
#include <thread> 

#include "../YoloDetection/detector.h"
#include "../Camera/camera.h"
#include "../YoloDetection/utils.h"

// detector pool class
class det_pool
{
private:
  std::vector<detector *> detectors;       // detectors vector with indexes to handled camera
  std::vector<camera *> cameras;           // cameras vector

public:

  // class constructor
  det_pool(const std::string &configPath, const std::string &weightsPath, const std::string &classPath,
    INT inpwidth = 416, INT inpheight = 416, FLOAT confThreshold = .5, FLOAT nmsThreshold = .4)
  {
    for (SIZE_T i = 0; i < std::thread::hardware_concurrency(); i++)
      detectors.push_back(new detector(configPath, weightsPath, classPath, 
        inpwidth, inpheight, confThreshold, nmsThreshold)); // init detector pool vector
  }
  
  // add new camera to vector
  det_pool & operator<<( camera *cam )
  {
    static size_t num = 0;
    if (cam == nullptr)
      return *this;
    if (std::find(cameras.begin(), cameras.end(), cam) != cameras.end())
    {
      std::cerr << "Camera already exists";
      return *this;
    }
    cam->index = num++;
    cameras.push_back(cam);

    return *this;
  }

  // main cycle function 
  VOID camera_handler(VOID);

  // set backend for detectors
  VOID set_backend(cv::dnn::dnn4_v20180903::Backend Mode = cv::dnn::DNN_BACKEND_OPENCV)
  {
    for (auto &i : detectors)
      i->set_preferable_backend(Mode);
  } /* End of 'set_backend' function */

  // set preferable target for neural network
  VOID set_target(cv::dnn::dnn4_v20180903::Target Target = cv::dnn::DNN_TARGET_OPENCL_FP16)
  {
    for (auto &i : detectors)
      i->set_preferable_target(Target);
  } /* End of 'set_target' function */

  VOID draw_predictions()
  {
    std::for_each(cameras.begin(), cameras.end(), [&]( camera *c ){
      c->draw_predictions(detectors[0]->get_classes_names());
    });
  } /* End of 'draw_predictions' function */

  // output frame from specific camera
  VOID output_from_camera( UINT n = 0, const std::string &kWinName = "First" )
  {
    cv::imshow(kWinName, cameras[n]->GetCurFrame());
  }
  // class destructor
  ~det_pool()
  {
    for (auto i = detectors.begin(); i != detectors.end();)
      i = detectors.erase(i);
  }
};

#endif