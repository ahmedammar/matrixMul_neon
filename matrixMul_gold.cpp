#include <CL/opencl.h>

extern "C"
void computeGold( cl_short*, const cl_short8*, const cl_short8*, unsigned int, unsigned int, unsigned int);

void
computeGold(cl_short* C, const cl_short8* A, const cl_short8* B, unsigned int hA, unsigned int wA, unsigned int wB)
{
    for (unsigned int i = 0; i < hA; ++i)
        for (unsigned int j = 0; j < wB; ++j) {
            cl_short8 sum = { 0, 0, 0, 0, 0, 0, 0, 0};
            for (unsigned int k = 0; k < wA; ++k) {
                cl_short8 a = A[i * wA + k];
                cl_short8 b = B[k * wB + j];
                sum.s0 += a.s0 * b.s0;
                sum.s1 += a.s1 * b.s1;
                sum.s2 += a.s2 * b.s2;
                sum.s3 += a.s3 * b.s3;
                sum.s4 += a.s4 * b.s4;
                sum.s5 += a.s5 * b.s5;
                sum.s6 += a.s6 * b.s6;
                sum.s7 += a.s7 * b.s7;
            }
            C[i * wB + j] = sum.s0 + sum.s1 + sum.s2 + sum.s3 + sum.s4 + sum.s5 + sum.s6 + sum.s7;
        }
}
