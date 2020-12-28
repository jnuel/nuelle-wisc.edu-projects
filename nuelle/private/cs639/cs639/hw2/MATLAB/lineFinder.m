function [new_img] = lineFinder(orig_img, hough_img, hough_threshold)
fh1 = figure();
imshow(orig_img);

[rho_num_bins, theta_num_bins] = size(hough_img);
[all_y, all_x] = size(orig_img);

% in order to find rho later
theta = linspace(0,pi,theta_num_bins); %know start, end, num elements
D = sqrt(size(orig_img,1).^2 + size(orig_img,2).^2);
rhos = linspace(-D,D,rho_num_bins);

thresh = hough_threshold;
xs= linspace(1,all_x,1);
ys= [];
for y=1: rho_num_bins
    for x=1: theta_num_bins
        if hough_img(y,x) > thresh
            
            rho = rhos(y);
            a = cos(theta(x));
            b = sin(theta(x));
            x_1 = 1;
            y1 = (x_1*b + rho) / (a);
            x_2 = all_x;
            y2 = (x_2*b + rho) / (a);
            line([x_1 x_2], [y1 y2],...
                'LineWidth',2, 'Color', [0, 1, 0]);
           
        end
    end
end

new_img = saveAnnotatedImg(fh1);
delete(fh1);
end

function annotated_img = saveAnnotatedImg(fh)
figure(fh); % Shift the focus back to the figure fh

% The figure needs to be undocked
set(fh, 'WindowStyle', 'normal');

% The following two lines just to make the figure true size to the
% displayed image. The reason will become clear later.
img = getimage(fh);
truesize(fh, [size(img, 1), size(img, 2)]);

% getframe does a screen capture of the figure window, as a result, the
% displayed figure has to be in true size. 
frame = getframe(fh);
frame = getframe(fh);
pause(0.5); 
% Because getframe tries to perform a screen capture. it somehow 
% has some platform depend issues. we should calling
% getframe twice in a row and adding a pause afterwards make getframe work
% as expected. This is just a walkaround. 
annotated_img = frame.cdata;
end

