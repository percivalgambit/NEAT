
/*! @file
 * This is a sample application using SSE floating-point arithmetic instructions to
 * test the ftrace tool.
 */

/*!
 * The main procedure of the application.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments
 */
int main(int argc, char *argv[]) {
    // Make sure that normal arithmetic operations produce the right output
    float a = 2.0f;        //!< a = 2.0
    float b = 0.3f;        //!< b = 0.3
    float c = a + b;       //!< c = 2.0 + 0.3 = 2.3
    float d = b - a;       //!< d = 0.3 - 2.0 = -1.7
    float e = a * c;       //!< e = 2.0 * 2.3 = 4.6
    float f = a / b;       //!< f = 2.0 / 0.3 = 6.666666...
    float g = (f + a) / b; //!< g = (6.66666... + 2.0) / 0.3 = 28.888885

    // Make sure that arithmetic instructions operating on two registers work correctly
    float h = b + b;       //!< h = 0.3 + 0.3 = 0.6

    // Make sure that arithmetic with large and small numbers produces the right output
    float i = 6.01e+30f;   //!< i = 6.01e+30
    float j = i + a;       //!< j = 6.01e+30 + 2.0 = 6.01e+30
    float k = i * a;       //!< k = 6.01e+30 * 2.0 = 1.202e+31
    float l = 1.2345e-35f; //!< l = 1.2345e-35
    float m = l + a;       //!< m = 1.2345e-35 + 2.0 = 2.0
    float n = l + l;       //!< n  = 1.2345e-35 + 1.2345e-35 = 2.4690e-35

    return 0;
}
