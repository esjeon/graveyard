%% region_ellipse.m - visualizes ellipse measurement from regionprops
%
% Copyright © 2013 Eon S. Jeon <esjeon@live.com>
% 
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the “Software”), to
% deal in the Software without restriction, including without limitation the
% rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
% sell copies of the Software, and to permit persons to whom the Software is
% furnished to do so, subject to the following conditions:
% 
% The above copyright notice and this permission notice shall be included in
% all copies or substantial portions of the Software.
% 
% THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
% IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
% FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
% AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
% LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
% FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
% DEALINGS IN THE SOFTWARE.
% 

clear all
close all

I = rgb2gray(imread('cancer.jpg'));
stats = regionprops(I, {'Extrema' 'MajorAxisLength' 'MinorAxisLength' 'Orientation' 'PixelList' 'Area' 'Centroid'});
[maxval idx] = max([stats.Area]);

p = stats(idx);

figure;
imagesc(I);
colormap('gray');
hold on;

% center
cx = p.Centroid(1);
cy = p.Centroid(2);
plot (cx,cy, '*r');


% Axis
t = p.Orientation / 180 * pi; % in rad
a = p.MajorAxisLength / 2;
b = p.MinorAxisLength / 2;

R = [cos(t) sin(t);
     -sin(t) cos(t)];


phi = linspace(0,2*pi,50);
points = R * [a*cos(phi); b*sin(phi)];
plot (cx+points(1,:), cy+points(2,:), 'LineWidth', 2, 'Color', 'Red');


% Axis
dA = R * ([1 -1; 0  0] * a);
dB = R * ([0  0; 1 -1] * b);

figure;
subplot (1,2,1); colormap('gray'); imagesc(I); imdistline (gca, cx+dB(1,:), cy+dB(2,:));
subplot (1,2,2); colormap('gray'); imagesc(I); imdistline (gca, cx+dA(1,:), cy+dA(2,:));


