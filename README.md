# InternetOfDoors
IoT and Web Security based lock system using a BeagleBone.

# CodeImplementation
gcc -lmraa -o project project2.cpp
./project r01.cs.ucla.edu 12000 

# Running the executable will output:
The program is going to record….
Input Value: N (where N is either 0, 1 or 2) [On release of the button]
User Input: The 5-character password entered
A message that the password was sent to the server (User input successfully sent to server) and the server response:
Yes
No, N attempts left (where N is 2, 1 or 0)

If the password is entered correct, the server will respond with ‘Group12 YES’
If the password is entered incorrectly three times, then 
An alarm will sound
Prevent the user from sending in additional input
An email will be sent to the owner of the lock. 

For details on how to use this lock, you can follow the instructions on http://group12engr96.scienceontheweb.net/
