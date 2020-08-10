% Test signal was a 0.5V amplitude, 50 Hz sine wave with a 2.5V offset. Test
% noise of 1.1 KHz was added to evaluate filter performance. 

% The cuttoff frequency of the filter was 0.02*fs or 56 Hz. 


% below are some output samples from the chip (taken using a logic analyzer)
fID = fopen('logic_bus_output_kernel_length_17.txt', 'r');
data = textscan(fID,'%s');
fclose(fID);
x = str2double(data{1}(1:300))';
n = 0:length(x)-1;

subplot(2,2,1); stem(n,x,'Linestyle','none'); title('output samples');
subplot(2,2,2); stairs(n,x); title('ZOH');
subplot(2,2,3); plot(n,x); title('FOH'); 

% The period is about 56 samples

% Each sample on logic analyzer takes 383.7us, which gives a frequency of
% about 47 Hz, very close to input 50 Hz.

% The max for a 10 bit output is 1024, input signal was 1V amplitude with
% 2.5V offset. Looking at the figures, the offset is around 516 units, with
% 516/1024 =~ 0.5. This is acceptable because 2.5V is half of the reference
% voltage on the chip.

% Max = 605 and min = 410, averaging a 97.5 unit swing, (98/1024)*5V = 0.5V
% which is also acceptable because the input target signal swings between
% 3V and 2V

% Visual simililarity to a sinusoid is ok. 

% Overall, both interpolations show acceptable analog reconstruction (the
% code works!!!!!)

