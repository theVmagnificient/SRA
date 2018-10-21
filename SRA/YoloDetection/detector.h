// Module to process dnn_model 
// Programmer: KL1
// Date: 05.10.2018

#ifndef __DETECTOR_H
#define __DETECTOR_H

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>

#include "dnn_model.h"
#include "../Camera/camera.h"

#include <string>

// main detection class
class detector : public dnn_model 
{
private:

  // flag for current net status
  BOOL isWorking = FALSE;

  // current blob to process
  cv::Mat curBlob, curFrame;

  // add new camera to process through net
  VOID set_input( camera &cam )
  {
    //curFrame = cam.GetNextFrame();

    cv::dnn::blobFromImage(cam.GetNextFrame(), curBlob, 1 / 255.0, cvSize(inpWidth, inpHeight),
      cv::Scalar(0, 0, 0), TRUE, FALSE);

    YoloNet->setInput(curBlob);
  } /* End of 'operator<<' function */

  // forward pass of neural network
  // return: vector of detected objects
  VOID net_forward( camera &cam )
  {
    std::vector<cv::Mat> outs;
    try
    {
      YoloNet->forward(outs, get_outputs_names());
    }
    catch (cv::Exception & e)
    {
      std::cerr << e.msg << std::endl; // output exception message
    }

    postprocess(cam, outs);

  } /* End of 'net_forward' function */

public:
  detector(const std::string &configPath, const std::string &weightsPath, const std::string &classPath,
    INT inpwidth = 416, INT inpheight = 416, FLOAT confThreshold = .5, FLOAT nmsThreshold = .4) :
    dnn_model(configPath, weightsPath, classPath, confThreshold, nmsThreshold, inpwidth, inpheight)
  {
  }

  VOID run_pipeline( camera &cam )
  {
    isWorking = TRUE;
    set_input(cam);
    net_forward(cam);
    isWorking = FALSE;
  }

  BOOL get_net_status( VOID ) const
  {
    return isWorking;
  }

  cv::Mat get_cur_frame() const
  {
    return curFrame;
  }
  
  // draw predictions on current frame
  VOID draw_predictions()
  {
    utils::draw_pred(processedObjects, curFrame, classes);
  } /* End of 'draw_predictions' function */

}; /* End of 'detector' class*/


#endif