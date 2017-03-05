/*! @file
 * This is a sample application using SSE floating-point arithmetic instructions
 * to test the ftrace tool.
 */

#include <stdint.h>
#include <stdio.h>

/// Print the hex value of a 32-bit value to stdout
#define PRINT_HEX(fp) printf("%08x\n", *(uint32_t *)&(fp))

float helper1(float a, float b);

float nested_helper();

void helper2();

float a, b, c, d, e, f, g, h, i, j, k, l, m, n;

/**
 * The main procedure of the application.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments
 */
int main(int argc, char *argv[]) {
  a = 2.0f;  // a = 2.0
  b = 0.3f;  // b = 0.3
  helper1(a, b);

  // Make sure that arithmetic instructions operating on two registers work
  // correctly
  h = b + b;  // h = 0.3 + 0.3 = 0.6

  helper2();

  PRINT_HEX(a);
  PRINT_HEX(b);
  PRINT_HEX(c);
  PRINT_HEX(d);
  PRINT_HEX(e);
  PRINT_HEX(f);
  PRINT_HEX(g);
  PRINT_HEX(h);
  PRINT_HEX(i);
  PRINT_HEX(j);
  PRINT_HEX(k);
  PRINT_HEX(l);
  PRINT_HEX(m);
  PRINT_HEX(n);

  return 0;
}

float helper1(float fst, float snd) {
  // Make sure that normal arithmetic operations produce the right output
  c = fst + snd;  // c = 2.0 + 0.3 = 2.3
  d = snd - fst;  // d = 0.3 - 2.0 = -1.7
  e = c * a;      // e = 2.0 * 2.3 = 4.6
  f = fst / snd;  // f = 2.0 / 0.3 = 6.666666...
  return nested_helper();
}

float nested_helper() {
  g = (a + f) / b;  // g = (6.66666... + 2.0) / 0.3 = 28.888885
  return g;
}

void helper2() {
  // Make sure that arithmetic with large and small numbers produces the right
  // output
  i = 6.01e+30f;    // i = 6.01e+30
  j = a + i;        // j = 6.01e+30 + 2.0 = 6.01e+30
  k = a * i;        // k = 6.01e+30 * 2.0 = 1.202e+31
  l = 1.2345e-35f;  // l = 1.2345e-35
  m = a + l;        // m = 1.2345e-35 + 2.0 = 2.0
  n = l + l;        // n  = 1.2345e-35 + 1.2345e-35 = 2.4690e-35
}
