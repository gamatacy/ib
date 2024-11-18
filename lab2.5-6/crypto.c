#include "crypto.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

long __mod(long a, long n) {
  if (a > 0)
    return a % n;
  else {
    long i = 0;
    for (;;) {
      if (llabs(a - i) % n == 0)
        return i;
      ++i;
    }
  }
}

int mod_inverse(int a, int m) {
  int ret = 0;
_LOOP:
  if (__mod((ret * a), m) != 1 && ret <= m) {
    ++ret;
    goto _LOOP;
  } else {
    return ret;
  }
}

point add_points(point p, point q) {
  int64_t px = p.x, py = p.y;
  int64_t qx = q.x, qy = q.y;
  point ret = {0, 0};

  if (px == 0 && py == 0) {
    return q;
  }
  if (qx == 0 && qy == 0) {
    return p;
  }

  int64_t l;

  if (px == qx && py == qy) {
    l = (3 * px * px - 1) * mod_inverse(2 * py, MOD);
  } else {
    l = (py - qy) * mod_inverse(px - qx, MOD);
  }

  ret.x = __mod(l * l - px - qx, MOD);
  ret.y = __mod(l * (px - ret.x) - py, MOD);

  return ret;
}

point mul_point(point p, int k) {
  point accum = p;
  point ans = {0, 0};

  while (k > 0) {
    if (k % 2 > 0) {
      ans = add_points(ans, accum);
    }
    accum = add_points(accum, accum);
    k >>= 1;
  }
  return ans;
}

point neg(point p) {
  p.y = -p.y;
  return p;
}

int calc_secret_key(point public_key) {
  int secret_key = 1;
  point p;
_LOOP:
  p = mul_point(G, secret_key);
  if (p.x == PUBLIC_KEY.x && p.y == PUBLIC_KEY.y)
    return secret_key;
  ++secret_key;
  goto _LOOP;
}

encrypted_letter encrypt_letter(point p, int k) {
  point kG = mul_point(G, k);
  point kP = mul_point(PUBLIC_KEY, k);
  point pkP = add_points(p, kP);
  encrypted_letter cM = {kG, pkP};
  return cM;
}

point decrypt_letter(encrypted_letter l) {
#if DECRYPT
  int secret_key = SECRET_KEY;
#else
  int secret_key = calc_secret_key(PUBLIC_KEY);
#endif
  point decrypted_letter = add_points(l.pkP, mul_point(neg(l.kG), secret_key));
  return decrypted_letter;
}

void print_point(point p) { printf("x: %d y: %d\n", p.x, p.y); }

