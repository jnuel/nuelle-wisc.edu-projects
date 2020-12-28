function result = computeFlow(img1, img2, search_radius, template_radius, grid_MN)
    % Check images have the same dimensions, and resize if necessary
    if find(size(img2) ~= size(img1))
        img2 = imresize(img2, size(img1));
    end
    % Get number of rows and cols for output grid
    M = grid_MN(1);
    N = grid_MN(2);

    [H, W] = size(img1);
    % locations where we estimate the flow
    grid_y = round(linspace(search_radius+1, H-(search_radius+1), M));
    grid_x = round(linspace(search_radius+1, W-(search_radius+1), N));
    
    % allocate matrices where we will store the computed optical flow
    U = zeros(1, M*N);    % horizontal motion
    V = zeros(1, M*N);    % vertical motion
    x = zeros(1,M*N);
    y = zeros(1,M*N);
    
    step_y = H/M;
    step_x = W/N;

    ind = 1;
    % compute flow for each grid patch
    for i = 1:step_y:H
        for j = 1:step_x:W
            %------------- PLEASE FILL IN THE NECESSARY CODE WITHIN THE FOR LOOP -----------------
            % Note: Wherever there are questions mark you should write
            % code and fill in the correct values there. You may need
            % to write more lines of code to obtain the correct values to 
            % input in the questions marks.
            
            % extract the current patch/window (template)
            temp_ymin = max(1, i-template_radius);
            temp_ymax = min(size(img1,1),i+template_radius);
            temp_xmin = max(1, j-template_radius);
            temp_xmax = min(size(img1,2),j+template_radius);
            template = img1(temp_ymin:temp_ymax,temp_xmin:temp_xmax);
            % where we'll look for the template
            search_ymin = max(1, i-search_radius);
            search_ymax = min(size(img2,1),i+search_radius);
            search_xmin = max(1,j-search_radius);
            search_xmax = min(size(img2,2),j+search_radius);
            search_area = img2(search_ymin:search_ymax,search_xmin:search_xmax);

            % compute correlation
            corr_map = normxcorr2(template,search_area);
            
            % Look at the correlation map and find the best match
            % The best match will have the Maximum Correlation value
            
%             [~, max_ind] = max(corr_map(:));
            % Convert the index into row and col
            corr_map = corr_map(size(template, 1) : size(corr_map, 1) - size(template, 1), size(template, 2) : size(corr_map, 2) - size(template, 2));
            [max_ind_row, max_ind_col] = find(corr_map == max(corr_map(:)));
%             [max_ind_row, max_ind_col] = ind2sub(size(corr_map), max_ind);
            
            % express peak location as offset from template location
            U(ind) =  max_ind_col(1) - (temp_xmin-search_xmin+1);
            V(ind) = max_ind_row(1) - (temp_ymin-search_ymin+1);
            x(ind) = j;
            y(ind) = i;
            ind = ind + 1;

        end
    end
    
    % Any post-processing or denoising needed on the flow
    
    % plot the flow vectors
    fig = figure();
    imshow(img1);
    hold on; quiver(x,y,U,V, 2, 'y', 'LineWidth', 1.3);
%     hold on; quiver(grid_x, grid_y, U, V, 2, 'y', 'LineWidth', 1.3);
    % From https://www.mathworks.com/matlabcentral/answers/96446-how-do-i-convert-a-figure-directly-into-an-image-matrix-in-matlab-7-6-r2008a
    frame = getframe(gcf);
    result = frame2im(frame);
    hold off;
    close(fig);
end