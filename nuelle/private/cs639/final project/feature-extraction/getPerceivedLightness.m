function mean_perceived = getPerceivedLightness(img_name) 

% Get starting image
img_color = imread(img_name);

% Scale image from RGB to (0,1) plus gamma encoding
img_scaled = double(img_color)/255.0;
img_gamma_encoded = power(img_scaled, 2.2);

% Initialize empty array and calculate luminance per color channel
img_perceived = zeros(size(img_gamma_encoded));
img_perceived(:,:,1) = img_gamma_encoded(:,:,1) * 0.2126;
img_perceived(:,:,2) = img_gamma_encoded(:,:,2) * 0.7152;
img_perceived(:,:,3) = img_gamma_encoded(:,:,3) * 0.0722;

% Mean luminance (0,1)
mean_perceived = mean2(img_perceived);

% fprintf('Mean perceived lightness: %.4f\n', mean_perceived);