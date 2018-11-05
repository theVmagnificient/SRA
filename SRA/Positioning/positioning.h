// positioning module
// Programmer: VP1
// Date: 17.10.2018

#pragma once
#include <cstdio>
#include <cstring>
#include <string>
#include <opencv2\core.hpp>

struct ZONE
{
  std::vector<cv::Point> Corners;
  cv::Scalar Color;
};                 // Zones

class positioning
{
  double H,  // H of camera
    Xl, Yl,  // local X and Y
    Xg, Yg;  // global X and Y
  double M[4][4]; // Matrix

  std::pair<double, double> project(std::pair<double, double> P)
  {
    double a = P.first * M[0][0] + P.second * M[1][0] + 1 * M[2][0];
    double b = P.first * M[0][1] + P.second * M[1][1] + 1 * M[2][1];
    double w = P.first * M[0][2] + P.second * M[1][2] + 1 * M[2][2];
    return std::make_pair(a / w, b / w);
  }

  std::vector<ZONE> Zones; // All zones

  bool IsPointInZone(int ZoneNum, cv::Point Point)
  {
    bool c = false;
    double y = Point.y;
    double x = Point.x;
    for (int i = 0, j = Zones[ZoneNum].Corners.size() - 1; i < Zones[ZoneNum].Corners.size(); j = i++)
    {
      double ypi = Zones[ZoneNum].Corners[i].y;
      double xpi = Zones[ZoneNum].Corners[i].x;
      double ypj = Zones[ZoneNum].Corners[j].y;
      double xpj = Zones[ZoneNum].Corners[j].x;
      if ((((ypi <= y) && (y<ypj)) || ((ypj <= y) && (y<ypi))) &&
        (x >(xpj - xpi) * (y - ypi) / (ypj - ypi) + xpi))
        c = !c;
    }
    return c;
  }
public:
  // Constructor
  // h1 - H of camera
  // x1 y1 - local X and Y
  // x2 y2 - global X and Y
  // m - Matrix
  positioning( double h1 = 0, double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0, double *m = nullptr ) : H(h1), Xl(x1), Yl(y1), Xg(x2), Yg(y2)
  {
    if (m != nullptr)
      memcpy(M, m, sizeof(double) * 16);
  }

  // load from file
  // S - file name
  void Load( const std::string &S )
  {
    FILE *F = fopen(S.c_str(), "r");
    if (F != NULL)
    {
      fscanf(F, "%lf", &H);
      fscanf(F, "%lf%lf", &Xl, &Yl);
      fscanf(F, "%lf%lf", &Xg, &Yg);
      for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
          fscanf(F, "%lf", &(M[i]) + j);
      fclose(F);
    }
  }

  /* Load all Zones */
  VOID LoadZones(const std::string &FName)
  {
    FILE *F;
    if ((F = fopen(FName.c_str(), "r")) != NULL)
    {
      CHAR Buf[600] = { 0 };
      ZONE tmp;
      while (fgets(Buf, 599, F))
      {
        if (Buf[0] == 'z')
        {
          if (tmp.Corners.size() != 0)
          {
            Zones.push_back(tmp);
            tmp = ZONE();
          }
        }
        else
        {
          int x = 0, y = 0;
          sscanf(Buf, "%i %i", &x, &y);
          tmp.Corners.push_back(cv::Point(x, y));
        }
      }
      if (tmp.Corners.size() != 0)
        Zones.push_back(tmp);
      fclose(F);
    }
  }

  // Transform coordinates of pixel to coordinates of world
  // In - coordinates
  // Hh - Human height
  std::pair<double, double> GetPosition(std::pair<double, double> In, double Hh)
  {
    auto FacePosition = project(In);
    auto B = std::make_pair(FacePosition.first - Xl, FacePosition.second - Yl);
    double B1 = 1.0 - Hh / H;
    return std::make_pair(B.first * B1 + Xl + Xg, B.second * B1 + Yl + Yg);
  }

  // Get Number of Zone or -1 if no Zone
  INT GetZone(std::pair<double, double> Point)
  {
    for (int i = 0; i < Zones.size(); i++)
      if (IsPointInZone(i, cv::Point(Point.first, Point.second)))
        return i;
    return -1;
  }
};
