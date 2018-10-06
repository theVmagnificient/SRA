// Utils module 
// Programmer: KL1
// Date: 06.10.2018

#ifndef __UTILS_H
#define __UTILS_H

#include <fstream>
#include <Windows.h>
#include <opencv2/core/types_c.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>

// utilities namespace
namespace utils
{
  // check whether file exists
  inline BOOL file_exists( const std::string &file_name )
  {
    std::fstream file(file_name.c_str());
    return file.good();
  } /* End of 'file_exists' function */

  // detected object descriptor
  struct detected_object
  {
    cv::Rect box;         // bounding box 
    INT classId;          // class id
    FLOAT conf;           // confidence (0..1)
  };

  // Draw the predicted bounding box
  static VOID draw_pred( std::vector<detected_object> &Obj, cv::Mat &frame, const std::vector<std::string> &classes )
  {
    for (auto &i : Obj)
    {
      cv::rectangle(frame, i.box, cv::Scalar(255, 178, 50), 3);

      //Get the label for the class name and its confidence
      std::string label = cv::format("%.2f", i.conf);
      if (!classes.empty())
      {
        CV_Assert(i.classId < (INT)classes.size());
        label = classes[i.classId] + ":" + label;
      }

      //Display the label at the top of the bounding box
      int baseLine;
      cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
      i.box.y = MAX(i.box.y, labelSize.height);
      cv::rectangle(frame, cv::Point(i.box.x, i.box.y - round(1.5 * labelSize.height)), 
        cv::Point(i.box.x + round(1.5 * labelSize.width), i.box.y + baseLine), 
        cv::Scalar(255, 255, 255), cv::FILLED);
      cv::putText(frame, label, cv::Point(i.box.x, i.box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0), 1);
    } 
  } /* End of 'draw_pred' function */
} /* end of 'utils' namespace */

#endif