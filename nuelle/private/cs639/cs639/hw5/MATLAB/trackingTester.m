function trackingTester(data_params, tracking_params)
    % Useful function to get ROI from the img 
    function roi = get_ROI(img, rect)
        % just a convenience function
        xmin = rect(1);
        ymin = rect(2);
        width = rect(3);
        height = rect(4);
%         roi = img(ymin:ymin+height-1, xmin:xmin+width-1,:);
        roi = img(ymin:ymin+height, xmin:xmin+width,:);

    end
    
    % Verify that output directory exists
    if ~exist(data_params.out_dir, 'dir')
        fprintf(1, "Creating directory %s.\n", data_params.out_dir);
        mkdir(data_params.out_dir);
    end
    trackingbox_color = [255, 255, 0];
    % Load the first frame, draw a box on top of that frame, and save it.
    first_frame = imread(fullfile(data_params.data_dir, data_params.genFname(1)));
    annotated_first_frame = drawBox(first_frame, tracking_params.rect, trackingbox_color, 3);
    imwrite(annotated_first_frame, fullfile(data_params.out_dir, data_params.genFname(1)));
    
    % take the ROI from the first frame and keep its histogram to match
    % later
    src_template = get_ROI(first_frame, tracking_params.rect);
    
    
    %------------- FILL IN CODE -----------------
    % Create the intensity histogram using the obj_roi that was extracted
    % above.
    
    obj_rect = tracking_params.rect;
    num_hist_bins = tracking_params.bin_n;
    search_radius = tracking_params.search_radius;

    [src_temp_ind, src_temp_map] = rgb2ind(src_template, num_hist_bins);
    obj_hist = histcounts(src_temp_ind(:), 1:num_hist_bins);
    %------------- END FILL IN CODE -----------------

	% OPTIONAL: You can do the matching in color too.
	% Use the rgb2ind function to transform the image such that it only has a
    % fixed number of colors (much less than 256^3). If you visualize the mapped_obj 
    % image it should look similar to the obj_roi image, but with much less
    % variations in the color (the palette's size is tracking_params.bin_n).
    % The output colormap will tell you which colors were chosen to be used in 
    % the mapped_obj output image.
    % NOTE: If you want to do this, you have to do it consistently for all
    % frames!
    % [mapped_obj, colormap] = rgb2ind(obj_roi, tracking_params.bin_n);
    % Create a color histogram from the mapped_obj image that has the colors
    % quantized.
    % Hint: If the mapped_obj image has Q different colors, then your
    % histogram will have Q bins, one for each color.
    % obj_hist = ??
    
    % Normalize histogram such that its sum = 1
%     obj_hist = double(obj_hist) / sum(obj_hist(:));

    % Tracking loop
    % -------------    
    % initial location of tracking box
    obj_col = tracking_params.rect(1);
    obj_row = tracking_params.rect(2);
    obj_width = tracking_params.rect(3);
    obj_height = tracking_params.rect(4);
    frame_ids = data_params.frame_ids;
    
    for frame_id = frame_ids(2:end)
        
        frame = imread(fullfile(data_params.data_dir, data_params.genFname(frame_id)));
        [ht, wid, ~] = size(frame);
        %------------- FILL IN CODE -----------------

        x_min = max(1, obj_col - search_radius);
        x_max = min(wid, obj_col + obj_width + search_radius);
        y_min = max(1, obj_row - search_radius);
        y_max = min(ht, obj_row + obj_height + search_radius);
        
        window = frame(y_min:y_max, x_min:x_max, :);
        
        winds(:, :, 1) = im2col(window(:, :, 1), [obj_rect(4), obj_rect(3)]);
        winds(:, :, 2) = im2col(window(:, :, 2), [obj_rect(4), obj_rect(3)]);
        winds(:, :, 3) = im2col(window(:, :, 3), [obj_rect(4), obj_rect(3)]);
        m_sim = zeros(size(window, 2), 1);
        for j = 1 : size(winds, 2)
            curr_wind_ind = rgb2ind(winds(:, j, :), src_temp_map);
            curr_wind_hist = histcounts(curr_wind_ind(:), 1:num_hist_bins);
            
            m_sim(j) = norm(obj_hist - curr_wind_hist);
        end
        
        %------------- FILL IN CODE -----------------
        
        % find the best-matching region
        [~, idx] = min(m_sim);
        
        div = size(window, 1) - obj_rect(4) + 1;
        rec_x = floor(idx / div) + x_min;
        rec_y = round(mod(idx, div) + y_min);
        if rec_y == y_min
            rec_y = div + y_min;
        end
       
%       UPDATE the obj_row and obj_col, which tell us the location of the
        % top-left pixel of the bounding box around the object we are
        % tracking.
        obj_col = rec_x;
        obj_row = rec_y;
        
        %------------- END FILL IN CODE -----------------

        % generate box annotation for the current frame
        annotated_frame = drawBox(frame, [obj_col obj_row obj_width obj_height], trackingbox_color, 3);
        % save annotated frame in the output directory
        imwrite(annotated_frame, fullfile(data_params.out_dir, data_params.genFname(frame_id)));
    end
end
