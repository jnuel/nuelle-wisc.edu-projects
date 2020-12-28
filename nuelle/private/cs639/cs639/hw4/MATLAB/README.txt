loadFocalStack- 
For load focal stack, I read in all the files and their corresponding names. 
Then, I read in the first image to get the dimensions for what the images would be. 
After that, I created my rgb and gray stack. 
Then, I went through each image and stored the 'color' values are each img into the rgb and gray stack arrays.

generateIndexMap- 
First, I used the provided SML = abs(Lx) + abs(Ly); code to get those applied laplacian filter values.
Then, in the forloop where SML is calculated, I applied an average filter to those SML values.
Lastly, outside of the for loop, I returned the max SML values that were stored. 

refocusApp-
First, I found the image dimensions of the stack. 
Then I displayed the first image so that we could get user input from it. 
Then, once we had the input and decided that it was ok(within dimensions),
I then read from the indexMap provided image to display the corresponding image based off 
where the user wants it to be clear at given the provided points! 
