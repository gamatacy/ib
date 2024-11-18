#include "crypto.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD "уверовать"
#define K_VALUES                                                               \
  (int[]) { 6, 14, 5, 7, 12, 11, 4, 9, 19 }

int main() {
  int k = 0;

#if DECRYPT
  encrypted_letter encrypted_word[] = {
      {{283, 493}, {314, 127}}, {{425, 663}, {561, 140}},
      {{568, 355}, {75, 433}},  {{440, 539}, {602, 627}},
      {{188, 93}, {395, 414}},  {{179, 275}, {25, 604}},
      {{72, 254}, {47, 349}},   {{72, 254}, {417, 137}},
      {{188, 93}, {298, 225}},  {{56, 419}, {79, 111}}};
  k = sizeof(encrypted_word) / sizeof(encrypted_letter);
  point decrypted_word[k];
#else
  encrypted_letter encrypted_word[(sizeof(WORD) - 1) / 2];
  printf("Encrypt\n");
  for (int i = 0; i < sizeof(WORD); i += 2) {
    uint16_t *c = (uint16_t *)&WORD[i];
    uint16_t ch = ((*c & 0x00FF) << 8) | ((*c & 0xFF00) >> 8);
    if (k == (sizeof(WORD) - 1) / 2)
      break;
    for (int j = 0; j < ALPHABET_SIZE; ++j) {
      if (__alphabet[j].letter == ch) {
        encrypted_word[k] = encrypt_letter(__alphabet[j].p, K_VALUES[k]);
        break;
      }
    }

    ++k;
  }
  point decrypted_word[(sizeof(WORD) - 1) / 2];
#endif

  printf("Decrypt\n");
  uint16_t *word = (uint16_t *)malloc(sizeof(uint16_t) * (k + 1));
  for (int i = 0; i < k; ++i) {
    decrypted_word[i] = decrypt_letter(encrypted_word[i]);
    for (int j = 0; j < ALPHABET_SIZE; ++j) {
      if (__alphabet[j].p.x == decrypted_word[i].x &&
          __alphabet[j].p.y == decrypted_word[i].y) {
        *(word + i) = (uint16_t)__alphabet[j].letter;
        break;
      }
    }
  }
  *(word + k) = '\0';

  printf("UTF8: ");
  for (int i = DECRYPT; i < k; ++i) {
    printf("%x", *(word + i));
  }
  printf("\n");
}
