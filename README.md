# InternetOfDoors
IoT and Web Security based lock system using a BeagleBone.

# CodeImplementation
gcc -lmraa -o project project2.cpp </br>
./project r01.cs.ucla.edu 12000 

# Running the executable will output:
The program is going to record…. </br>
Input Value: N (where N is either 0, 1 or 2) [On release of the button] </br>
User Input: The 5-character password entered </br>
A message that the password was sent to the server (User input successfully sent to server) and the server response: </br>
&nbsp; &nbsp; &nbsp; Yes </br>
&nbsp; &nbsp; &nbsp; No, N attempts left (where N is 2, 1 or 0) </br>

If the password is entered correct, the server will respond with ‘Group12 YES’ </br>
If the password is entered incorrectly three times, then </br>
&nbsp; &nbsp; &nbsp;An alarm will sound </br>
&nbsp; &nbsp; &nbsp;Prevent the user from sending in additional input </br>
&nbsp; &nbsp; &nbsp;An email will be sent to the owner of the lock.  </br>

For details on how to use this lock, you can follow the instructions on http://group12engr96.scienceontheweb.net/ </br>
