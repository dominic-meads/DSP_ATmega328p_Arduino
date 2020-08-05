/******************************************************************************
convolves an array that is shifting
*******************************************************************************/

#include <stdio.h>
#include <math.h>




// function declarations
double convolve(double * array, double * kernel, int size);

void right_shift_one(double * array, double * array_copy, int size);

void copy_array(double * array, double * array_copy, int size);

void print_array(double * array, int size);

void Blackman_kernel(double * array, int size, double fc);




int main()
{
    // set up arrays
    float data;
    double my_array[17] = {};  // initialize to zero for padding
    double my_array_copy[17] = {};
    int my_size = sizeof(my_array)/sizeof(my_array[0]);  // calculate length of array
    
    // set up filter variables
    double kernel[17] = {};  // filter kernel (array of filter coefficients) MUST BE TYPE DOUBLE
    int kernel_size = sizeof(kernel)/sizeof(kernel[0]);  // calculate length of kernel
    double fc = 0.020;  // cuttoff frequency of 0.020*fsample
    
    // output variables
    double out;
    
    // initialize the file output showing the FIR coefficients
    FILE * fp1;
    fp1 = fopen("kernel.txt","w+");
    
    // initialize the file output showing the output results
    FILE * fp2;
    fp2 = fopen("output_signal.txt","w+");
    
    // Generate FIR coeficients
    Blackman_kernel(kernel, kernel_size, fc);
    for(int ii = 0; ii < 17; ii++)
    {
        printf("%f ",kernel[ii]);
        fprintf(fp1,"%f\n",kernel[ii]);
    }
    
    printf("\n");
    
    // real time output signal generator
    for(int i = 0; i < 20; i++)
    
    {
        copy_array(my_array, my_array_copy, my_size);  // make a copy of the array for shifting
        right_shift_one(my_array, my_array_copy, my_size);  // shift right to create an open spot
        printf("enter a value: ");
        scanf("%f", &data);  // this and the above line simulate the analog conversion being executing and finishing.
        my_array[0] = data;  // place new data in the cleared array spot (simulating time shift)
        out = convolve(my_array,kernel,my_size);  // generate output from convolution
        fprintf(fp2,"%f\n",out);  // place output result to test in MATLAB
    }
    
    return 0;
}




// function definitions

double convolve(double * array, double * kernel, int size)  // performs a convolution
{
    double result = 0;  // output
    
    for(int i = 0; i < size; i++)
    {
        result += array[i] * kernel[i];  // sum the product of sample array and coefficient array
    }
    
    return result;
}


void right_shift_one(double * array, double * array_copy, int size)
{
    for(int i = 1; i < size; i++)
    {
        array[i] = array_copy[i-1];  // shift one to right (>>)
    }
    
    array[0] = 0;  // shift in a zero
}


void copy_array(double * array, double * array_copy, int size)
{
    for(int k = 0; k < size; k++)  
    {
        array_copy[k] = array[k];
    }
}


void print_array(double * array, int size)
{
    for(int ii = 0; ii < size; ii++)
    {
        printf("%f",array[ii]);
    }
}


void Blackman_kernel(double * h, int size, double fc)
{
    double k = 0;  // init normalization coefficient
    
    for(int i = 0; i < size; i++)
    {
        h[i] = (sin(2*M_PI*fc*(i-size/2))/(i-size/2)) * (0.42 - 0.5*cos(2*M_PI*i/size) + 0.08*cos(4*M_PI*i/size));  // the Blackman window has great stop band attenuation
        //     ^-------------sinc function----------^   ^-------------------Blackman window---------------------^ 
        
        if (i == size/2)
        {
           h[i] = 2*M_PI*fc;  // take care of Nan 
        }
    }
    
    for(int ii = 0; ii < size; ii++){  // calculate normalization coefficient
        k += h[ii];
    }
    
    for(int iii = 0; iii < size; iii++){  // normalize array
        h[iii] = h[iii]/k;
    }
}
