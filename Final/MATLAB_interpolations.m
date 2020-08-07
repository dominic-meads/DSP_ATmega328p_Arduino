% Test signal was a 1V amplitude, 50 Hz sine wave with a 2.5V offset. Test
% noise of 1.1 KHz was added to evaluate filter performance. 

% The cuttoff frequency of the filter was 0.02*fs or 56 Hz. 


% below are some output samples from the chip (taken using a logic analyzer)
x = [289, 295, 303, 314, 358, 372, 389, 406, 458, 478, 435, 489, 511, 533, 587, 544, 565, 616, 633, 649, 662, 705, 714, 719, 724, 725, 726, 723, 720, 715, 708, 667, 654, 640, 623, 572, 552, 594, 541, 518, 496, 442, 420, 464, 412, 393, 376, 361, 316, 305, 296,290, 350, 348, 350, 289, 294, 302, 311, 355, 369, 385, 402, 453, 474, 430, 484, 506, 528, 582, 603, 559, 611, 628];
n = 0:length(x)-1;

subplot(2,2,1); stem(n,x); title('output samples');
subplot(2,2,2); stairs(n,x); title('ZOH');
subplot(2,2,3); plot(n,x); title('FOH'); 

% The period is about 54 samples

% Each sample on logic analyzer takes 383.7us, which gives a frequency of
% about 46 Hz, very close to input 50 Hz.

% The max for a 10 bit output is 1024, input signal was 1V amplitude with
% 2.5V offset. Looking at the figures, the offset is around 500 units, with
% 500/1024 =~ 0.5. This is acceptable because 2.5V is half of the reference
% voltage on the chip.

% Max = 725, and min = 290, averaging a 217 unit swing, (217/1024)*5V =~ 1V
% which is also acceptable because the input target signal swings between
% 3.5V and 1.5V

% Visual simililarity to a sinusoid is ok. 

% Overall, both interpolations show acceptable analog reconstruction (the
% code works!!!!!)
