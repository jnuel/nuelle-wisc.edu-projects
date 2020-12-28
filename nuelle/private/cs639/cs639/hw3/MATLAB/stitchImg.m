function stitched_img = stitchImg(varargin)
ransac_n = 100;
ransac_eps = 3;
imgcenter = varargin{1};
size_imgc = size(imgcenter);
numimgs = nargin;

for imgnum = 2: numimgs
    cur_img = varargin{imgnum};
    [source_pts, dest_pts] = genSIFTMatches(cur_img, imgcenter);
    [~, H] = runRANSAC(source_pts, dest_pts, ransac_n, ransac_eps);
    % Apply homographies for both left and right images
    size_src = size(cur_img);
    src_corners = [1,1; size_src(2),1; size_src(2), size_src(1); 1, size_src(1)];
    dest_corners = applyHomography(H, src_corners);
    minx = round(min(dest_corners(:,1)));
    maxx = round(max(dest_corners(:, 1)));
    miny = round(min(dest_corners(:, 2)));
    maxy = round(max(dest_corners(:, 2)));
    size_center = size(imgcenter);
    translatex = 0; translatey = 0;
    if minx < 0
        imgcenter = horzcat(zeros(size_center(1), -minx, 3), imgcenter);
        translatex = -minx;
    elseif maxx > size_center(2)
        imgcenter = horzcat(imgcenter, zeros(size(imgcenter,1), maxx-size_center(2), 3));
    end
    if miny < 0
        imgcenter = vertcat(zeros(-miny, size(imgcenter, 2), 3), imgcenter);
        translatey = -miny;
    elseif maxy > size_center(1)
        imgcenter = vertcat(imgcenter, zeros(maxy - size_center(1), size(imgcenter,2),3));
    end
    dest_wxh = [size(imgcenter,2), size(imgcenter,1)];
    H_new = [1, 0, translatex; 0, 1, translatey; 0, 0, 1] * H;
    [mask, res_img] = backwardWarpImg(cur_img, inv(H_new), dest_wxh);
    mask_ref = imgcenter(:, :, 1);
    mask_ref(imgcenter(:, :, 1) > 0) = 1;
    imgcenter = blendImagePair(res_img, mask, imgcenter, mask_ref, 'blend');
    imgcenter(isnan(imgcenter)) = 0;
end
stitched_img = imgcenter;
end
     