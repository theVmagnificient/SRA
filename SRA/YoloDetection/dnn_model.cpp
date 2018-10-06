// Cnn config module
// Programmer: KL1
// Date: 05.10.2018

#include "dnn_model.h"

// remove the bounding boxes with low confidence using non-maxima suppression
// and save processed obects to dnn_model::processedObjects
VOID dnn_model::postprocess( cv::Mat &frame, const std::vector<cv::Mat> &outs )
{
  std::vector<int> classIds;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;

  for (size_t i = 0; i < outs.size(); ++i)
  {
    // Scan through all the bounding boxes output from the network and keep only the
    // ones with high confidence scores. Assign the box's class label as the class
    // with the highest score for the box.
    FLOAT *data = (FLOAT *)outs[i].data;
    for (INT j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
    {
      cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
      cv::Point classIdPoint;
      DOUBLE confidence;

      // Get the value and location of the maximum score
      minMaxLoc(scores, nullptr, &confidence, 0, &classIdPoint);

      if (confidence > confThreshold)
      {
        INT centerX = (INT)(data[0] * frame.cols);
        INT centerY = (INT)(data[1] * frame.rows);
        INT width = (INT)(data[2] * frame.cols);
        INT height = (INT)(data[3] * frame.rows);
        INT left = centerX - width / 2;
        INT top = centerY - height / 2;

        classIds.push_back(classIdPoint.x);
        confidences.push_back((FLOAT)confidence);
        boxes.push_back(cv::Rect(left, top, width, height));
      }
    }
  }

  // process non maxima suppresion to delete overlapping boxes
  // and leave only ones with high confidences
  std::vector<INT> indices;
  cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

  // clear objects from prev step
  processedObjects.clear();
  for (size_t i = 0; i < indices.size(); ++i)
  {
    int idx = indices[i];
    cv::Rect box = boxes[idx];
    //drawPred(classIds[idx], confidences[idx], box.x, box.y,
    //  box.x + box.width, box.y + box.height, frame);

    utils::detected_object obj;
    obj.box = box;
    obj.classId = classIds[idx];
    obj.conf = confidences[idx];

    
    processedObjects.push_back(obj);  // save processed boxes to objects
  }
} /* End of 'postprocess' function */

// Get the names of the output layers
std::vector<std::string> dnn_model::get_outputs_names( VOID )
{
  static std::vector<std::string> names;
  if (names.empty())
  {
    // get the indices of the output layers, i.e. the layers with unconnected outputs
    std::vector<int> outLayers = YoloNet->getUnconnectedOutLayers();

    //get the names of all the layers in the network
    std::vector<std::string> layersNames = YoloNet->getLayerNames();

    // Get the names of the output layers in names
    names.resize(outLayers.size());
    for (size_t i = 0; i < outLayers.size(); ++i)
      names[i] = layersNames[outLayers[i] - 1];
  }
  return names;
} /* End of 'dnn_model::getOutputsNames' function */