% Test signal was a 0.31V amplitude, 50 Hz sine wave with a 2.5V offset. Test
% noise of 1.1 KHz (0.19V amplitude, 2.5V offset) was added to evaluate filter performance. 

% The cuttoff frequency of the filter was 0.02*fs or 56 Hz.

n = 1:150;
fs = 1513.77;  % 1/660.6us
Ts = 1/fs;
nTs = n*Ts;

xin = -0.31*sin(50*2*pi*nTs + 1.5) + 0.19*sin(1100*2*pi*nTs) + 2.5;  % input signal (inverted and shifted to visually match up better with output samples)
subplot(2,2,1); plot(n,xin); title('input signal'); axis([0 150 1.5 3.5]); xlabel('Samples'); ylabel('Voltage');

% below are some output samples from the chip (taken using a logic analyzer)
fID = fopen('FIR_output.txt', 'r');
data = textscan(fID,'%s');
fclose(fID);
x = str2double(data{1}(1:150))';
n = 0:length(x)-1;

subplot(2,2,2); stem(n,(x/1023)*5,'Linestyle','none'); title('output samples'); axis([0 150 1.5 3.5]); xlabel('Samples'); ylabel('Voltage');
subplot(2,2,3); stairs(n,(x/1023)*5); title('ZOH'); axis([0 150 1.5 3.5]); xlabel('Samples'); ylabel('Voltage');
subplot(2,2,4); plot(n,(x/1023)*5); title('FOH'); axis([0 150 1.5 3.5]); xlabel('Samples'); ylabel('Voltage');

% The period is about 30 samples

% Each output sample on logic analyzer has a period of 660.6us, which gives
% a frequency of about 50.5 Hz, very close to input 50 Hz.

% The max for a 10 bit output is 1023, input signal was 0.3V amplitude with
% 2.5V offset. Looking at the figures, the output voltage offset is very
% close to 2.5V

% Max = 2.854 and min = 2.175, averaging about a 0.34V amplitude.
% (amplitude a bit higher than expected)

% The below code shows the frequency response of the filter. (found by taking
% the dtft of the impulse response (filter kernel).
h = [0 0.0014 0.0066 0.0175 0.0356 0.0600 0.0870 0.1104 0.1257 0.1236 0.1095 0.0858 0.0589 0.0347 0.0170 0.0064 0.0014];
nh = 0:16;
k = 0:500;
w = (k/500)*2*pi*0.5*2800;

H = dtft(nh,h,k);

figure;
plot(w/(2*pi*2800),abs(H)/max(abs(H))); title('frequency response'); xlabel('frequency expressed as a fraction of fs'); ylabel('magnitude'); axis([0 0.5 0 1.2]);

% Note the small stopband ripple characteristic of a Blackman window

% Visual simililarity to a sinusoid is good. 

% Overall, both interpolations show acceptable analog reconstruction (the
% code works!!!!!)

