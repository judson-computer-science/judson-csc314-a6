# Assignment 5

## Overview
In this assignment you will create a client and server program.  The client will invoke the services of the server to perform an arithmatic calculation on two values.  The request structure will contain an *Op Code* field specifying the operation to be performed along with two additional flags that tell the server if it should print the calculation to STDOUT and if it should return the original input values are part of its response.  After computing the result, the server will generate a new message with the result, the total number of requests serviced and an *Op Code* field specifying the type of Operation that was performed for the client.  The server should remain up to service additional requests while the client is invoked from the command line each time a new calculation is to be performed.

## Input
Your client program will be invoked by providing three command line parameters as follows:

	./client <options> <value1> <value2>

		options: This is a single string containing any combination
		     	 of the following values used to derive the *Op Code*:

			e = the server should echo the calulation
			    (ECHO_CALC field is set in the *Op Code*)
			r = the server should return the original values
			    (RETURN_VALS field is set in the *Op Code*)
			m = perform multiplication (MUL_REQ field is set
			    in the *Op Code*)
			s = perform substraction (SUB_REQ field is set
                       	    in the *Op Code*)
			a = perform addition (ADD_REQ field is set in
                            the *Op Code*)

			NOTE: m, s and a are mutually exclusive operations
			      so the user should only select one

		value1: This is the value that is used as the first operand
			in the calculation. This value should be in the range
			of what can fit into a uint8_t (0 to 255). You are
			NOT expected to validate this input

		value2: This is the value that is used as the second operand
			in the calculation. This value should be in the range
			of what can fit into a uint8_t (0 to 255). You are
			NOT expected to validate this input

## Implementation
Both client and server MUST use the header files provided in the `incl` directory, which provide the connectivity parameters (in `incl/parameters.h`) and the following additional definitions (in `incl/calc.h`):

	// Op Code bits
	#define ADD_REQ     0x01 // indicates a request to add input values
	#define SUB_REQ     0x02 // indicates a request to subtract input values
	#define MUL_REQ     0x04 // indicates a request to multiply input values
	#define ADD_RSP     0x08 // indicates a response to a ADD_REQ
	#define SUB_RSP     0x10 // indicates a response to a SUB_REQ
	#define MUL_RSP     0x20 // indicates a response to a MUL_REQ
	#define RETURN_VALS 0x40 // input values should be returned by server
	#define ECHO_CALC   0x80 // calculation should be echo'd by server

	struct Calculation {
		uint8_t op_code;
		uint8_t value1;
		uint8_t value2;
		int32_t result;
		uint16_t count;
	};

In addition to these, the `incl/calc.h` header also provides an implementation for the following functions (look inside calc.h for details on what they do):

	void print_buff(const char *, unsigned char *, int);
	void build_struct(struct Calculation *, const unsigned char *);

Also in this file, you will find two unimplemented functions that you will need to provide an implementation for: `build_buff()` and `print_struct()`. You MUST add functionality to the body of those functions as part of your work for this assignment.

You will create a client (`calc_client.c`) and a server (`calc_server.c`) that will communicate with each other via a TCP socket (at the IP and Port specified in `parameters.h`). The server will `accept()` a connection, then `read()` from the connection before performing the calculation, contructing the response and using `send()` to respond to the client.  After sending the response, the server should return to the state of waiting for an incoming connection.  The server should continue to run until you kill it by hitting CTRL+C. All communication between client and server should happen by sending serialized data (slack bytes removed and network byte order used where appropriate).

## Accepting Command Line Parameters
To accept command line parameters, use the following signature for your `main()` function of your client:

	int main(int argc, char const *argv[])
	
		argc:	the number of parameters passed at the command line
			(including the name of the executable file).

		argv:	an array of strings where each member of the array
			is the string representation of the parameters
			passed to the function (for example argv[0] would
			contain the name of the executable, argv[1] would
			contain the value passed as the first command line
			parameter, etc).
