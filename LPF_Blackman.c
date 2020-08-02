/******************************************************************************
convolves an array that is shifting
*******************************************************************************/

#include <stdio.h>
#include <math.h>




// function declarations
void right_shift_one(int * array, int * array_copy, int size);

void copy_array(int * array, int * array_copy, int size);

void print_array(int * array, int size);

void Blackman_kernel(double * array, int size, double fc);




int main()
{
    int my_array[8] = {0,0,1,1,0,0,0,0};
    int my_array_copy[8] = {};
    int my_size = sizeof(my_array)/sizeof(my_array[0]);  // calculate length of array
    
    copy_array(my_array, my_array_copy, my_size);  // make a copy of the array for shifting
    
    double kernel[17] = {};  // filter kernel (array of filter coefficients) MUST BE TYPE DOUBLE
    int kernel_size = sizeof(kernel)/sizeof(kernel[0]);  // calculate length of kernel
    double fc = 0.020;  // cuttoff frequency of 0.020*fsample
    
    FILE * fp;
    fp = fopen("kernel.txt","w+");
    
    Blackman_kernel(kernel, kernel_size, fc);
    for(int ii = 0; ii < 17; ii++)
    {
        printf("%f ",kernel[ii]);
        fprintf(fp,"%f\n",kernel[ii]);
    }
    
    right_shift_one(my_array, my_array_copy, my_size);
    print_array(my_array, my_size);
    
    return 0;
}




// function definitions
void right_shift_one(int * array, int * array_copy, int size)
{
    for(int i = 1; i < size; i++)
    {
        array[i] = array_copy[i-1];  // shift one to right (>>)
    }
    
    array[0] = 0;  // shift in a zero
}


void copy_array(int * array, int * array_copy, int size)
{
    for(int k = 0; k < size; k++)  
    {
        array_copy[k] = array[k];
    }
}


void print_array(int * array, int size)
{
    for(int ii = 0; ii < size; ii++)
    {
        printf("%d",array[ii]);
    }
}


void Blackman_kernel(double * h, int size, double fc)
{
    for(int i = 0; i < size; i++)
    {
        h[i] = (sin(2*M_PI*fc*(i-size/2))/(i-size/2)) * (0.42 - 0.5*cos(2*M_PI*i/size) + 0.08*cos(4*M_PI*i/size));  // the Blackman window has great stop band attenuation
        //     ^-------------sinc function----------^   ^-------------------Blackman window---------------------^ 
        
        if (i == size/2)
        {
           h[i] = 2*M_PI*fc;  // take care of Nan 
        }
    }
}
