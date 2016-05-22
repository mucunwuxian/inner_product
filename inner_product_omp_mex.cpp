
# include <mex.h>
# include <omp.h>

double calc_function(double* w, double* x, int in_dim_i_begin, int in_dim_i_end)
{
	// calucuration
	int    in_dim_i;
	double *w_ptr = w + in_dim_i_begin;
	double *x_ptr = x + in_dim_i_begin;
	
	double sum = 0;
	for (in_dim_i = in_dim_i_begin; in_dim_i <= in_dim_i_end; in_dim_i++)
	{
		sum += (*w_ptr) * (*x_ptr);
		w_ptr++;
		x_ptr++;
	}
	return sum;
}

void mexFunction(int nlhs,       mxArray *plhs[], 
	             int nrhs, const mxArray *prhs[])
{
	// I/O check
	if(nrhs != 2) { mexErrMsgTxt("Wrong number of input arguments."); }
	if(nlhs != 1) {	mexErrMsgTxt("Wrong number of output arguments."); }
	if(mxGetClassID(prhs[0]) != mxDOUBLE_CLASS) { mexErrMsgTxt("w must be a real vector."); }
	if(mxGetClassID(prhs[1]) != mxDOUBLE_CLASS) { mexErrMsgTxt("w must be a real vector."); }

	// get input value
	double *w, *x;
	int *in_dim_ptr, in_dim;
	w          = (double*)mxGetPr(prhs[0]);
	x          = (double*)mxGetPr(prhs[1]);
	in_dim_ptr = (int*)mxGetDimensions(prhs[0]);
	in_dim     = in_dim_ptr[0];

	// make output variable
	double *y;
	int out_dim_arr[1];
	out_dim_arr[0] = 1;
	plhs[0] = mxCreateNumericArray(1, out_dim_arr, mxDOUBLE_CLASS, mxREAL);
	y       = (double*)mxGetData(plhs[0]);
	*y      = 0;

	// do thread setting
	int num_thread  = omp_get_max_threads();
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
	
	double* sum = new double[num_thread];
	#pragma omp parallel for num_threads(num_thread)
	for (thread_i = 0; thread_i < num_thread; thread_i++)
	{
		sum[thread_i] = calc_function(w, x, in_dim_i_begin[thread_i], in_dim_i_end[thread_i]);
	}
	for (thread_i = 0; thread_i < num_thread; thread_i++)
	{
		*y += sum[thread_i];
	}
}


	