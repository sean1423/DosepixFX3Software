//#include "StdAfx.h"
#include "CommonFunctions.h"

using namespace std;

CommonFunctions::CommonFunctions(void) {

	const int MAXCOUNTER = 11811;
	unsigned int rev[11811];
	unsigned int number = BinaryToIntConverter("0011111111111111");
	unsigned int revNumber = number;
	unsigned int s = sizeof(number) * CHAR_BIT - 1; // extra shift needed at end
	unsigned int xor = 0;

	//direct order
	unsigned int tap1 = 0;
	unsigned int tap2 = 13;
	

	for (int iClock = 0; iClock < MAXCOUNTER; iClock++) {
		pseudoRandomCounter14BitTap0_13[revNumber] = iClock;
		rev[iClock] = revNumber;
		xor = ((number >> tap2) ^ (number >> tap1)) & 0x1;
		number = ( (number << 1) | xor ) & 0x3FFF;
		revNumber = number;

		// http://graphics.stanford.edu/~seander/bithacks.html
		// swap odd and even bits
		revNumber = ((revNumber >> 1) & 0x55555555) | ((revNumber & 0x55555555) << 1);
		// swap consecutive pairs
		revNumber = ((revNumber >> 2) & 0x33333333) | ((revNumber & 0x33333333) << 2);
		// swap nibbles ... 
		revNumber = ((revNumber >> 4) & 0x0F0F0F0F) | ((revNumber & 0x0F0F0F0F) << 4);
		// swap bytes
		revNumber = ((revNumber >> 8) & 0x00FF00FF) | ((revNumber & 0x00FF00FF) << 8);
		// swap 2-byte long pairs
		revNumber = ( revNumber >> 16             ) | ( revNumber               << 16);
		// -----------------------------------------------------
		revNumber = revNumber >> 18; 
	}

	// Writing to the file
	//FILE * pFileForm;
	//pFileForm = fopen ("c:\\pFileForm.txt","w");
	//for (int iClock = 0; iClock < MAXCOUNTER; iClock++) {
	//	//cout << iClock << " " << rev[iClock] << " " << bitset<14>(rev[iClock]) << endl;
	//	fprintf (pFileForm, "%d %d\n", iClock, rev[iClock]);
	//	//getchar();
	//}
	//fclose (pFileForm);

}

CommonFunctions::~CommonFunctions(void) {

}

int CommonFunctions::GetNumberOfDigits(int i) {
	return i > 0 ? (int) log10((double) i) + 1 : 1;
}

bool CommonFunctions::GetBitValue(unsigned char RxBuffer, int bit) { // Returns bit value
	if (bit > 9) {
		std::cout << "[CommonFunctions::GetBitValue] WARNING: bit value must be 8 or less. Setting bit to 8..." << std::endl; bit = 8;
	}
	return (bool)(RxBuffer & (1 << bit));
}

bool CommonFunctions::GetBitValue(int RxBuffer, int bit) {
	// INFO: indexing starts with 0! so if you want to check bit 1 (result 0) of number say 1010, use int bit = 1;
	if (bit > 13) {
		std::cout << "[CommonFunctions::GetBitValue] WARNING: bit value must be 13 or less. Setting bit to 13 (of 0..1..2..13)..." << std::endl; bit = 13;
	}
	return (bool)(RxBuffer & (1 << bit)); //
}

void CommonFunctions::SetBitValue(unsigned char& RxBuffer, int bit, bool value) { // Sets variable "bit" = "value"
	if (bit > 7) {
		std::cout << "[CommonFunctions::SetBitValue] WARNING: bit value must be 7 or less. Setting bit to 7..." << std::endl; bit = 7; // If the bit passed to this function is greater than 7 then this line sets the bit value to 7
	}
	if (value == 0) { // we want to set 0. If "value" == 0 then sets bit value to 0. Else do nothing. If bit value does not equal 1 or 0 then error message.
		if (this->GetBitValue(RxBuffer, bit) == 1) {
			RxBuffer &= ~(1 << bit); // Clears the bit?
		}
		else if (this->GetBitValue(RxBuffer, bit) == 0) {
			// do nothing...
		}
		else {
			std::cout << "[CommonFunctions::SetBitValue] ERROR 1: This should've not happend!" << std::endl; // Does this if bit value is other than 1 or 0.
		}
	}
	else if (value == 1) { // If "value" == 1 and bit == 1 do nothing. If bit == 0 sets the bit to 1
		if (this->GetBitValue(RxBuffer, bit) == 1) {
			// do nothing...
		}
		else if (this->GetBitValue(RxBuffer, bit) == 0) {
			RxBuffer |= 1 << bit; // Sets the bit?
		}
		else {
			std::cout << "[CommonFunctions::SetBitValue] ERROR 2: This should've not happend!" << std::endl; //Does this if the value is other than 1 or 0.
		}
	}
	else {
		// std::cout << "[CommonFunctions::SetBitValue] DEBUG 3: Value: " << value << std::endl;
		std::cout << "[CommonFunctions::SetBitValue] ERROR 3: This should've not happend!" << std::endl;
	}
}

void CommonFunctions::SetBitValue(int& RxBuffer, int bit, bool value) {
	if (bit > sizeof(int)*8) {
		// std::cout << "[CommonFunctions::SetBitValue] WARNING: bit value must be 7 or less. Setting bit to 7..." << std::endl; bit = 7;
	}
	if (value == 0) { // we want to set 0
		if (this->GetBitValue(RxBuffer, bit) == 1) {
			RxBuffer &= ~(1 << bit);
		}
		else if (this->GetBitValue(RxBuffer, bit) == 0) {
			// do nothing...
		}
		else {
			std::cout << "[CommonFunctions::SetBitValue] ERROR: This should've not happend!" << std::endl;
		}
	}
	else if (value == 1) { // we want to set 1
		if (this->GetBitValue(RxBuffer, bit) == 1) {
			// do nothing...
		}
		else if (this->GetBitValue(RxBuffer, bit) == 0) {
			RxBuffer |= 1 << bit;
		}
		else {
			std::cout << "[CommonFunctions::SetBitValue] ERROR: This should've not happend!" << std::endl;
		}
	}
	else {
		std::cout << "[CommonFunctions::SetBitValue] ERROR: This should've not happend!" << std::endl;
	}
}

int CommonFunctions::BinaryToIntConverter(string binaryString) {
    int result = 0 ;
    for(size_t count = 0; count < binaryString.length(); ++count) {
        result *=2;
        result += binaryString[count] == '1' ? 1 : 0;
    }  
	return result; 
}

int CommonFunctions::PseudoRandomDecoder(int value) {
	//cout << value << " ";
	int bob = 0;
	//cout << bob << endl;
	if (value != 0 ) {
		try {
			bob = pseudoRandomCounter14BitTap0_13.find(value)->second;
		}
		catch (...) {
			return 0;
		}
	}
	return bob;
}


unsigned short CommonFunctions::ReverseBitOrder(unsigned short v) {
  v = ((v >> 1) & 0x5555) | ((v & 0x5555) << 1); /* swap odd/even bits */
  v = ((v >> 2) & 0x3333) | ((v & 0x3333) << 2); /* swap bit pairs */
  v = ((v >> 4) & 0x0F0F) | ((v & 0x0F0F) << 4); /* swap nibbles */
  v = ((v >> 8) & 0x00FF) | ((v & 0x00FF) << 8); /* swap bytes */
  return v;
}
