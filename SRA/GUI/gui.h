// GUI module
// Programmer: VP1
// Date: 05.11.2018

#pragma once
#include <string>
#include <Windows.h>
#include <cstdio>
#include <opencv2/core.hpp>
#include "../Positioning/positioning.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


class gui
{
  std::vector<std::pair<INT, INT>> Points; // array of points to show
  cv::Mat Frame;                           // Frame to show
  size_t W, H;                             // Size of window
  std::vector<ZONE> Zones;                 // All zones to Draw

  /* Convert point to Screen coordinates */
  cv::Point ConvertPoint(cv::Point P)
  {
    int x = P.x;
    int y = P.y;
    return cv::Point((x * 100) + W / 2 - 50, (-y * 100) + H / 2 + 50);
  }
public:
  // Gui constructor.
  // ARGUMENTS - size of GUI window
  gui( INT Wd, INT Hd ) : Frame(Hd, Wd, CV_8UC3, cv::Scalar::all(0)), W(Wd), H(Hd)
  {
    cv::namedWindow("Position View", cv::WINDOW_AUTOSIZE);
  }

  /* Load all Zones */
  VOID LoadZones( const std::string &FName )
  {
    FILE *F;
    if ((F = fopen(FName.c_str(), "r")) != NULL)
    {
      CHAR Buf[600] = {0};
      ZONE tmp;
      while (fgets(Buf, 599, F))
      {
        if (Buf[0] == 'z')
        {
          if (tmp.Corners.size() != 0)
          {
            tmp.Color = cv::Scalar(255.0 * rand() / RAND_MAX, 255.0 * rand() / RAND_MAX, 255.0 * rand() / RAND_MAX);
            Zones.push_back(tmp);
            tmp = ZONE();
          }
        }
        else
        {
          int x = 0, y = 0;
          sscanf(Buf, "%i %i", &x, &y);
          tmp.Corners.push_back(ConvertPoint(cv::Point(x, y)));
        }
      }
      if (tmp.Corners.size() != 0)
      {
        tmp.Color = cv::Scalar(255.0 * rand() / RAND_MAX, 255.0 * rand() / RAND_MAX, 255.0 * rand() / RAND_MAX);
        Zones.push_back(tmp);
      }
      fclose(F);
    }
  }

  // Set points to show
  // ARGUMENTS - position of points (std::vector<std::pair<INT, INT>>)  
  VOID SetPoints( const std::vector<std::pair<INT, INT>> Points )
  {
    this->Points = Points;
  }

  // Set points to show
  // ARGUMENTS - position of points (std::vector<std::pair<INT, INT>>)  
  VOID SetPoints(const std::vector<std::pair<double, double>> Points)
  {
    this->Points.clear();
    this->Points.reserve(Points.size());
    for (int i = 0; i < Points.size(); i++)
      this->Points.push_back(std::make_pair(Points[i].first, Points[i].second));
  }

  // Response GUI
  VOID Response( VOID )
  {
    Frame = cv::Mat(H, W, CV_8UC3, cv::Scalar::all(0));
    // Draw Zones
    for (int i = 0; i < Zones.size(); i++)
    {
      const cv::Point *elementPoints[1] = { Zones[i].Corners.data() };
      int numberOfPoints = (int)Zones[i].Corners.size();
      cv::fillPoly(Frame, elementPoints, &numberOfPoints, 1, Zones[i].Color);
    }

    // Draw basic rect
    cv::circle(Frame, ConvertPoint(cv::Point(0, 0)), 4, cv::Scalar(0, 0, 255), -1);
    cv::circle(Frame, ConvertPoint(cv::Point(1, 0)), 4, cv::Scalar(0, 0, 255), -1);
    cv::circle(Frame, ConvertPoint(cv::Point(0, 1)), 4, cv::Scalar(0, 0, 255), -1);
    cv::circle(Frame, ConvertPoint(cv::Point(1, 1)), 4, cv::Scalar(0, 0, 255), -1);

    // draw people
    for (int i = 0; i < Points.size(); i++)
    {
      cv::Point P = ConvertPoint(cv::Point(Points[i].first, Points[i].second));
      cv::rectangle(Frame, cv::Rect(P, cv::Size(3, 3)), cv::Scalar(255, 0, 0));
    }
    cv::imshow("Position View", Frame);
  }
};
