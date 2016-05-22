
#include <mex.h>
#include <omp.h>

#include <immintrin.h>

float calc_function(float* w, float* x, int in_dim_i_begin, int in_dim_i_end)
{
	// calucuration
	int   in_dim_i;
	float* w_ptr = w + in_dim_i_begin;
	float* x_ptr = x + in_dim_i_begin;
	
	__m256 w_x8;
	__m256 x_x8;
	__m256 wx_x8;
	__m256 sum_x8;
	
	sum_x8 = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, 0);

	for (in_dim_i = in_dim_i_begin; in_dim_i <= (in_dim_i_end - 8 + 1); in_dim_i += 8)
	{
		w_x8   = _mm256_load_ps(w_ptr);
		x_x8   = _mm256_load_ps(x_ptr);
		wx_x8  = _mm256_mul_ps(w_x8, x_x8);
		sum_x8 = _mm256_add_ps(sum_x8, wx_x8);

		w_ptr += 8;
		x_ptr += 8;
	}
	
	float sum[8] = {0};
	_mm256_store_ps(sum, sum_x8);

	return sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];
}

void mexFunction(int nlhs,       mxArray *plhs[], 
	             int nrhs, const mxArray *prhs[])
{
	// I/O check
	if(nrhs != 2) { mexErrMsgTxt("Wrong number of input  arguments."); }
	if(nlhs != 1) {	mexErrMsgTxt("Wrong number of output arguments."); }
	if(mxGetClassID(prhs[0]) != mxSINGLE_CLASS) { mexErrMsgTxt("w must be a real vector."); }
	if(mxGetClassID(prhs[1]) != mxSINGLE_CLASS) { mexErrMsgTxt("w must be a real vector."); }

	// get input value
	float *w, *x;
	int *in_dim_ptr, in_dim;
	w          = (float*)mxGetPr(prhs[0]);
	x          = (float*)mxGetPr(prhs[1]);
	in_dim_ptr = (int*)mxGetDimensions(prhs[0]);
	in_dim     = in_dim_ptr[0];

	// make output variable
	float *y;
	int out_dim_arr[1];
	out_dim_arr[0] = 1;
	plhs[0] = mxCreateNumericArray(1, out_dim_arr, mxSINGLE_CLASS, mxREAL);
	y       = (float*)mxGetData(plhs[0]);
	*y      = 0;

	// do thread setting
	int num_thread  = (int)omp_get_max_threads();
	int thread_i;
	int *in_dim_i_begin, *in_dim_i_end;
	in_dim_i_begin    = new int[num_thread];
	in_dim_i_end      = new int[num_thread];
	in_dim_i_begin[0] = 0;
	for (thread_i = 0; thread_i < (num_thread - 1); thread_i++)
	{
		in_dim_i_end  [thread_i]     = (int)((thread_i + 1) * in_dim / num_thread) - 1;
		in_dim_i_begin[thread_i + 1] = in_dim_i_end[thread_i] + 1;
	}
	in_dim_i_end[num_thread - 1] = in_dim - 1;
	
	float* sum = new float[num_thread];
	//#pragma omp parallel for num_threads(num_thread)
	for (thread_i = 0; thread_i < num_thread; thread_i++)
	{
		sum[thread_i] = calc_function(w, x, in_dim_i_begin[thread_i], in_dim_i_end[thread_i]);
	}
	for (thread_i = 0; thread_i < num_thread; thread_i++)
	{
		*y += sum[thread_i];
	}
}


	