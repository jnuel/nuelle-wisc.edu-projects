function M = applyGetPLightness(D,S)
% D = '/Users/julkroll/Documents/jk/grad-ms/fall2020-courses/computer-vision/cv-final-project/joynuelle.github.io-cs639/Datasets/Ariana Grande'; % mix of jpg and jpeg
% S = [dir(fullfile(D,'*.jpeg'));dir(fullfile(D,'*.jpg'))];  % pattern to match filenames.
numImages = numel(S);
maxImg = 0;
minImg = realmax;
listImg = zeros([numImages, 1]);
curMax = 0; 
curMin = 0;
M = containers.Map('KeyType','char','ValueType','double');

for imgNum = 1:numImages
    F = fullfile(D,S(imgNum).name);
    feature_value = getPerceivedLightness(F);
    if feature_value > maxImg
       maxImg = feature_value;
       curMax = imgNum;
    end
    if feature_value < minImg
        minImg = feature_value;
        curMin = imgNum;
    end
    listImg(imgNum) = feature_value;
    M(S(imgNum).name) = feature_value;
end

% medianImg = median(listImg);
% curMedian = find(listImg==medianImg, 1, 'first');
% 
% figure('Name','Measured Data');
% F = fullfile(D,S(curMax).name);
%     img = imread(F);
%     subplot(1,3,1), imshow(img);
%     title(['Lightest perceived image: ', num2str(curMax), ' (value = ', num2str(maxImg), ')']);
% F = fullfile(D,S(curMedian).name);
%     img = imread(F);
%     subplot(1,3,2), imshow(img);
%     title(['Median perceived image: ', num2str(curMedian), ' (value = ', num2str(medianImg), ')']);
% F = fullfile(D,S(curMin).name);
%     img = imread(F);
%     subplot(1,3,3), imshow(img);
%     title(['Darkest perceived image: ', num2str(curMin), ' (value = ', num2str(minImg), ')']);