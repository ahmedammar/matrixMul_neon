g++ -o matrixMul_neon matrixMul_gold.cpp matrixMul_neon.cpp main.cpp  -mfloat-abi=softfp -mfpu=neon -O3
