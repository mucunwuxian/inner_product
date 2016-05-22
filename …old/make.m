mex inner_product_mex.cpp; COMPFLAGS="/openmp $COMPFLAGS"
mex inner_product_omp_mex.cpp     CFLAGS="\$CFLAGS -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp";
mex inner_product_omp_sse_mex.cpp CFLAGS="\$CFLAGS -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp";

