% function eyes_open = getEyesOpen(img_name)
function eyes_open_container = getEyesOpen(D,S)
eyes_open_container = containers.Map('KeyType','char','ValueType','double');

for imgNum = 1:numel(S)
%     disp(S(imgNum).name);
    %path = '../Datasets/Trevor Noah/trevor15.jpeg';
%     logical_str = {'false', 'true'};
%     eyes_titles = {'Eyes closed', 'Eyes open'};
    F = fullfile(D,S(imgNum).name);
    img = imread(F);
    img_gray = rgb2gray(img);
    
    
    % Get subject's face
    FaceDetector = vision.CascadeObjectDetector();
    FaceDetector.MergeThreshold = 20;
    BB = step(FaceDetector, img); % BB = [x-coord, y-coord, x-addition, y-addition]
    
    % Try profile if frontal face is not found
    if size(BB,1) == 0
        FaceDetector = vision.CascadeObjectDetector('ProfileFace');
        BB = step(FaceDetector, img);
%         if size(BB,1) > 0
% %             display('Success with profile face');
%         end
    end
    
     % If multiple boxes are found, use the largest one as the portrait face
    if size(BB,1) > 1
        maxBoxDim = 0;
        boxIndex = 1;
        for i = 1:size(BB,1)
            boxDim = BB(i,3);
            if boxDim > maxBoxDim
               maxBoxDim = boxDim;
               boxIndex = i;
            end
        end
        BB = BB(boxIndex,:);
    end
    
%      I= imread('img.jpg');
     EyeDetect = vision.CascadeObjectDetector('EyePairBig');
     EyeDetect.UseROI = true;
%      EyeDetect.roi = BB;
     BBeyes = EyeDetect(img_gray, BB);
%      BBeyes=step(EyeDetect,img_gray);
     
     if isempty(BBeyes)
         eyes_open_container(S(imgNum).name) = false;
%          figure('Name','Eyes closed');
%          imshow(img);
%          disp('false');
     else
          % If multiple boxes are found, use the largest one as the portrait face
        if size(BBeyes,1) > 1
            maxBoxDim = 0;
            boxIndex = 1;
            for i = 1:size(BBeyes,1)
                boxDim = BBeyes(i,3);
                if boxDim > maxBoxDim
                   maxBoxDim = boxDim;
                   boxIndex = i;
                end
            end
            BBeyes = BBeyes(boxIndex,:);
        end
    
%         cropped_eyes = imcrop(img, BBeyes);
%         figure('Name',S(imgNum).name);
%         imshow(cropped_eyes);
        face_left = BB(1);
        face_right = BB(1) + BB(3);
        face_top = BB(2);
        face_bottom = BB(2) + BB(4);
        eye_left = BBeyes(1);
        eye_right = BBeyes(1) + BBeyes(3);
        eye_top = BBeyes(2);
        eye_bottom = BBeyes(2) + BBeyes(4);
        crosslr = eye_left > face_left && eye_right < face_right;
        crosstb = eye_top > face_top && eye_bottom < face_bottom;
        if crosslr && crosstb
            eyes_open_container(S(imgNum).name) = true;
%             disp('true');
        end

            
%          x=length(BB(:,4));
%        %To detect Eyes
%         I2 = imcrop(img,BB(x,:));
%         figure('Name',S(imgNum).name);
%         imshow(I2);

%         imshow(img);
%         eyes_open_container(S(imgNum).name) = true;
%          disp('true');
     end
end
    
% %    To detect Eyes
%     I2 = imcrop(img_gray,BB(x,:));
%     figure,imshow(I2);
%     figure, imshow(insertObjectAnnotation(img, 'rectangle', BB, 'Face'));
%     rectangle('Position',BB,'LineWidth',4,'LineStyle','-','EdgeColor','b');
%     
% %     if 
% %     eyes_open_container(S(imgNum).name) = eyes_open;
% 
%    
%     EyeDetector = vision.CascadeObjectDetector('EyePairBig'); % EyePairBig, EyePairSmall, LeftEye, RightEye
%     EyeDetector.MergeThreshold = 10;
%     BB = step(EyeDetector, img); % BB = [x-coord, y-coord, x-addition, y-addition]
%     out = insertObjectAnnotation(img, 'rectangle', BB, '');
% 
%     eyes_open = false;
% 
%     if size(BB,1) == 1 && size(BB,2) == 4
%         cropped_eyes = imcrop(img, BB);
%         imshow(cropped_eyes);
%         max_eye_pixel = max(img_gray(:));
%         min_eye_pixel = min(img_gray(:));
%         eye_spread = max_eye_pixel - min_eye_pixel;
%         fprintf('Eye spread is: %d\n', eye_spread);
%         if eye_spread > 200
%             eyes_open = true;
%         end
%     end
%     disp(eyes_open);
%     eyes_open_container(S(imgNum).name) = eyes_open;
%     % imshow(out);
% %     title([eyes_titles{eyes_open + 1}]);
% 
% %     fprintf('Eyes open: %s\n', logical_str{eyes_open + 1});
% end