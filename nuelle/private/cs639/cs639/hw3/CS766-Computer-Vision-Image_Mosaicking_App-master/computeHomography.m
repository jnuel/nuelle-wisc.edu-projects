function H_3x3 = computeHomography(src_pts_nx2, dest_pts_nx2)


% src_pts_nx2 and dest_pts_nx2 are the coordinates of corresponding points 
% of the two images, respectively. src_pts_nx2 and dest_pts_nx2 
% are nx2 matrices, where the first column contains
% the x coodinates and the second column contains the y coordinates.
%
% H, a 3x3 matrix, is the estimated homography that 
% transforms src_pts_nx2 to dest_pts_nx2. 

xs = src_pts_nx2(:,1);
ys = src_pts_nx2(:,2);

xd = dest_pts_nx2(:,1);
yd = dest_pts_nx2(:,2);

A  = zeros(8, 9);

A(1,:) = [ xs(1), ys(1), 1, 0, 0, 0, -xd(1)*xs(1), -xd(1)*ys(1), -xd(1)  ];
A(2,:) = [ 0, 0, 0,  xs(1), ys(1), 1, -yd(1)*xs(1), -yd(1)*ys(1), -yd(1) ];

A(3,:) = [ xs(2), ys(2), 1, 0, 0, 0, -xd(2)*xs(2), -xd(2)*ys(2), -xd(2) ];
A(4,:) = [ 0, 0, 0, xs(2), ys(2), 1, -yd(2)*xs(2), -yd(2)*ys(2), -yd(2) ];

A(5,:) = [ xs(3), ys(3), 1, 0, 0, 0, -xd(3)*xs(3), -xd(3)*ys(3), -xd(3) ];
A(6,:) = [ 0, 0, 0, xs(3), ys(3), 1, -yd(3)*xs(3), -yd(3)*ys(3), -yd(3) ];

A(7,:) = [ xs(4), ys(4), 1, 0, 0, 0, -xd(4)*xs(4), -xd(4)*ys(4), -xd(4) ];
A(8,:) = [ 0, 0, 0, xs(4), ys(4), 1, -yd(4)*xs(4), -yd(4)*ys(4), -yd(4)  ];

[V,D] = eig(A'*A);
H_3x3 = reshape(V(:,1),3,3)';
