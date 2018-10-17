// Tracking module
// Programmer: VP1
// Date: 17.10.2018

#pragma once
#include <cstdio>
#include <cstring>
#include <string>

class tracking
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

public:
  // Constructor
  // h1 - H of camera
  // x1 y1 - local X and Y
  // x2 y2 - global X and Y
  // m - Matrix
  tracking( double h1 = 0, double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0, double *m = nullptr ) : H(h1), Xl(x1), Yl(y1), Xg(x2), Yg(y2)
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
};