alphabet __alphabet[] = {
    {.letter = ' ', {33, 355}},  {.letter = 'C', {67, 667}},
    {.letter = 'f', {100, 364}}, {.letter = 'К', {200, 30}},
    {.letter = '!', {33, 396}},  {.letter = 'D', {69, 241}},
    {.letter = 'g', {100, 387}}, {.letter = 'Л', {200, 721}},
    {.letter = '\'', {34, 74}},  {.letter = 'E', {69, 510}},
    {.letter = 'h', {102, 267}}, {.letter = 'М', {203, 324}},
    {.letter = '#', {34, 677}},  {.letter = 'F', {70, 195}},
    {.letter = 'i', {102, 484}}, {.letter = 'Н', {203, 427}},
    {.letter = '$', {36, 87}},   {.letter = 'G', {70, 556}},
    {.letter = 'j', {105, 369}}, {.letter = 'О', {205, 372}},
    {.letter = ':', {36, 664}},  {.letter = 'H', {72, 254}},
    {.letter = 'k', {105, 382}}, {.letter = 'П', {205, 379}},
    {.letter = '&', {39, 171}},  {.letter = 'I', {72, 497}},
    {.letter = 'l', {106, 24}},  {.letter = 'Р', {206, 106}},
    {.letter = '\"', {39, 580}}, {.letter = 'J', {73, 72}},
    {.letter = 'm', {106, 727}}, {.letter = 'С', {206, 645}},
    {.letter = '(', {43, 224}},  {.letter = 'K', {73, 679}},
    {.letter = 'n', {108, 247}}, {.letter = 'Т', {209, 82}},
    {.letter = ')', {43, 527}},  {.letter = 'L', {74, 170}},
    {.letter = 'o', {108, 504}}, {.letter = 'У', {209, 669}},
    {.letter = '*', {44, 366}},  {.letter = 'M', {74, 581}},
    {.letter = 'p', {109, 200}}, {.letter = 'Ф', {210, 31}},
    {.letter = '+', {44, 385}},  {.letter = 'N', {75, 318}},
    {.letter = 'q', {109, 551}}, {.letter = 'Х', {210, 720}},
    {.letter = ',', {45, 31}},   {.letter = 'O', {75, 433}},
    {.letter = 'r', {110, 129}}, {.letter = 'Ц', {215, 247}},
    {.letter = '-', {45, 720}},  {.letter = 'P', {78, 271}},
    {.letter = 's', {110, 622}}, {.letter = 'Ч', {215, 504}},
    {.letter = '.', {47, 349}},  {.letter = 'Q', {78, 480}},
    {.letter = 't', {114, 144}}, {.letter = 'Ш', {218, 150}},
    {.letter = '/', {47, 402}},  {.letter = 'R', {79, 111}},
    {.letter = 'u', {114, 607}}, {.letter = 'Щ', {218, 601}},
    {.letter = '0', {48, 49}},   {.letter = 'S', {79, 640}},
    {.letter = 'v', {115, 242}}, {.letter = 'Ъ', {221, 138}},
    {.letter = '1', {48, 702}},  {.letter = 'T', {80, 318}},
    {.letter = 'w', {115, 509}}, {.letter = 'Ы', {221, 613}},
    {.letter = '2', {49, 183}},  {.letter = 'U', {80, 433}},
    {.letter = 'x', {116, 92}},  {.letter = 'Ь', {226, 9}},
    {.letter = '3', {49, 568}},  {.letter = 'V', {82, 270}},
    {.letter = 'y', {116, 659}}, {.letter = 'Э', {226, 742}},
    {.letter = '4', {53, 277}},  {.letter = 'W', {82, 481}},
    {.letter = 'z', {120, 147}}, {.letter = 'Ю', {227, 299}},
    {.letter = '5', {53, 474}},  {.letter = 'X', {83, 373}},
    {.letter = '{', {120, 604}}, {.letter = 'Я', {227, 452}},
    {.letter = '6', {56, 332}},  {.letter = 'Y', {83, 378}},
    {.letter = '|', {125, 292}}, {.letter = 'а', {228, 271}},
    {.letter = '7', {56, 419}},  {.letter = 'Z', {85, 35}},
    {.letter = '}', {125, 459}}, {.letter = 'б', {228, 480}},
    {.letter = '8', {58, 139}},  {.letter = '[', {85, 716}},
    {.letter = '~', {126, 33}},  {.letter = 'в', {229, 151}},
    {.letter = '9', {58, 612}},  {.letter = '\\', {86, 25}},
    {.letter = 'А', {189, 297}}, {.letter = 'г', {229, 600}},
    {.letter = ';', {59, 386}},  {.letter = '^', {90, 21}},
    {.letter = 'В', {192, 32}},  {.letter = 'е', {234, 587}},
    {.letter = '<', {61, 129}},  {.letter = '_', {90, 730}},
    {.letter = 'Г', {192, 719}}, {.letter = 'ж', {235, 19}},
    {.letter = '=', {61, 622}},  {.letter = '`', {93, 267}},
    {.letter = 'Д', {194, 205}}, {.letter = 'з', {235, 732}},
    {.letter = '>', {62, 372}},  {.letter = 'a', {93, 484}},
    {.letter = 'Е', {194, 546}}, {.letter = 'и', {236, 39}},
    {.letter = '?', {62, 379}},  {.letter = 'b', {98, 338}},
    {.letter = 'Ж', {197, 145}}, {.letter = 'й', {236, 712}},
    {.letter = '@', {66, 199}},  {.letter = 'c', {98, 413}},
    {.letter = 'З', {197, 606}}, {.letter = 'к', {237, 297}},
    {.letter = 'A', {66, 552}},  {.letter = 'd', {99, 295}},
    {.letter = 'И', {198, 224}}, {.letter = 'л', {237, 454}},
    {.letter = 'м', {238, 175}}, {.letter = 'с', {243, 664}},
    {.letter = 'ц', {250, 14}},  {.letter = 'ы', {253, 540}},
    {.letter = 'н', {238, 576}}, {.letter = 'т', {247, 266}},
    {.letter = 'ч', {250, 737}}, {.letter = 'ь', {256, 121}},
    {.letter = 'о', {240, 309}}, {.letter = 'у', {247, 485}},
    {.letter = 'ш', {251, 245}}, {.letter = 'э', {256, 630}},
    {.letter = 'п', {240, 442}}, {.letter = 'ф', {249, 183}},
    {.letter = 'щ', {251, 506}}, {.letter = 'ю', {257, 293}},
    {.letter = 'р', {243, 87}},  {.letter = 'х', {249, 568}},
    {.letter = 'ъ', {253, 211}}, {.letter = 'я', {257, 458}},
    {.letter = 'B', {67, 84}},   {.letter = 'e', {99, 456}},
    {.letter = 'Й', {198, 527}}};