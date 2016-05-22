mex inner_product_mex.cpp; 
mex inner_product_omp_mex.cpp     COMPFLAGS="$COMPFLAGS /openmp";
mex inner_product_omp_sse_mex.cpp COMPFLAGS="$COMPFLAGS /openmp";
mex inner_product_omp_avx_mex.cpp COMPFLAGS="$COMPFLAGS /openmp /arch:AVX /fp:fast";
