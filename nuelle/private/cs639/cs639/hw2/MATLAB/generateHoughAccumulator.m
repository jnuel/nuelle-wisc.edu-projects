function [H] = generateHoughAccumulator(im, theta_num_bins, rho_num_bins)
%% Initializing parameters
[height, width] = size(im);

theta = linspace(0,pi,theta_num_bins); %know start, end, num elements

D = sqrt(size(im,1).^2 + size(im,2).^2);
theta_max = theta_num_bins;
rho = linspace(-D,D,rho_num_bins);

drho = (2*D)/(rho_num_bins-1);
H = zeros(rho_num_bins, theta_num_bins);

% Do the hough transform
for y=1: height
    for x=1: width
        if im(y,x) > 0
            for t=1: theta_num_bins
                r = y*cos(theta(t)) - x*sin(theta(t));
                r_index = round((r/drho) + (rho_num_bins/2));
                H(r_index, t) = H(r_index, t)+1;
            end
        end
    end
end
H = rescale(H, 0, 255);

end