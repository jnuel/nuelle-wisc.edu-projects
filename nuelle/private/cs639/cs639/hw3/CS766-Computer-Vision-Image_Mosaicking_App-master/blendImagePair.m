function out_img = blendImagePair(wrapped_imgs, masks, wrapped_imgd, maskd, mode)
sz = size(wrapped_imgs);
imax = sz(1);
jmax = sz(2);

if strcmp(mode, 'overlay') == 1
    for i = 1:1: imax  
        for j = 1:1: jmax
            if maskd(i,j) ~= 0
                wrapped_imgs(i,j,1) = wrapped_imgd(i,j,1);
                wrapped_imgs(i,j,2) = wrapped_imgd(i,j,2);
                wrapped_imgs(i,j,3) = wrapped_imgd(i,j,3);
            end
        end
    end
    out_img = wrapped_imgs;
    
elseif strcmp(mode, 'blend') == 1
    
    blmaskd = bwdist(~maskd);
    blmaskd = cat(3, blmaskd, blmaskd, blmaskd);
    wrapped_imgd = im2double(wrapped_imgd);
    wrapped_imgd = wrapped_imgd.*blmaskd;
    

    
    blmasks = bwdist(~masks);
    blmasks = cat(3, blmasks, blmasks, blmasks);
    wrapped_imgs = im2double(wrapped_imgs);
    wrapped_imgs = wrapped_imgs.*blmasks;

    
    out_img = (wrapped_imgs + wrapped_imgd)./(blmasks + blmaskd);
    
    
    
end

