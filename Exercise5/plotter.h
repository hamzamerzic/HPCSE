#ifndef PLOTTER_H
#define PLOTTER_H

#include <vector>
#include <utility>
#include <cmath>
#include <string>
#include <fstream>

class Plotter {
 public:
  Plotter(unsigned width, unsigned height, int radius)
    : width_(width), height_(height), radius_(radius),
      pixels_(width_, std::vector<int> (height_, 0)) {}

  void Plot(std::vector<std::pair<int, int>>& points,
      std::string file="output.pbm") {
    pixels_ = std::move(std::vector<std::vector<int>>
      (width_, std::vector<int> (height_, 0)));
    for (auto& point : points) {
      DrawPoint(pixels_, point);
    }
    WriteOut(file);
  }

 private:
  void DrawPoint(std::vector<std::vector<int>>& pixels_,
      std::pair<int, int>& point) {
    int x0 = point.first, y0 = point.second;
    pixels_[x0][y0] = 1;

    int x = radius_, y = 0, decisionOver2 = 1 - x;
    while(y <= x)
    {
      pixels_[mod( x + x0, width_)][mod( y + y0, height_)] = 1; // Octant 1
      pixels_[mod( y + x0, width_)][mod( x + y0, height_)] = 1; // Octant 2
      pixels_[mod(-x + x0, width_)][mod( y + y0, height_)] = 1; // Octant 4
      pixels_[mod(-y + x0, width_)][mod( x + y0, height_)] = 1; // Octant 3
      pixels_[mod(-x + x0, width_)][mod(-y + y0, height_)] = 1; // Octant 5
      pixels_[mod(-y + x0, width_)][mod(-x + y0, height_)] = 1; // Octant 6
      pixels_[mod( x + x0, width_)][mod(-y + y0, height_)] = 1; // Octant 8
      pixels_[mod( y + x0, width_)][mod(-x + y0, height_)] = 1; // Octant 7
      y++;
      if (decisionOver2<=0)
      {
        decisionOver2 += 2 * y + 1;
      }
      else
      {
        x--;
        decisionOver2 += 2 * (y - x) + 1;
      }
    }
  }

  void WriteOut(std::string file) {
    std::fstream output (file, std::fstream::out);

    output << "P1" << std::endl << width_ << " " << height_ << std::endl;
    for (int j = height_ - 1; j >= 0; --j) {
      for (unsigned i = 0; i < width_; ++i) {
        output << pixels_[i][j] << " ";
      }
      output << std::endl;
    }
    output.close();
  }

  inline int mod(int x, int m) {
    return (x % m + m) % m;
  }

  unsigned width_, height_;
  int radius_;
  std::vector<std::vector<int>> pixels_;
};

#endif  // PLOTTER_H
