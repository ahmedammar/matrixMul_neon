#include <sys/times.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <arm_neon.h>
#include <CL/opencl.h>

#ifndef __pld
#define __pld(x) asm volatile ( "   pld [%[addr]]\n" :: [addr] "r" (x) : "cc" );
#endif

int computeNeon(cl_short* C, const cl_short8* A, const cl_short8* B, unsigned int hA, unsigned int wA, unsigned int wB) {


	uint16x4_t row_v1, col_v1, product_v1;
	uint16x4_t row_v2, col_v2, product_v2;
	uint16x4_t sum;
	uint16_t s[2];
	uint32_t i, j, k;

	for (i = 0; i < hA; i++) {
		for(j = 0; j< wB; j++) {
#if 0
			// First splat V[x] in V_v.
			col_v1 = vdup_n_u16(B[j*wB].s0);
			col_v2 = vdup_n_u16(B[j*wB].s4);
#endif
			// Initialize product_v vector to zero
			product_v1 = vdup_n_u16(0.0f);
			product_v2 = vdup_n_u16(0.0f);
			for (k = 0; k < wA; k++) {
				// Load row's next 8 16bit ints into row_v
				row_v1 = vld1_u16((const uint16_t*)&A[i*wA + k].s);
				row_v2 = vld1_u16(((const uint16_t*)&A[i*wA + k].s)+4);

				col_v1 = vld1_u16((const uint16_t*)&B[k*wB + j].s);
				col_v2 = vld1_u16(((const uint16_t*)&B[k*wB + j].s)+4);

				// Multiply V[x] with 4 row[x] elements 
				// add to previous product_v and store back the result
				product_v1 = vmla_u16(product_v1, row_v1, col_v1);
				product_v2 = vmla_u16(product_v2, row_v2, col_v2);
			}
			// Now add all elements and store the result to a sum vector
			// and then store that to s[2] array. The sum is in s[0]
			sum = vpadd_u16(product_v1, product_v2);
			sum = vpadd_u16(sum, sum);
			vst1_u16(s, sum);

			C[i * wB + j] = s[0] + s[1];
		}
	}

	return 0;
}
