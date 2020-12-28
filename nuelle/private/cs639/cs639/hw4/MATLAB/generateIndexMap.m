function index_map = generateIndexMap(gray_stack, w_size)
    [H, W, N] = size(gray_stack);
    sml_values = zeros(size(gray_stack));
    % Compute the focus measure -- the sum-modified laplacian
    %
    % horizontal Laplacian kernel
    Kx = [0.25 0 0.25;...
           1  -3   1; ...
          0.25 0 0.25];
    Ky = Kx';   % vertical version
    
    % horizontal and vertical Laplacian responses
    Lx = zeros(H, W, N);
    Ly = zeros(H, W, N);
    average_filter = fspecial('average',2*w_size); 
    for n = 1:N
        I = im2double(gray_stack(:,:,n));
        Lx(:,:,n) = imfilter(I, Kx, 'replicate', 'same', 'corr');
        Ly(:,:,n) = imfilter(I, Ky, 'replicate', 'same', 'corr');
        SML = abs(Lx) + abs(Ly);
        sml_values(: , : , n) = imfilter(SML(: , : , n) , average_filter, 'replicate');
    end

    % sum-modified Laplacian
%     SML = (abs(Lx) .^ 2) + (abs(Ly) .^ 2);
    % can also use the absolute value itself
    % this is probably more well-known
    % SML = abs(Lx) + abs(Ly);
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%     % ADD YOUR CODE HERE
    [~, index_map] = max(sml_values, [], 3);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end