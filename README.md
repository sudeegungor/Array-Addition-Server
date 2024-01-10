# Array-Addition-Server
In this assignment you are asked to create a simple array addition server, which will take
two series of numbers from a port connection using telnet program (telnet console
command that has been used in lab sessions), perform an addition operation on them and
output the result to original telnet connection.
The integers you accept and work on should be at most 3 digits long (from 0 to 999). If
the input contains number(s) larger than 999 and smaller than 0, you should just return an
error. The number of integers should also be equal for both input integer arrays. If they
are not equal, this should also return an error.
Your program should be written in C programming language and it should work on a
Linux computer. You should create a simple virtual Linux operating system inside
VirtualBox software (the same setup we use in our lab sessions) for development and
testing of your assignment.
A sample port/socket communication between your assignment and telnet program is
given below with extra information and comments.


telnet localhost 60000


With this command, you will be connected to port 60000 of your localhost (your own
computer). Assuming this is the port that your assignment is currently working on, you
should receive the following message.


Hello, this is Array Addition Server!
Please enter the first array for addition:
105 449 445 842 292 655 959 6 404 149


To simplify input, we will just use single spaces for integer separation. Commas or
parenthesis's are also not necessary. As you can see, there are 10 integer inputs for the
first array.


Please enter the second array for addition:
999 601 78 502 156 482 805 670 834 27


Just like before, we input 10 different integers for this operation, different number of
integers should raise an error.


The result of array addition are given below:
1 105 50 524 344 449 138 764 677 238 176


As you can see, if the sum of two numbers are greater than 999, only the first 3 digits are
stored and written in its original location and a carry is added to the integer at left. This
results on an additional integer generated with carry at the leftmost location of array.
Thank you for Array Addition Server! Good Bye!
With this message, the connection is closed by the server and communication ends.
