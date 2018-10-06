// Cnn config module
// Programmer: KL1
// Date: 05.10.2018

#ifndef __DNN_MODEL_H
#define __DNN_MODEL_H

#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>

#include "utils.h"


// dnn descriptor and initializer
class dnn_model
{
private:
  const std::string &configPath;   // path to .cfg file 
  const std::string &weightsPath;  // path to wights file
  const std::string &classPath;    // path to files with classes names


  FLOAT confThreshold;        // confidence threshold
  FLOAT nmsThreshold;         // non-maximum suppression threshold
 



protected:
  cv::dnn::Net *YoloNet;             // yolo net instance
  INT inpWidth, inpHeight;           // size of network's input image (%13 == 0)
  std::vector<std::string> classes;  // names of recognised classes
  std::vector<utils::detected_object> processedObjects;  // objects after postprocess

public:

  // class constructor
  dnn_model(const std::string &configPath, const std::string &weightsPath, const std::string &classPath,
    FLOAT confThreshold = .5, FLOAT nmsThreshold = .4, INT inpwidth = 416, INT inpwheight = 416) :
    configPath(configPath), weightsPath(weightsPath), classPath(classPath), confThreshold(confThreshold),
    nmsThreshold(nmsThreshold), inpWidth(inpwidth), inpHeight(inpwheight)
  {
    // check whether all neccesary files exist
    if (!(utils::file_exists(configPath) && utils::file_exists(weightsPath) &&
      utils::file_exists(classPath)))
    {
      std::cerr << "Cannot open config files";
      exit(0);
    }

    // push classes names
    std::ifstream ifs(classPath.c_str());
    std::string line;
    while (getline(ifs, line))
      classes.push_back(line);

    YoloNet = new cv::dnn::Net();
    *YoloNet = cv::dnn::readNetFromDarknet(configPath, weightsPath);
  } /* End of 'dnn_model' function */


  // set backend for neural network
  VOID set_preferable_backend(cv::dnn::dnn4_v20180903::Backend Mode = cv::dnn::DNN_BACKEND_OPENCV)
  {
    YoloNet->setPreferableBackend(Mode);
  } /* End of 'set_preferable_backend' function */

  // set preferable target for neural network
  VOID set_preferable_target(cv::dnn::dnn4_v20180903::Target Target = cv::dnn::DNN_TARGET_OPENCL_FP16)
  {
    YoloNet->setPreferableTarget(Target);
  } /* End of 'set_preferable_target' function */

  // remove the bounding boxes with low confidence using non-maxima suppression
  VOID postprocess(cv::Mat &frame, const std::vector<cv::Mat> &outs);

  // Get the names of the output layers
  std::vector<std::string> get_outputs_names( VOID );

  
}; /* End of 'dnn_model' class */


#endif