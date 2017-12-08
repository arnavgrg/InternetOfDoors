#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <mraa/aio.h>
#include <stdbool.h>
#include <unistd.h> //for sleep
#include <string.h> //for comparing the strings


//defining what PWD_LEN is
#define PWD_LEN 5

sig_atomic_t volatile is_pressed=0;

//custom error handler function to print helpful custom error messages
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void buttonHandle()
{
	if(is_pressed==0)
		is_pressed=1;
	else
		is_pressed=0;
}

//takes light sensor values and calculates & returns average
int readValues()
{
	bool button = true;
	//calculates the value from light sensor
	double value = 0;
	double aveval = 0;
	double count = 0;

	mraa_aio_context sensor;
	sensor = mraa_aio_init(1);

	while(button)
	{
		count = 0;
		while(is_pressed)
		{
			button = false;
			value += mraa_aio_read(sensor);
			usleep(20000);
			count++;
		}
	}
	aveval = value/count; //calculates average value
	return aveval;
}

//function that gets password & uses light sensor and button in this function
void getPass(char *password)
{
	int count = 0; //counter for for loop
	int value = 0;
	//the threshold values (lower and upper bounds)
	const float THRESHOLD_LOWER = 75.0;
	const float THRESHOLD_UPPER = 300.0;

	//initializes the context of mraa
	mraa_gpio_context button;button = mraa_gpio_init(60);

	//configure button to be an input pin.
	mraa_gpio_dir(button, MRAA_GPIO_IN);

	//when button is pressed,calls buttonHandle() w/ 1 = pressed and 0= not pressed
	mraa_gpio_isr(button, MRAA_GPIO_EDGE_BOTH, &buttonHandle, NULL);

	while(count<PWD_LEN)
	{
		if(is_pressed)
		{       value = readValues(); //value=average value here
			//determines where calculated value is relative to thresholds and updates password depending on if the average value is above or below the threshold
			if(value <= THRESHOLD_LOWER)
			{
				//below_threshold = true;
				password[count] = '2';
				printf("Input Value: 2\n");
			}
			else if (value > THRESHOLD_LOWER && value < THRESHOLD_UPPER)
			{
				//between_threshold = true;
				password[count] = '0';
				printf("Input Value: 0\n");
			}
			else if (value >= THRESHOLD_UPPER)
			{
				//above_threshold = true;
				password[count] = '1';
				printf("Input Value: 1\n");
			}
			count++;
		}
		password[count] = '\0';
	}
	mraa_gpio_close(button);
}


//function that alarms the buzzer if freeze is called
void buzz()
{
	//initializes the context of mraa
	mraa_gpio_context buzzer;
	buzzer = mraa_gpio_init(62);

	//mraa_aio_context led;
	mraa_gpio_dir(buzzer, MRAA_GPIO_OUT);

	int time = 0;
	while (time < 15)
	{
		mraa_gpio_write(buzzer, 1); //turn on for one second
		time += 1;
		sleep(1);
		mraa_gpio_write(buzzer, 0); //turn off for one second
		time += 1;
		sleep(1);
	}
	mraa_gpio_close(buzzer);
}

//main
int main(int argc, char *argv[])
{
	int client_socket_fd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	char password[256];
	int attempt = 0;

	// Read command line arguments, need to get the host IP address and port
	if (argc < 3)
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	// Convert the arguments to the appropriate data types
	portno = atoi(argv[2]);

	// setup the socket
	client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	// check if the socket was created successfully. If it wasn’t, display an error and exit
	if(client_socket_fd < 0)
	{
		error("ERROR opening socket");
	}

	// check if the IP entered by the user is valid
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	// clear our the serv_addr buffer
	memset((char *) &serv_addr, 0, sizeof(serv_addr));


	// set up the socket
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	// try to connect to the server
	if (connect(client_socket_fd,(struct sockaddr *)& serv_addr,sizeof(serv_addr)) < 0){
		error("ERROR connecting");
	}
	memset(buffer, 0 ,256);

	strcpy(buffer, "ID = Group12 Password = ");
	strcat(buffer, "\n");
	printf("the program is going to record... \n");
	sleep(1);

	is_pressed=0;
	getPass(password);
	printf("%s %s\n", "user input: ",  password);

	//concatenates two strings buffer and password
	strcat(buffer, password);
	strcat(buffer, "\n");

	n = write(client_socket_fd, buffer, strlen(buffer));
	printf("user input successfully sent to server: ");

	// n contains how many bytes were received by the server
	// if n is less than 0, then there was an error
	if (n < 0)
	{
		error("ERROR writing to socket");
	}
	//clear out the buffer
	memset(buffer, 0, 256);

	//get the response from the server and print it to the user
	n = read(client_socket_fd, buffer, 255);
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	printf("%s\n",buffer);

	//compares the replied message from the server (YES/NO) to determine if the program should freeze or not. If the inputted password is incorrect (NO==NO);


	if (strcmp(buffer," No, 0 Attempts Left\n")==0)
	{
		system("mutt -s \"Alert from DoorLock! Suspicious entry prevented!\" enginneering96sample@gmail.com < messages.txt");
		buzz();
		printf("Locked Out\n");
	}

	// clean up the file descriptors
	close(client_socket_fd);
	return 0;
}

