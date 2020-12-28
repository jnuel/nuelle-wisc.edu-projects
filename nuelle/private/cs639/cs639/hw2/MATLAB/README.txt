README for Hw2

Joy Nuelle, nuelle (name, id)

Challenge 1b- Hough Transform
The voting scheme that I used for the Hough Accumulator is that I would map the calculated 'r' to an index by dividing r by the calculated nrho( 2* distance of the image/ (number of rho bins)) and then adding the (rho_num_bins/2) and rounding the final number that we found. 
I incremented the H accumulator at each calculated r_index at t by 1 so that in the end there would be peaks in my image! 
I chose 500 rho num bins and 200 theta num bins because I found that this combination allowed the image to get several clear peaks in the picture, and didn't result in too many votes in each bin. This combination resulted in clear peaks, and not too many or too little. 


Challenge 1c- Find peaks
The method that I used to find peaks is I originally just tried for the hough_threshold to be 0.5 * max(hough_img(:)) which gave me 255/2= 128, and I found that those values worked well for my lineFinder for the first two images, but for the third image I needed the threshold to be a bit higher in order to distinguish the peaks from the other values that were in the H accumulator, resulting in the found value of 135. 
In line finder, I iterated through the H accumulator and if hough_img(y,x) was greater than the threshold, I found the first X and last X and their corresponding Y values manipulating the line equation from earlier. 
Then, I used line to plot the line on the image!

Challenge 1d- Line Segments
My algorithm to determine the line segments was very similar to Find peaks at the beginning, the only difference being that at the beginning of the file I found the edges using Canny method again on the orig_img and then if hough_img(y,x) was > threshold value, I would go through each X value with step value 2 to find the Y that matched the X given rho and theta, and then rounded that value. If the edge_img that I calculated at (round_y, x_axis) was > 0 meaning that edge had detected an edge there, then it would save that x_axis value and corresponding round_y value into an array. 
Then after going through each x, it would plot the line, and start over with the next theta/rho that was > thresh! 