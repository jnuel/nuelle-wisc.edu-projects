function runFinalProject(varargin)
% runFinalProject is the "main" interface that lets you execute all the 
% functions for our final project.
%
% Usage:
% runFinalProject                       : list all the registered functions
% runFinalProject('function_name')      : execute a specific test
% runFinalProject('all')                : execute all the registered functions

% Settings to make sure images are displayed without borders.
orig_imsetting = iptgetpref('ImshowBorder');
iptsetpref('ImshowBorder', 'tight');
temp1 = onCleanup(@()iptsetpref('ImshowBorder', orig_imsetting));

fun_handles = {@getData};
% Make sure you download runTests.m as well!
% Call test harness
runTests(varargin, fun_handles);

%%
function getData()
% D = uigetdir; % lets the user choose which folder of images they want to analyze
% D = 'datasets/celebrities/sandra_oh'; %jpeg 
% D = 'datasets/celebrities/nick_jonas'; % mix of jpg and jpeg
D = '/Users/JoyNuelle/Desktop/cs639/final_project/datasets/Julia1';
S = [dir(fullfile(D,'*.jpeg'));dir(fullfile(D,'*.jpg'))];  % pattern to match filenames.

if numel(S) == 0
    disp("Didn't find any photos to analyze...");
else
    blur_container = getBlur(D,S); % returns amount of blur w/img title name
    amount_of_color_container = getUniqueColorCount(D,S); % returns amount of unique colors w/img title name
    amount_of_lightness_container = applyGetPLightness(D,S); % returns amount of lightness w/img title name
    eyes_open_container = getEyesOpen(D,S); % returns if eyes are open w/ img title name
    rot_container = getRuleOfThirds(D,S);
    
    [~,~, curMax, curMin, ~] = rank(S, blur_container, amount_of_color_container, amount_of_lightness_container, eyes_open_container, rot_container);
    figure('Name','Best and Worst Image');
    F = fullfile(D,S(curMax).name);
        img = imread(F);
        subplot(1,2,1), imshow(img);
        title(['Overall highest ranked image: ', S(curMax).name]);
    F = fullfile(D,S(curMin).name);
        img = imread(F);
        subplot(1,2,2), imshow(img);
        title(['Overall lowest ranked image: ', S(curMin).name]);
end

%% rank the images based off the data we got back
function [maxImg, minImg, curMax, curMin, final_ranks] = rank(S, bc, cc, lc, eoc, rotc)
maxImg = 0;
minImg = realmax;
curMax = 0; 
curMin = 0;

num_imgs = numel(S);
final_ranks = containers.Map('KeyType','char','ValueType','double');
% disp(num_imgs);

for imgNum = 1:numel(S)
    disp(S(imgNum).name);
    final_ranks(S(imgNum).name) = 0;
    
%     Blur count number / num images is the score for this image
    blur_count = bc(S(imgNum).name);
    fprintf('Blur count: %.4f\n', blur_count);
    final_ranks(S(imgNum).name) = final_ranks(S(imgNum).name) + blur_count;
    
%     Hue count number / num images is the score for this image
    hue_count = cc(S(imgNum).name) / 1000;
    fprintf('Hue count: %.4f\n', hue_count);
    final_ranks(S(imgNum).name) = final_ranks(S(imgNum).name) + hue_count;
    
%      Lightness count number / num images is the score for this image
    light_count = lc(S(imgNum).name) * 100;
    fprintf('Light count: %.4f\n', light_count);
    final_ranks(S(imgNum).name) = final_ranks(S(imgNum).name) + light_count;
    
    % Eyes open T/F = +100, or - 100 is the score for this image
    eyes_open = eoc(S(imgNum).name);
    if eyes_open
        final_ranks(S(imgNum).name) = final_ranks(S(imgNum).name) + 100;
    else
        final_ranks(S(imgNum).name) = final_ranks(S(imgNum).name) - 100;
    end
    fprintf('With eyes open: %.4f\n', final_ranks(S(imgNum).name));
    % Rule of Thirds T/F = +100, or - 100 is the score for this image

    rule_of_thirds = rotc(S(imgNum).name);
    if rule_of_thirds
        final_ranks(S(imgNum).name) = final_ranks(S(imgNum).name) + 20;
    end
    fprintf('Final score with rule of thirds: %.4f\n', final_ranks(S(imgNum).name));

    
    if final_ranks(S(imgNum).name) > maxImg
        maxImg = final_ranks(S(imgNum).name);
        curMax = imgNum;
    end
    if final_ranks(S(imgNum).name) < minImg
        minImg = final_ranks(S(imgNum).name);
        curMin = imgNum;
    end
end


keySet = keys(final_ranks);
valueSet = values(final_ranks);
valueSet = cell2mat(valueSet);
[sortedValues, sortIdx] = sort( valueSet );
sortedKeys = keySet( sortIdx );

for imgNum = 1:numel(S)
    fprintf('%.f -> image %s: %.4f\n', (numel(S)+1)-imgNum, string(sortedKeys(imgNum)), sortedValues(imgNum));
end
% for imgNum = 1:numel(S)
%     fprintf('%.f -> image %s: %.4f\n', imgNum, string(keySet(imgNum)), final_ranks(S(imgNum).name));
% end





