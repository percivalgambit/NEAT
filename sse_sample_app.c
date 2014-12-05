
/*! @file
 *  This is a sample application using SSE floating-point arithmetic instructions to
 * test the ftrace tool.
 */

#include <xmmintrin.h>

/*!
 * The main procedure of the application.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments
 */
int main(int argc, char *argv[]) {
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
