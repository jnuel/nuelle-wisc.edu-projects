function [rgb_stack, gray_stack] = loadFocalStack(focal_stack_dir)
% rgb_stack is an mxnx3k matrix, where m and n are the height and width of
% the image, respectively, and 3k is the number of images in a focal stack
% multiplied by 3 (each image contains RGB channels). 
%
% rgb_stack will only be used for the refocusing app viewer 
%
% gray_stack is an mxnxk matrix.
all_files = dir(fullfile(focal_stack_dir,'*.jpg'));  % Find all filenames ending in '*.jpg'
names = {all_files.name};

% Find the number of images
num_imgs = size(names, 2);

% Find the height and width of the first image to create m and n
img1 = imread(fullfile(focal_stack_dir, names{1}));
sz = size(img1);
m = sz(1);
n = sz(2);

rgb_stack = uint8(zeros(m,n,3*num_imgs));
gray_stack = uint8(zeros(m,n,num_imgs));

for i = 1: num_imgs
    imgi = imread(fullfile(focal_stack_dir, names{i}));
    rgb_stack(:,:, 3*(i-1)+1 : 3*i) = imgi;
    gray_imgi = rgb2gray(imgi);
    gray_stack(:,:, i) = gray_imgi;
end
