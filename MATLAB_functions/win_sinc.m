function h = win_sinc(tBW,fc,win);
% DESCRIPTION
%  calculates a LOW PASS windowed-sinc FIR filter kernel
%  All other frequency responses can be obtained using LPF 
%  and spectral inversion. Includes an m/2 shift to avoid 
%  negative indexes (which MATLAB does not allow).
%
%  ex. h = LP_win_sinc(tBW,fc,win);
%
% INPUT VARIABLES
%  tBW : bandwidth of transition band (expressed as decimal) MUST MAKE M
%        AN EVEN NUMBER (m = 4/tBW) ex. 0.2, 0.1, 0.05, etc...
%  fc : cuttoff frequency of filter expressed as a decimal (no greater than
%       0.5 * fsample)
%  win : this function supports both Hamming and Blackman 
%        windows. Hamming yields faster roll-off while Blackman gives 
%        an order of magnitude better stop-band attenuation. 
%        1 = hamming
%        2 = blackman
%
% OUTPUT VARIABLES
%  h : vector of double precision numbers containing the filter kernel
%
% REFERENCES
%  adapted from Dr. Steven W. Smith, Ph.D., author of 
%  "The Scientist and Engineer's Guide to Digital Signal Processing" (p.285)
%  can be accessed: http://www.dspguide.com/
%
% DOCUMENTATION
%  ver 1.0 by Dominic Meads  3/9/2020
%  filename: LP_win_sinc.m

m = 4/tBW;  % filter length of m+1 points
k = 0;  % for normalization 

h = zeros(1,m+1);  % allocates memory for a m+1 sized vector 

for i = 1:length(h)
    if i == m/2
        h(i) = 2*pi*fc;
    else
        if win == 2  % blackman
            h(i) = (sin(2*pi*fc*(i-m/2))/(i-m/2))*(0.42-0.5*cos(2*pi*i/m)+0.08*cos(4*pi*i/m));
        elseif win == 1 % hamming
            h(i) = (sin(2*pi*fc*(i-m/2))/(i-m/2))*(0.54 - 0.46*cos(2*pi*i/m));
        end % end if--window
    end % end if--i
end % end for

for i = 1:length(h)  % calculate normalization coefficient
    k = k + h(i);  
end

h = h/k;  % normalize
