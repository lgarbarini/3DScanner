clear 
close all
array = csvread('lidar12.csv') %load filename
%load('lidar12.csv');

%The LIDAR module returns 8192 when there is a timeout, this skews the dynamic range
ulimit = 400; %ulimit sets max distance to be plotted (e.g. 400 = 400mm)
G = ulimit-array; %invert the image so brighter is closer
G = mat2gray(G, [0 ulimit]); %create grayscale
G = rot90(G); %correct rotation

set(gcf, 'colormap', gray);
contourf(G, 10); %adjust number of contours depending on image complexity. Usually, 10 - 50 yields good results.


