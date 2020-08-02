%%  Test

% set up test signal
fs = 500;           % sampling frequency
Ts = 1/fs;          % Sampling Period
n = 0:fs-1;          % Samples
t = n*Ts;           % sampling instances
x = sin(5*2*pi*t) + 0.5*sin(200*2*pi*t);  % signal

figure('Color',[1 1 1]);
b = plot(n,x,'.');
grid on;
xlabel('sample #');
ylabel('f[n]');
set(b,'Color', [1 0.4 0.4]);
set(b,'MarkerSize',8);
hold on;
b = plot(n,x);
box off;
set(b,'Linewidth',.5);
set(b,'Color', [0.4 0.4 1]);

figure;
%stem(x);
stem(x,'Linestyle','none');
% --------------------------------------------------


% kernel from C
fID = fopen('filter_kernel.txt','r');  % open file for reading
data = textscan(fID,'%s');  % place the data in a cell: "Cell arrays commonly contain either lists of character vectors of different lengths"
fclose(fID);

h = str2double(data{1})';  % convert elements in the cell to type "double"

n1 = 0:16;
figure;
stem(n1,h);


% convolve filter
xout = convolution(x,h);
figure;
% stem(xout);
stem(xout,'Linestyle','none');


%%  MATLAB check

% check kernel
h_check = win_sinc(0.25,0.02,2);
figure;
stem(n1,h_check);

% convolve filter
xout_check = convolution(x,h_check);
figure;
% stem(xout_check);
stem(xout_check,'Linestyle','none');
