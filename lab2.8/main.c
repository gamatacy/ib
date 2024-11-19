#include "crypto.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

  point P = {36,87};
  int n = 111;

  point res = mul_point(P, n); 
    
  print_point(res);

}
