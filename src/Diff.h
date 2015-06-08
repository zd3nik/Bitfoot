//----------------------------------------------------------------------------
// Copyright (c) 2015 Shawn Chidester <zd3nik@gmail.com>, All rights reserved
//----------------------------------------------------------------------------

#ifndef BITFOOT_DIFF_H
#define BITFOOT_DIFF_H

#include "Defs.h"

namespace bitfoot
{

class Diff
{
public:
  //--------------------------------------------------------------------------
  int Dist(const int from, const int to) const {
    if (IS_SQUARE(from) && IS_SQUARE(to)) {
      return distance[from][to];
    }
    return 8;
  }

  //--------------------------------------------------------------------------
  Direction Dir(const int from, const int to) const {
    if (IS_SQUARE(from) && IS_SQUARE(to)) {
      return direction[from][to];
    }
    return OtherDir;
  }

  //--------------------------------------------------------------------------
  Diff() {
    for (int a = 0; a < 64; ++a) {
      for (int b = 0; b < 64; ++b) {
        const int x1 = XC(a);
        const int y1 = YC(a);
        const int x2 = XC(b);
        const int y2 = YC(b);
        const int d1 = abs(x1 - x2);
        const int d2 = abs(y1 - y2);

        distance[a][b]  = std::max<int>(d1, d2);
        direction[a][b] = OtherDir;

        if (x1 == x2) {
          if (y1 > y2) {
            direction[a][b] = South;
          }
          else if (y1 < y2) {
            direction[a][b] = North;
          }
        }
        else if (y1 == y2) {
          if (x1 > x2) {
            direction[a][b] = West;
          }
          else if (x1 < x2) {
            direction[a][b] = East;
          }
        }
        else if (x1 > x2) {
          if (y1 > y2) {
            if (((SQR(x1,y1) - SQR(x2,y2)) % 9) == 0) {
              direction[a][b] = SouthWest;
            }
          }
          else if (y1 < y2) {
            if (((SQR(x2,y2) - SQR(x1,y1)) % 7) == 0) {
              direction[a][b] = NorthWest;
            }
          }
        }
        else if (x1 < x2) {
          if (y1 > y2) {
            if (((SQR(x1,y1) - SQR(x2,y2)) % 7) == 0) {
              direction[a][b] = SouthEast;
            }
          }
          else if (y1 < y2) {
            if (((SQR(x2,y2) - SQR(x1,y1)) % 9) == 0) {
              direction[a][b] = NorthEast;
            }
          }
        }
      }
    }
  }

private:
  int           distance[64][64];
  Direction direction[64][64];
};

extern Diff _diff;

} // namespace bitfoot

#endif // BITFOOT_DIFF_H
