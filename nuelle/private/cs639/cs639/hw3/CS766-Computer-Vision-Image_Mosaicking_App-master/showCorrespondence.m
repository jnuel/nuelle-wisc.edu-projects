function result_img = ...
    showCorrespondence(orig_img, warped_img, src_pts_nx2, dest_pts_nx2)


I = [orig_img, warped_img];

fh2 = figure; imshow(I);

hold on

szOrg = size(orig_img);


sz = size(src_pts_nx2);
n = sz(1);

X = [];
Y = [];

for i = 1:1:n
    X = [X;src_pts_nx2(i,1), szOrg(2) + dest_pts_nx2(i,1)];
    Y = [Y; src_pts_nx2(i,2),  dest_pts_nx2(i,2)];
end


for i = 1:1:length(X)
    line(X(i,:), Y(i,:),...
            'LineWidth',2, 'Color', [1, 0, 0]);
    hold on;
end

hold off
annotated_img = saveAnnotatedImg(fh2);




result_img = annotated_img;
delete(fh2);




%%
function annotated_img = saveAnnotatedImg(fh)
figure(fh); 

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
