
# include <mex.h>

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

	// calucuration
	int dim_i;
	double *w_ptr = w;
	double *x_ptr = x;
	for (dim_i = 0; dim_i < in_dim; dim_i++)
	{
		*y += (*w_ptr) * (*x_ptr);
		w_ptr++;
		x_ptr++;
	}
}


	