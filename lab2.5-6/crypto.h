#pragma once

#define MOD (751)
#define PUBLIC_KEY ((point){425, 663})
#define SECRET_KEY 41
#define G ((point){0, 1})

typedef struct {
  int x;
  int y;
} point;

typedef struct {
  point kG;
  point pkP;
} encrypted_letter;

encrypted_letter encrypt_letter(point p, int k);
point decrypt_letter(encrypted_letter l);
void print_point(point p);

typedef struct {
  int letter;
  point p;
} alphabet;

extern alphabet __alphabet[];
#define ALPHABET_SIZE 154