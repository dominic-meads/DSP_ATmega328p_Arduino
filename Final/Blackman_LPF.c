/*
 * Blackman_LPF.c
 *
 *
 * Sampling frequency = 3.92 KHz (Nyquist = 1.96 KHz)
 * Creates a LPF with a cuttoff frequency = 78.4 Hz (0.02*fs). Output to a 10-bit R2R DAC
 *
 *
 * 8/6/2020
 * Author : Dominic Meads
 */ 

#include <avr/io.h>
#include <stdint.h>           // int to uint16_t for optimization
#include <avr/interrupt.h>    // for sei function
#include <math.h>             // for trig functions




// function declarations
	// ADC 
void ADC_init();                 // initialize ADC in single conversion mode
void Start_conversion();         // run a single conversion on input channel A1
uint16_t ADC_get_data();         // read ADC data registers in appropriate order
	// DSP
float convolve(uint16_t * array, float * kernel, uint8_t size);    // performs a convolution between two input arrays, returns single value
void Blackman_kernel(float * array, uint8_t size, float fc);       // generates the FIR filter coefficients based on windoww type and cuttoff frequency
	// Array
void right_shift_one(uint16_t * array, uint8_t size);  // right shifts each element in the array by one place
// end function declarations



// Global Variables (for access by ISR)
uint16_t ADC_data = 0;
// end global variables



int main(void)
{
	DDRB = 0b00001111;  // PB3 down to PB0 will be the top 4 bits of the 10 bit output to the R2R DAC (PB3 = MSB)
    DDRD = 0b11111100;  // PD7 down to PD2 will be the bottom 6 bits of the 10 bit output to the R2R DAC (PD2 = LSB)
	
	// input variables
	uint16_t signal[17] = {};                                   // MUST INITIALIZE TO ZERO FOR PADDING
	uint8_t signal_length = sizeof(signal)/sizeof(signal[0]);  // length of "signal" 	
	// DSP variables
	float kernel[17] = {};                                 // filter kernel (array of filter coefficients) MUST BE TYPE FLOAT OR DOUBLE
	uint8_t kernel_length = sizeof(kernel)/sizeof(kernel[0]);  // calculate length of kernel
	float fc = 0.020;                                      // cuttoff frequency of 0.020*fsample
	
	// output variables 
	float out = 0;       // A convolution only returns one output, which will be type float
	uint16_t out_adjust = 0;  // the R2R DAC is 10 bits, thus the "out" variable must be approximated to a variable that can be an input to the DAC (int)
	
	sei();               // enable global interrupts
	
	ADC_init();          // initialize the ADC
	
	Blackman_kernel(kernel,kernel_length,fc);  // generate FIR filter coefficients
		
    while (1) 
    {
		right_shift_one(signal,signal_length);   // shift the signal array to clear a spot for new ADC value
		Start_conversion();                           // start the ADC conversion, ISR will take over and put the data into the global variable "ADC_data"
		signal[0] = ADC_data;                         // put the ADC result into the open signal spot
		out = convolve(signal,kernel,kernel_length);  // perform the convolution with the filter kernel
		out_adjust = (int) out;                       // approximate the double to an integer (not ideal, but the R2R DAC can only use integer form binary numbers) 
		PORTB = out_adjust >> 6;					  // output top 4 bits of result into PORTB
		PORTD = out_adjust << 2;                      // output bottom 6 bits of result into PORTD (avoid outputs on TX(PD1) and RX(PD0) pins)	
    }
}



// function definitions
	// ADC 
	void ADC_init()
	{	// sets ADC in single conversion mode
		PRR = (0 << PRADC);  // disable low power mode
		ADMUX = (1 << REFS0) | (1 << MUX0);  // sets AVcc as reference and A1 as single ended input
		ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // enable ADC and interrupt, set a pre-scaler of 128 (50 KHz < ADC clk < 200KHz for max resolution)
		DIDR0 = (1 << ADC1D);  // disable digital input to ADC pin A1
	}
	
	void Start_conversion()
	{   // the first conversion will take 25 ADC clk cycles to initialize, the rest will take only 13
		ADCSRA |= (1 << ADSC);  // start conversion (the ADSC bit will clear upon successful conversion, need to set again to start another)
	}
	
	uint16_t ADC_get_data()
	{
		uint16_t ADC_data;
		ADC_data = ADCL;  // ADCL must be read before ADCH
		ADC_data |= (ADCH << 8);  // shift and place the MSB and MSB-1 into the data
		
		return ADC_data;
	}
	
	// DSP
	float convolve(uint16_t * array, float * kernel, uint8_t size)  // performs a convolution
	{
		float result = 0;  // output
		for(uint8_t i = 0; i < size; i++)
		{
			result += array[i] * kernel[i];  // MAC
		}
		
		return result;
	}
	
	void Blackman_kernel(float * h, uint8_t size, float fc)
	{
		float k = 0;  // init normalization coefficient
		for(uint8_t i = 0; i < size; i++)
		{
			h[i] = (sin(2*M_PI*fc*(i-size/2))/(i-size/2)) * (0.42 - 0.5*cos(2*M_PI*i/size) + 0.08*cos(4*M_PI*i/size));  // the Blackman window has great stop band attenuation, but slower rolloff than the Hamming window
			//     ^-------------sinc function----------^   ^-------------------Blackman window---------------------^
			if (i == size/2)
			{
				h[i] = 2*M_PI*fc;  // take care of Nan case
			}
		}
		for(uint8_t ii = 0; ii < size; ii++){  // calculate normalization coefficient
			k += h[ii];
		}
		    
		for(uint8_t iii = 0; iii < size; iii++){  // normalize array
			h[iii] = h[iii]/k;
		}
	}
	
	// Array
	void right_shift_one(uint16_t * array, uint8_t size)
	{
		for(uint8_t i = size - 1; i > 0; i--)
		{
			array[i]=array[i-1];
		}
		array[0]=0;
	}
// end function definitions



// Interrupt Service Routines
ISR(ADC_vect){
	ADC_data = ADC_get_data();  // place the conversion result into the global variable ADC_data
}
// end Interrupt Service Routines
