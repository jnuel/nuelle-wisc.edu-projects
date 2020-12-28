function amount_of_color = getUniqueColorCount(D,S)
amount_of_color = containers.Map('KeyType','char','ValueType','double');

maxImg = 0;
minImg = 0;
curMax = 0; 
curMin = realmax;
for imgNum = 1:numel(S)
    F = fullfile(D,S(imgNum).name);
%     disp(S(imgNum).name);
    image = imread(F);
   
    % Number of unique colors in each image
    image_as_rows = reshape(image, [], 3);
    unique_colors = unique(image_as_rows, 'rows');
    num_unique_colors = size(unique_colors, 1);
%     disp(num_unique_colors);
    if num_unique_colors > curMax
        curMax = num_unique_colors;
        maxImg = imgNum;
    end
    if num_unique_colors < curMin
        curMin = num_unique_colors;
        minImg = imgNum;
    end
    
    amount_of_color(S(imgNum).name) = num_unique_colors;
end

% Display the most unique picture and least unique color-wise.
figure('Name','Measure of Unique Colors');
F = fullfile(D,S(maxImg).name);
    img = imread(F);
    subplot(1,2,1), imshow(img);
    title(['Most colored image: ', num2str(maxImg)]);
F = fullfile(D,S(minImg).name);
    img = imread(F);
    subplot(1,2,2), imshow(img);
    title(['Least image: ', num2str(minImg)])