## Compiling Code
A `Makefile` has been provided to help you build both the client and server execution binaries.  Inspect the Makefile to ensure you name the client and server source code files correctly.  You can then compile them in the following manner:

~~~~
make client
make server
~~~~

## Error Handling
Your client should validate the command line input and exit gracefully if there are not exactly three command line parameters provided.  There is no need to validate any other conditions as we will assume the 2nd and 3rd parameters are numeric (and within range) and that the second parameter contains at least one flag to indicate the arithmetic operation as well as 0 or more optional flags.

Use the value of argc to validate the number of command line parameters.

## Output
Your program should produce the following output, where lines beginning with "C:" are generated by the client, and lines starting with "S:" are generated by the server.

### SCENARIO 1: Incorrect Number of Arguments, first request serviced by server

~~~~
	> ./client
C:	wrong number of arguments!
~~~~


### SCENARIO 2: Subtract 255 from 25, echo calculation, return values, 1st request
~~~~
	> ./client sre 25 255
C:	
C:	Request:
C:	========
C:	  Op Code:     c2 (hex) SUB_REQ ECHO_CALC RETURN_VALS
C:	  Value 1:     25 (dec)
C:	  Value 2:    255 (dec)
C:	
C:	Sending bytes: c2 19 ff 00 00 00 00 00 00 
C:	

S:	[  1] Received bytes: c2 19 ff 00 00 00 00 00 00 
S:	[  1] CALCULATION: 25 - 255 = -230
S:	[  1] Sending bytes: d0 19 ff 1a ff ff ff 01 00 

C:	Received bytes: d0 19 ff 1a ff ff ff 01 00 
C:	
C:	Response:
C:	=========
C:	  Op Code:     d0 (hex) SUB_RSP ECHO_CALC RETURN_VALS
C:	  Value 1:     25 (dec)
C:	  Value 2:    255 (dec)
C:	  Result:    -230 (dec)
C:	  Requests:     1 (dec)
~~~~


### SCENARIO 3: Add 255 to 100, return values, 2nd request

~~~~
	> ./client ar 100 255
C:	
C:	Request:
C:	========
C:	  Op Code:     41 (hex) ADD_REQ RETURN_VALS
C:	  Value 1:    100 (dec)
C:	  Value 2:    255 (dec)
C:	
C:	Sending bytes: 41 64 ff 00 00 00 00 00 00 
C:	

S:	[  2] Received bytes: 41 64 ff 00 00 00 00 00 00 
S:	[  2] Sending bytes: 48 64 ff 63 01 00 00 02 00 

C:	Received bytes: 48 64 ff 63 01 00 00 02 00 
C:	
C:	Response:
C:	=========
C:	  Op Code:     48 (hex) ADD_RSP RETURN_VALS
C:	  Value 1:    100 (dec)
C:	  Value 2:    255 (dec)
C:	  Result:     355 (dec)
C:	  Requests:     2 (dec)
~~~~

### SCENARIO 4: Multiply 255 by 255, echo calculation, 27th request

~~~~
	> ./client me 255 255
C:
C:	Request:
C:	========
C:	  Op Code:     84 (hex) MUL_REQ ECHO_CALC
C:	  Value 1:    255 (dec)
C:	  Value 2:    255 (dec)
C:	
C:	Sending bytes: 84 ff ff 00 00 00 00 00 00 
C:	

S:	[ 27] Received bytes: 84 ff ff 00 00 00 00 00 00 
S:	[ 27] CALCULATION: 255 * 255 = 65025
S:	[ 27] Sending bytes: a0 00 00 01 fe 00 00 1b 00 

C:	Received bytes: a0 00 00 01 fe 00 00 1b 00 
C:	
C:	Response:
C:	=========
C:	  Op Code:     a0 (hex) MUL_RSP ECHO_CALC
C:	  Value 1:     OMITTED
C:	  Value 2:     OMITTED
C:	  Result:   65025 (dec)
C:	  Requests:    27 (dec)
~~~~
