function dest_pts_nx2 = applyHomography(H_3x3, src_pts_nx2)
sz = size(src_pts_nx2);
src = [src_pts_nx2'; ones(1,sz(1))];
d = H_3x3*src;

dest_pts_nx2 = zeros(sz(1),2);
for i = 1:1:sz(1)
    dest_pts_nx2(i,:) = [d(1,i)/d(3,i), d(2,i)/d(3,i)];
end

