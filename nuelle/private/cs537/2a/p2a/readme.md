Going through my code functions top-down:

I first create a Node struct so that I can implement paths.

Then I made the print error function that I call everytime I want to print an error.

Then I made a global head variable for the path(we want one for the entire duration of the program)

execthis basically calls fork and exec
- it checks to see if redirection has been set to true, and if so it reassigns stdout to a file
- we separate the commands appropriately from the passed in array args
- then we call execv with the commands!

builtin has the same functionality as ExecThis except it also now tests for the built in functions that I was supposed to implement, such as
- exit
- cd
- path including add, remove, and clear
and if none of those commands are the ones we want to call then call execthis

doThis takes in the command separated by the ;
- it tests to see if the passedin arg has an & inside of it
if it does - i should separate them based off the &'s first and then call execthis on all of them!
if it doesnt - i separate based off normal whitespace and proceed with calling builtin
- this takes care of parallel and forking

main sets up the getline to get the stdin and or the lines from the batch mode.
It sets up the head of the path and assigns it to /bin automatically.
Then we enter an infinite while loop until the user types in exit that will ask for the user smash> aka what command they want to write. 
