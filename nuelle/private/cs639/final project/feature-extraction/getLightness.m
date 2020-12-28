function mean_lightness = get_lightness(img_name) 

img_color = imread(img_name);
mean_lightness = round(mean2(img_color));
img_grayscale = rgb2gray(img_color);
mean_grayscale = round(mean2(img_grayscale));

fprintf('Mean RGB lightness: %d\n', mean_lightness)
fprintf('Mean grayscale lightness: %d\n', mean_grayscale);
