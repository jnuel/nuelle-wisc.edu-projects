function [mask, result_img] = backwardWarpImg(src_img, resultToSrc_H,...
    dest_canvas_width_height)


% szP = size(src_img);
% sz = [dest_canvas_width_height(1) dest_canvas_width_height(2) 3];
% iH = resultToSrc_H;
% 
% 
% figure(1);
% bg_img = image(ones(sz));
% close(1); 
% 
% mask = uint8(zeros(dest_canvas_width_height(2), dest_canvas_width_height(1)));
% 
% for i = 1:1: sz(1)
%     for j = 1:1:sz(2)
%         v = [i;j;1];
%         a = iH*v;
%         a(1) = ceil(a(1)/a(3));
%         
%         a(2) = ceil(a(2)/a(3));
%   
%         if a(1) > 0 && a(2) > 0 && a(1) <= szP(2) &&  a(2) <= szP(1)
%             bg_img(j,i,1) = src_img(a(2),a(1),1);
%             bg_img(j,i,2) = src_img(a(2),a(1),2);
%             bg_img(j,i,3) = src_img(a(2),a(1),3);
%             mask(j,i) = 255;
%         else
%             bg_img(j,i,1) = 0;
%             bg_img(j,i,2) = 0;
%             bg_img(j,i,3) = 0;
%             mask(j,i) = 0;
%         end
%         
%     end
% end
% 
% 
% figure(1); imshow(bg_img); close(1); 
% size(bg_img);
% size(mask);
% figure(1); imshow(mask); close(1);
% result_img = bg_img;
% return;
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
    for xs = 1:dest_height
        for ys = 1:dest_width
            % map back 
            point = resultToSrc_H * [xs;ys;1];
            point = point/ point(3);
            
            x1 = round(point(1));
            x2 = round(point(2));
            
            if x1 > 0 && x2 > 0 && x1 <= src_width && x2 <= src_height
                result_img(ys,xs,:) = src_img(x2,x1,:);
                mask(ys,xs) = true;
            end
        end
    end
    
    
    % ---------------------------
    % END YOUR CODE HERE    
    % ---------------------------
end

