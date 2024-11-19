#include "crypto.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

  point P = {59,386};
  point Q = {70,195};
  point R = {72,254};

  point res = add_points(
    add_points( mul_point(P, 2), mul_point(Q, 3)) // 2P + 3Q
    ,neg(R) // - R
  );
    
  print_point(res);

}
