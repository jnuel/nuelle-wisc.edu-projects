 function [mask, result_img] = backwardWarpImg(src_img, resultToSrc_H,...
                                               dest_canvas_width_height)
	src_height = size(src_img, 1);
	src_width = size(src_img, 2);
    src_channels = size(src_img, 3);
    dest_width = dest_canvas_width_height(1);
	dest_height	= dest_canvas_width_height(2);
    
    result_img = zeros(dest_height, dest_width, src_channels);
    mask = false(dest_height, dest_width);
    
    % this is the overall region covered by result_img
    [dest_X, dest_Y] = meshgrid(1:dest_width, 1:dest_height);
    
    % map result_img region to src_img coordinate system using the given
    % homography.
    src_pts = applyHomography(resultToSrc_H, [dest_X(:), dest_Y(:)]);
    src_X = reshape(src_pts(:,1), dest_height, dest_width);
    src_Y = reshape(src_pts(:,2), dest_height, dest_width);
    
    % ---------------------------
    % START ADDING YOUR CODE HERE
    % ---------------------------
    
    % Set 'mask' to the correct values based on src_pts.
    % extract red green and blue colors from the source image
    rs = src_img(:, :, 1);
    gs = src_img(:, :, 2);
    bs = src_img(:, :, 3);
    
    % interpolate those colors with the src pts
    rd = interp2(1 : src_width, 1 : src_height, rs, src_X, src_Y);
    gd = interp2(1 : src_width, 1 : src_height, gs, src_X, src_Y);
    bd = interp2(1 : src_width, 1 : src_height, bs, src_X, src_Y);
    
    result_img(:, :, 1) = rd;result_img(:, :, 2) = gd;result_img(:, :, 3) = bd;
    
    result_img(isnan(result_img)) = 0;
    
    % For the mask, set the image boundaries by finding corners and fill it
    % in with poly2mask
    src_corners = [1, 1; 1, src_height; src_width, src_height; src_width, 1; 1, 1];
    dest_corners = applyHomography(inv(resultToSrc_H),src_corners);
    
    mask = im2double(poly2mask(dest_corners(:, 1), dest_corners(:, 2), dest_height, dest_width));

    result_img = result_img .* cat(3, mask, mask, mask);
    % ---------------------------
    % END YOUR CODE HERE    
    % ---------------------------
end
