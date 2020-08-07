function y = my_convolution(x,h)
% DESCRIPTION
%  performs a convolution of x with h
%
%  Total length of the convolution is length(h)+length(x)-1
%
%  ex. y = my_convolution(x,h); -- where x is time domain signal and 
%     h is the filter kernel --
%
% INPUT VARIABLES
%  x : vector
%  h : vector
%
% OUTPUT VARIABLES
%  y : vector
%
% REFERENCES
%  adapted from Dr. Steven W. Smith, Ph.D., author of 
%  "The Scientist and Engineer's Guide to Digital Signal Processing" (p.121)
%  can be accessed: http://www.dspguide.com/
%
% DOCUMENTATION
%  ver 1.0 by Dominic Meads  3/9/2020
%  filename: my_convolution.m

y = zeros(1,length(x)+length(h));  % pre-allocate memory w/ approp. length

for i = 1:(length(x)+length(h))
    for j = 1:length(h)
        if (i-j < 1)    % indecies cannot be zeros :(
            j = j + 1;
        elseif (i-j > length(x)) 
            j = j + 1;
        else
            y(i) = y(i)+h(j)*x(i-j);  % new y(i) = previous y(i) + multiply
        end % end if-else
    end % end for-j
end % end for-i
