#include <xmmintrin.h>

int main() {
    float a_double[] __attribute__ ((aligned (16))) = { 2.0,  0, 0, 2.0 };
    float b_double[] __attribute__ ((aligned (16))) = { 0.5,  0, 0, 0.5 };

    __m128 *a_ptr = (__m128 *)a_double;
    __m128 *b_ptr = (__m128 *)b_double;

    __m128 c = _mm_add_ss(*a_ptr, *b_ptr);
    __m128 d = _mm_mul_ss(c, *b_ptr);
    __m128 e = _mm_sub_ss(*a_ptr, *b_ptr);
    __m128 f = _mm_div_ss(*a_ptr, e);
    return 0;
}
