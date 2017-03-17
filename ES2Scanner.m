clear 
close all
load('lidar12.csv');

ulimit = 400;
G = ulimit-lidar12;
G = mat2gray(G, [0 ulimit]);
G = rot90(G);

set(gcf, 'colormap', gray);
contourf(G, 10);


