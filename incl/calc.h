#ifndef _CALC_H
#define _CALC_H

#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>

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

void print_buff(const char * label, unsigned char * buff, int buff_size) {

	printf("%s: ", label);
	for(int i=0; i<buff_size; i++) printf("%02x ", buff[i]);
	printf("\n");
}

/**
 *  This function takes a read-only buffer (hence the 'const' modifier) and
 *  copies its values into the structure at the address provided.
 */
void build_struct(struct Calculation * calc, const unsigned char * buff) {

	int index = 0;

	calc->op_code = buff[index++];
	calc->value1 = buff[index++];
	calc->value2 = buff[index++];
	calc->result = buff[index++];
	calc->result <<= 8;
	calc->result |= buff[index++];
	calc->result <<= 8;
	calc->result |= buff[index++];
	calc->result <<= 8;
	calc->result |= buff[index++];
	calc->count = buff[index++];
	calc->count <<= 8;
	calc->count |= buff[index++];

	// we assume the input buffer had multi-byte fields
	// in network byte order.
	calc->result = ntohl(calc->result);
	calc->count = ntohs(calc->count);
}

/**
 *  This function serializes the input structure into a byte stream
 *  with the network byte order applied where appropriate.  Once
 *  complete, it returns the total number of types in the resulting
 *  buffer.
 */
int build_buff(unsigned char * buff, const struct Calculation * calc) {

	// TODO: provide an implementation here
}

/**
 *  This function prints the fiels of the structure provided as the
 *  second parameter.  The label (first parameter) is printed before
 *  the structure.  All options selected in the Op Code filed are also
 *  printed as appropriate in a space-delimited list.  The printed
 *  values match the names of the macros defining each Op Code field
 *  at the top of this file (ADD_REQ, ADD_RSP etc).
 */
void print_struct(const char * label, const struct Calculation * calc) {


	// TODO: provide an implementation here
}

#endif
