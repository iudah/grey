#include "type_alias.h"
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

f32 grey_rand_f32(f32 low, f32 high) {
  auto ratio = arc4random() / (f32)UINT32_MAX;
  f32 rnd = low + (high * ratio) - (low * ratio);
  return rnd;
}
