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

#include <string>

// main detection class
class detector : public dnn_model 
{
private:

  // flag for current net status
  BOOL isWorking = FALSE;

  // current blob to process
  cv::Mat curBlob, curFrame;

public:
  detector(const std::string &configPath, const std::string &weightsPath, const std::string &classPath,
    FLOAT confThreshold = .5, FLOAT nmsThreshold = .4, INT inpwidth = 416, INT inpheight = 416) :
    dnn_model(configPath, weightsPath, classPath, confThreshold, nmsThreshold, inpwidth, inpheight)
  {
  }

  // add new frame to process through net
  VOID operator<<( const cv::Mat &frame )
  {
    if (isWorking)
      return;

    curFrame = frame;
    cv::dnn::blobFromImage(curFrame, curBlob, 1 / 255.0, cvSize(inpWidth, inpHeight), 
      cv::Scalar(0, 0, 0), TRUE, FALSE);

    YoloNet->setInput(curBlob);
  } /* End of 'operator<<' function */

  // forward pass of neural network
  VOID net_forward( VOID )
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
    

    postprocess(curFrame, outs);
  } /* End of 'net_forward' function */

  BOOL get_net_status( VOID )
  {
    return isWorking;
  }

  cv::Mat get_cur_frame()
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