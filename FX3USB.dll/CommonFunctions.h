#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

// #pragma once

#include <map>
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "easylogging++.h"

class CommonFunctions {
public:
	int GetNumberOfDigits(int);
	bool GetBitValue(unsigned char RxBuffer, int bit);
	bool GetBitValue(int RxBuffer, int bit);
	void SetBitValue(unsigned char& RxBuffer, int bit, bool value);
	void SetBitValue(int& RxBuffer, int bit, bool value);
	int PseudoRandomDecoder(int value = 16383);
	int BinaryToIntConverter(std::string binaryString);
	unsigned short ReverseBitOrder(unsigned short value);

	CommonFunctions(void);
	~CommonFunctions(void);

private:
	std::map<unsigned int, unsigned int>pseudoRandomCounter14BitTap0_13;
};


/* a=target variable, b=bit number to act upon 0-n */
//#define BIT_SET(a,b) ((a) |= (1<<(b)))
//#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
//#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
//#define BIT_CHECK(a,b) ((a) & (1<<(b)))
//
///* x=target variable, y=mask */
//#define BITMASK_SET(x,y) ((x) |= (y))
//#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
//#define BITMASK_FLIP(x,y) ((x) ^= (y))
//#define BITMASK_CHECK(x,y) ((x) & (y))

#endif /* COMMONFUNCTIONS_H */
