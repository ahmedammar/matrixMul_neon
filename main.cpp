#include <sys/times.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <arm_neon.h>
#include <CL/opencl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern int computeNeon(cl_short* C, const cl_short8* A, const cl_short8* B, unsigned int hA, unsigned int wA, unsigned int wB);

extern "C"
void computeGold(cl_short* C, const cl_short8* A, const cl_short8* B, unsigned int hA, unsigned int wA, unsigned int wB);

#define N 256

void FillArray(cl_short8* pfData, int iSize)
{
    int i;
    for (i = 0; i < iSize; ++i)
    {
        pfData[i].s0 = rand();
        pfData[i].s1 = rand();
        pfData[i].s2 = rand();
        pfData[i].s3 = rand();
        pfData[i].s4 = rand();
        pfData[i].s5 = rand();
        pfData[i].s6 = rand();
        pfData[i].s7 = rand();
    }
}

double shrDeltaT()
{
	double DeltaT;

	static struct timeval _NewTime;  // new wall clock time (struct representation in seconds and microseconds)
	static struct timeval _OldTime0; // old wall clock time 0(struct representation in seconds and microseconds)
	gettimeofday(&_NewTime, NULL);

	DeltaT =  ((double)_NewTime.tv_sec + 1.0e-6 * (double)_NewTime.tv_usec) - ((double)_OldTime0.tv_sec + 1.0e-6 * (double)_OldTime0.tv_usec);

	// Reset old time 0 to new
	_OldTime0.tv_sec = _NewTime.tv_sec;
	_OldTime0.tv_usec = _NewTime.tv_usec;

	return DeltaT;
}

int main(void)
{
	double delta;
	unsigned int size = sizeof(cl_short8) * N * N;

	cl_short8* h_A_data = (cl_short8*) malloc(size);
	cl_short8* h_B_data = (cl_short8*) malloc(size);
	cl_short* h_C_neon = (cl_short*) malloc(size);
	cl_short* h_C_gold = (cl_short*) malloc(size);

	srand(time(NULL));//2006);
	FillArray(h_A_data, N*N);
	FillArray(h_B_data, N*N);

	shrDeltaT();
	computeGold(h_C_gold, h_A_data, h_B_data, N, N, N);
	delta = shrDeltaT();

	printf("GOLD time: %fs\n", delta);

	shrDeltaT();
	computeNeon(h_C_neon, h_A_data, h_B_data, N, N, N);
	delta = shrDeltaT();

	printf("NEON time: %fs\n", delta);

	return 0;
}
