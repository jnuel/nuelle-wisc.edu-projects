function refocusApp(rgb_stack, depth_map)

% Find max image dimensions
[maxH, maxW] = size(rgb_stack);
f1 = figure;
% Display the first image
imshow(rgb_stack(:, :, 1:3));

%Getting the selected point
[y,x] = ginput(1);

while ~isempty(x) && ~isempty(y) && 1 <= x &&  1 <= y && y <= maxW && x <= maxH
    index = depth_map(ceil(x), ceil(y));
    RGB_left = 1 + (index-1)*3;
    RGB_right = index*3;
    %Showing the new image based on selection
    imshow(rgb_stack(:,:,RGB_left:RGB_right));
    
    %Getting new points
    [y,x] = ginput(1);
end
close(f1);