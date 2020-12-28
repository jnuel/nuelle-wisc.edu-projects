function [inliers_id, H] = runRANSAC(Xs, Xd, ransac_n, eps)
%RUNRANSAC
    num_pts = size(Xs, 1);
    pts_id = 1:num_pts;
    inliers_id = [];
    H = zeros(3,3);
    best_fit = 0;
    
    for iter = 1:ransac_n
        % ---------------------------
        % START ADDING YOUR CODE HERE
        % ---------------------------
        % generate the 4 rand points to calculate an example H from
        rand_index = randi(num_pts, 4, 1);
        randxs = Xs(rand_index, :);
        randxd = Xd(rand_index, :);
        % compute the h based off these rando points
        H_rand = computeHomography(randxs, randxd);
        
        % decide how good the H is based offapplying the H on Xs, compare
        % found Xd values w/Euclidean distance
        found_Xd = applyHomography(H_rand, Xs);
        dist = sqrt((Xd(:,1)-found_Xd(:,1)).^2 + (Xd(:,2) - found_Xd(:,2)).^2);
        
        inlier = find(dist < eps);
        if length(inlier) > best_fit
            best_fit = length(inlier);
            H = H_rand;
            inliers_id = inlier;
        end
%         A = H * B; % A and B are 3 x n matrices and H is a 3 x 3 matrix
%         dist = sqrt(sum((A-B).^2)); % A and B are nx3 matrices and dist is a 1xn % matrix
%         inds = randperm(n, 4); % inds is a vector of 4 random unique integers % in [1, n]
        % ---------------------------
        % END ADDING YOUR CODE HERE
        % ---------------------------
    end
end
