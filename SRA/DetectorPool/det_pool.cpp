// Detector pool multithreading module
// Programmer: KL1
// Date: 21.10.2018

#include "det_pool.h"


// main cycle handler
VOID det_pool::camera_handler( VOID )
{
  std::vector<std::future<VOID>> fut;

  if (detectors.size() > cameras.size())
  {
    for (INT i = 0; i < cameras.size(); i++)
    {
       // push new future from package task
      try 
      {
        fut.push_back(std::async(std::launch::async, &detector::run_pipeline, 
          detectors[i], std::ref(*cameras[i])));
      }
      catch (std::exception &e)
      {
        std::cerr << e.what();
        // if connection fails then delete current camera
        if (e.what() == "No frame")
        {
          cameras.erase(cameras.begin() + i);
          i--;
        }
      }
    }

    // check all futures to get ready
    for (auto &i : fut)
      i.get();
  }
  fut.clear();
}