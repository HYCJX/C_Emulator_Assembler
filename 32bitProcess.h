#ifndef _32BIT_PROCESS_
#define _32BIT_PROCESS_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//Return the single bit in the given 32-bit instruction at the given index.
//@param instruction: Given 32-bit instruction.
//@param index:       Index of the target bit.
bool getBit(uint32_t instruction, int index);

//Check whether the bitSequence in the given instruction is equal to the given bitSequence(in big-endian form).
//Starting from index start and finishing at index end (both inclusive).
//@param instruction: Given 32-bit index.
//@param start:       Starting index.
//@param end:         Finishing index.
//@param bitSequence: A char sequence of 0s and 1s.
//Pre: start < end.
bool isBitsMatch(u_int32_t instruction, int start, int end, char *bitSequence);

//Return the value of teh 8-bit binary in reversed order.
//@param instructionL Given 8-bit binary.
unsigned char reverseByte(unsigned char instruction);

//Perform arithmetic shift over the given 32-bit binary number by a specific amount of bits.
//Store the carry bit in the given carry pointer.
//@param direction: Character representing rotate direction.
//@param x:         Given 32-bit binary.
//@param amount:    Amount of bits being shifted.
//@param *cPtr:     Pointer of boolean, store value for carry out.
//Pre: direction = ANY('L', 'R'). Otherwise shift operation will not be executed.
uint32_t arithShift32Bit(char direction, uint32_t x, unsigned int amount, bool *cPtr);

//Calculate the decimal value of the bit sequence in the given instruction,starting from index start and finishing at index end (both inclusive).
//@param instruction: Given 32-bit instruction.
//@param start:       Starting index.
//@param end:         Finishing index.
//Pre: start <= end. Otherwise return 0.
u_int32_t getBitSeq(uint32_t instruction, int start, int end);

//Perform logical shift over the given 32-bit binary number by a specific amount of bits.
//Store the carry bit in the given carry pointer.
//@param direction :Character representing rotate direction.
//@param x:         Given 32-bit binary.
//@param amount:    Amount of bits being shifted.
//@param *cPtr:     Pointer of boolean, store value for carry out.
//Pre: direction = ANY('L', 'R'). Otherwise shift operation will not be executed.
u_int32_t logicalShift32Bit(char direction, u_int32_t x, unsigned int amount, bool *cPtr);

//Return the value of the 32-bit binary in revered order.
//@param instruction: Given 32-bit binary.
u_int32_t reverse32Bit(u_int32_t instruction);

//Rotate the given 32-bit binary number by a specific amount of bits.
//Store the carry bit in the given carry pointer.
//@param direction: Character representing rotate direction.
//@param x:         Given 32-bit binary.
//@param amount:    Amount of bits being shifted.
//@param @cPtr:     Pointer of boolean, store value for carry out.
//Pre: direction = ANY('L', 'R'). Otherwise shift operation willnot be excuted.
u_int32_t rotate32Bit(char direction, u_int32_t x, unsigned int amount, bool *cPtr);

//Return the result of sign extending a number of certain length to 32 bits.
//@param number: Given number.
//@param length: Original length of the number.
uint32_t signExtend(u_int32_t number, unsigned int length);

//Print the given 32-bit in binary form and start and new line in the terminal.
//@param instruction: Given 32-bit binary.
void printlnBinary(u_int32_t instruction);

//Set the target bit at the given index of the designated 32-bit binary code as the given bit.
//@param dest:     Pointer to the designated 32-bit code.
//@param index:    Index fo teh target bit.
//@param givenBit: Bit replaces the target bit.
void setBit(uint32_t *dest, unsigned int index, bool givenBit);

#endif