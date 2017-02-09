/*
 * Dosepix.cpp
 *
 *  Created on: 17 Apr 2015
 *      Author: Dima Maneuski
 *      Author:
 */

#include "C:\Documents\eclipse\c++\DosepixFX.dll/DosepixFX.h"

using namespace std;

Dosepix::Dosepix() {
	LOG(DEBUG) << "";
	hw_isInitialised = false;
	myUSBDevice = NULL;
	auxs = NULL;
	//sprintf (chipID, "");
	//memset(rawPeripheryDACRegister, 0, sizeof(rawPeripheryDACRegister));
	memset(this->peripheryDACRegister, 1, sizeof(this->peripheryDACRegister));
	//memset(usbTxBuffer, 0, sizeof(usbTxBuffer));
	//memset(usbRxBuffer, 0, sizeof(usbRxBuffer));
	//memset(dacs, 1, sizeof(dacs));

	dacs.VCascReset = 255;
	dacs.IPixelDac = 255;
	dacs.IKrum = 255;
	dacs.VFBK = 255;
	dacs.IPreamp = 255;
	dacs.Vgnd = 255;
	dacs.VCascPreamp = 255;
	dacs.IDisc1 = 255;
	dacs.IDisc2 = 255;
	dacs.ITPBuffIn = 255;
	dacs.ITPBuffOut = 255;
	dacs.VTPrefCoarse = 255;
	dacs.VTPrefFine = 511;
	dacs.VThA = 8191;

	omr.OperationMode=2;
	omr.GlobalShutter=1;
	omr.PLL=3;
	omr.Polarity=1;
	omr.AnalogOutSel=5;
	omr.AnalogInSel=5;
	omr.DisableColClkGate=1;
	omr.Unassigned=6;
}

Dosepix::~Dosepix() {
	LOG(DEBUG) << "";
}

int Dosepix::HW_Initalise() {
	LOG(DEBUG) << "";
	auxs = new CommonFunctions();
	myUSBDevice = new FX3USB();

	myUSBDevice->HW_Initalise();

	hw_isInitialised = myUSBDevice->IsInitialised();
	if (hw_isInitialised == false) {
		LOG(ERROR) << "Failed to initialise hardware!";
	}
	return 0;
}

int Dosepix::HW_Finalise() {
	LOG(DEBUG) << "";
	myUSBDevice->HW_Finalise();
	hw_isInitialised = myUSBDevice->IsInitialised();
	return 0;
}


//int Dosepix::HW_Chip_Write_DAC_DATA(){
//
//	LOG(DEBUG) << "Beginning DAC Data Writing";
//	myUSBDevice->DiscardBuffer();
//	//unsigned short DAC_Data = BOOST_BINARY( 1111111111111111 ); // uses BOOST_BINARY to assign a 16 bit binary number to DAC_Data.
//	unsigned char DAC_Data = 0xFFFF;
//	LOG(DEBUG) << DAC_Data;
//
//	myUSBDevice->SetBuffer(DAC_Data, 16);
//	myUSBDevice->HW_TransmitData();
//	Sleep(100);
//
//
//}


int Dosepix::HW_Chip_WriteOMR() {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "We are in DosepixFX.cpp";
	myUSBDevice->DiscardBuffer();
    unsigned char dataPacket[DPX_OMR_LENGTH / 8 + 1];
    unsigned char header[1];
    header[0] = 0x23; // OMR header, b'11_0001_xx, see page25
    dataPacket[0] = header[0];
    memcpy(&dataPacket[1], this->rawOMR, DPX_OMR_LENGTH / 8);
	myUSBDevice->SetBuffer(dataPacket, DPX_OMR_LENGTH / 8 + 1);
	myUSBDevice->HW_TransmitData();
	Sleep(100);
	return 0;
}

/**
 *	TODO: Please check that when new row is started the data is copied correctly into the dataPacket.
 *	This needs to be checked as it not clear how C++ copied data in 2D arrays.
 */
int Dosepix::HW_Chip_SetConfigurationBits() {
	LOG(DEBUG) << "";
	myUSBDevice->DiscardBuffer();
    unsigned char dataPacket[DPX_CONFIGBITS_LENGTH / 8 + 1];
    unsigned char header[1];
    header[0] = 0x13; // Configuration Bit header, b'11_0010_xx, see page25
    dataPacket[0] = header[0];

    //LOG(DEBUG) << "ColSelRegister: " << std::bitset<8>(this->pixelConfigMtrx[0][0]);

    memcpy(&dataPacket[1], &this->pixelConfigMtrx[0][0], DPX_CONFIGBITS_LENGTH / 8);
	myUSBDevice->SetBuffer(dataPacket, DPX_CONFIGBITS_LENGTH / 8 + 1);
	myUSBDevice->HW_TransmitData();
	Sleep(100);
	return 0;
}

int Dosepix::HW_Chip_SetSingleDigitalThreshold(){
	LOG(DEBUG) << "";
		myUSBDevice->DiscardBuffer();
		unsigned char dataPacket[512 + 1];
		unsigned char header[1];
		header[0] = 0x33; // 0x33 Single Digital Threshold header, b'11_0011_xx, see page25
		dataPacket[0] = header[0];
		// we need to rotate bits (MSB with LSB to comply with data sending convention we set in firmware)

		//unsigned char b = x;
		//b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
//		cout << "------------------" << endl;
//		cout << bitset<16>(singleDigitalThD08[0][0]) << endl;
//		cout << "------------------" << endl;
//
//		unsigned short b = singleDigitalThD08[0][0];
//		b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
//		cout << "------------------" << endl;
//		cout << bitset<16>(b) << endl;
//		cout << "------------------" << endl;
//		memcpy(&dataPacket[1], &this->singleDigitalThD00 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD01 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD02 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD03 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD04 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD05 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD06 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD07 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD08 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD09 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD10 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD11 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD12 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD13 [0][0], 512);
//		memcpy(&dataPacket[1], &this->singleDigitalThD14 [0][0], 512);
		memcpy(&dataPacket[1], &this->singleDigitalThD15 [0][0], 512);
		myUSBDevice->SetBuffer(dataPacket, 3); //512 + 1
		myUSBDevice->HW_TransmitData();
//		Sleep(100);
		return 0;
}

int Dosepix::HW_Chip_SetPixelDACBits() {
	LOG(DEBUG) << "";
	myUSBDevice->DiscardBuffer();
    unsigned char dataPacket[DPX_DACBITS_LENGTH / 8 + 1];
    unsigned char header[1];
    header[0] = 0xB; // Configuration Bit header, b'11_0100_xx, see page25
//    header[0] = 0x80; // test
    dataPacket[0] = header[0];

    LOG(DEBUG) << "pixelDACMtrx: " << std::bitset<8>(this->pixelDACMtrx[0][0]);

    memcpy(&dataPacket[1], &this->pixelDACMtrx[0][0], DPX_DACBITS_LENGTH / 8);
	myUSBDevice->SetBuffer(dataPacket, DPX_DACBITS_LENGTH / 8 + 1);
	myUSBDevice->HW_TransmitData();
	Sleep(100);
	return 0;
}

int Dosepix::HW_Chip_WritePeripheryDACRegister() {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "We are in DosepixFX.cpp";
	myUSBDevice->DiscardBuffer();
	unsigned char dataPacket[DPX_FSR_LENGTH / 8 + 1];
	unsigned char header[1];
	header[0] = 0x2B; // Periphery DAC header, b'11_0101_xx, see page25
	dataPacket[0] = header[0];
	memcpy(&dataPacket[1], this->rawPeripheryDACRegister, DPX_FSR_LENGTH / 8);
	myUSBDevice->SetBuffer(dataPacket, DPX_FSR_LENGTH / 8 + 1);
	myUSBDevice->HW_TransmitData();
	Sleep(100);
	return 0;
}

int Dosepix::HW_Chip_SetColSelRegister() {
	LOG(DEBUG) << "";
	myUSBDevice->DiscardBuffer();
    unsigned char dataPacket[DPX_COLSEL_LENGTH / 8 + 1];
    unsigned char header[1];
    header[0] = 0x3B; // Configuration Bit header, b'11_0111_xx, see page25
    dataPacket[0] = header[0];
    memcpy(&dataPacket[1], &this->ColSelRegister, DPX_COLSEL_LENGTH / 8);
	myUSBDevice->SetBuffer(dataPacket, DPX_COLSEL_LENGTH / 8 + 1);
	myUSBDevice->HW_TransmitData();
	Sleep(100);
	return 0;
}

void Dosepix::SetOMR_OperationMode(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	bool bit0 = auxs->GetBitValue(value, 0);
	bool bit1 = auxs->GetBitValue(value, 1);

	LOG(DEBUG) << "bit0 " << bit0;
	LOG(DEBUG) << "bit1 " << bit1;
	this->OMR[23] = bit0;
	this->OMR[22] = bit1;
	this->UpdateRawOMRRegister();

}

void Dosepix::SetOMR_GlobalShutter(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	this->OMR[21] = value;
	this->UpdateRawOMRRegister();
	bool bit0 = auxs->GetBitValue(value, 0);
	LOG(DEBUG) << "bit0 " << bit0;
	this->UpdateRawOMRRegister();
}

void Dosepix::SetOMR_PLL(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	bool bit0 = auxs->GetBitValue(value, 0);
	bool bit1 = auxs->GetBitValue(value, 1);
	bool bit2 = auxs->GetBitValue(value, 2);

	LOG(DEBUG) << "bit0 " << bit0;
	LOG(DEBUG) << "bit1 " << bit1;
	LOG(DEBUG) << "bit2 " << bit2;

	this->OMR[20] = bit0;
	this->OMR[19] = bit1;
	this->OMR[18] = bit2;
	this->UpdateRawOMRRegister();
}

void Dosepix::SetOMR_Polarity(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	this->OMR[17] = value;
	bool bit0 = auxs->GetBitValue(value, 0);
	LOG(DEBUG) << "bit0 " << bit0;
	this->UpdateRawOMRRegister();
}

void Dosepix::SetOMR_AnalogOutSel(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	bool bit0 = auxs->GetBitValue(value, 0);
	bool bit1 = auxs->GetBitValue(value, 1);
	bool bit2 = auxs->GetBitValue(value, 2);
	bool bit3 = auxs->GetBitValue(value, 3);
	bool bit4 = auxs->GetBitValue(value, 4);

	LOG(DEBUG) << "bit0 " << bit0;
	LOG(DEBUG) << "bit1 " << bit1;
	LOG(DEBUG) << "bit2 " << bit2;
	LOG(DEBUG) << "bit3 " << bit3;
	LOG(DEBUG) << "bit4 " << bit4;

	this->OMR[16] = bit0;
	this->OMR[15] = bit1;
	this->OMR[14] = bit2;
	this->OMR[13] = bit3;
	this->OMR[12] = bit4;
	this->UpdateRawOMRRegister();
}

void Dosepix::SetOMR_AnalogInSel(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	bool bit0 = auxs->GetBitValue(value, 0);  //Passes the function GetBitValue (the value passed to SetOMR_AnalogInSel) as well as a value between 0 and 4.
	bool bit1 = auxs->GetBitValue(value, 1);  //Sets the variable bit1 to the value of the 12 bit(?) value at index 1.
	bool bit2 = auxs->GetBitValue(value, 2);
	bool bit3 = auxs->GetBitValue(value, 3);
	bool bit4 = auxs->GetBitValue(value, 4);

	LOG(DEBUG) << "bit0 " << bit0;  // Prints the value bit0 to console
	LOG(DEBUG) << "bit1 " << bit1;
	LOG(DEBUG) << "bit2 " << bit2;
	LOG(DEBUG) << "bit3 " << bit3;
	LOG(DEBUG) << "bit4 " << bit4;


	this->OMR[11] = bit0;
	this->OMR[10] = bit1;
	this->OMR[9]  = bit2;
	this->OMR[8]  = bit3;
	this->OMR[7]  = bit4;
	this->UpdateRawOMRRegister();
}

void Dosepix::SetOMR_DisableColClkGate(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	this->OMR[6] = value;
	this->UpdateRawOMRRegister();
}

void Dosepix::SetOMR_Unassigned(int value) {
	LOG(DEBUG) << "";
	LOG(DEBUG) << "Value: " << value;
	bool bit0 = auxs->GetBitValue(value, 0);
	bool bit1 = auxs->GetBitValue(value, 1);
	bool bit2 = auxs->GetBitValue(value, 2);
	bool bit3 = auxs->GetBitValue(value, 3);
	bool bit4 = auxs->GetBitValue(value, 4);
	bool bit5 = auxs->GetBitValue(value, 5);

	LOG(DEBUG) << "bit0 " << bit0;
	LOG(DEBUG) << "bit1 " << bit1;
	LOG(DEBUG) << "bit2 " << bit2;
	LOG(DEBUG) << "bit3 " << bit3;
	LOG(DEBUG) << "bit4 " << bit4;
	LOG(DEBUG) << "bit5 " << bit5;

	this->OMR[5] = 	bit0;
	this->OMR[0] = 	bit5;
	this->UpdateRawOMRRegister();
//	for (int i = 0; i <= 23; i++) { this->OMR[i] = auxs->GetBitValue(value, i); }
}

void Dosepix::SetPixelConfigBit(bool mtrx[16][16], int bit) {
	for (int i = 0; i < 16; i++) {//loops the  rows
		for (int j = 0; j < 16; j++) { //loops the columns
			auxs->SetBitValue(pixelConfigMtrx[i][j], bit, mtrx[i][j]);
		}
	}
	//this->LinearisePixelConfigMtrx();
}

void Dosepix::SetDigitalThreshold(int thesholdNumber, unsigned short mtrx[16][16]){
//	unsigned short th[16][16];

	switch (thesholdNumber) {
		case 0: {
			memcpy(&this->singleDigitalThD00[0][0], &mtrx[0][0], 16*16*2);
			break;
		}
		case 1: {
			memcpy(&this->singleDigitalThD01[0][0], &mtrx[0][0], 16*16*2);
			break;
				}
		case 2: {
			memcpy(&this->singleDigitalThD02[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 3: {
			memcpy(&this->singleDigitalThD03[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 4: {
			memcpy(&this->singleDigitalThD04[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 5: {
			memcpy(&this->singleDigitalThD05[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 6: {
			memcpy(&this->singleDigitalThD06[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 7: {
			memcpy(&this->singleDigitalThD07[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 8: {
			memcpy(&this->singleDigitalThD08[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 9: {
			memcpy(&this->singleDigitalThD09[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 10: {
			memcpy(&this->singleDigitalThD10[0][0], &mtrx[0][0], 16*16*2);
			break;
		}
		case 11: {
			memcpy(&this->singleDigitalThD11[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 12: {
			memcpy(&this->singleDigitalThD12[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 13: {
			memcpy(&this->singleDigitalThD13[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 14: {
			memcpy(&this->singleDigitalThD14[0][0], &mtrx[0][0], 16*16*2);
			break;
						}
		case 15: {
			memcpy(&this->singleDigitalThD15[0][0], &mtrx[0][0], 16*16*2);
			break;
						}

	}

	for (int i = 0; i < 16; i++) {//loops the columns
		for (int j = 0; j < 16; j++) { //loops the rows
			this->singleDigitalThD00[i][j] += (0  << 12);
			this->singleDigitalThD01[i][j] += (1  << 12);
			this->singleDigitalThD02[i][j] += (3  << 12);
			this->singleDigitalThD03[i][j] += (2  << 12);
			this->singleDigitalThD04[i][j] += (6  << 12);
			this->singleDigitalThD05[i][j] += (4  << 12);
			this->singleDigitalThD06[i][j] += (5  << 12);
			this->singleDigitalThD07[i][j] += (7  << 12);
			this->singleDigitalThD08[i][j] += (15 << 12);
			this->singleDigitalThD09[i][j] += (13 << 12);
			this->singleDigitalThD10[i][j] += (12 << 12);
			this->singleDigitalThD11[i][j] += (14 << 12);
			this->singleDigitalThD12[i][j] += (10 << 12);
			this->singleDigitalThD13[i][j] += (11 << 12);
			this->singleDigitalThD14[i][j] += (9  << 12);
			this->singleDigitalThD15[i][j] += (8  << 12);
//			printf("%d ", singleDigitalThD08[i][j]);
			//LOG(DEBUG) <<"test"<<singleDigitalThD08;
		}
//		printf("\n");
	}
	for (int i = 0; i < 16; i++) {//loops the columns
		for (int j = 0; j < 16; j++) { //loops the rows
			this->singleDigitalThD00[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD00[i][j]);
			this->singleDigitalThD01[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD01[i][j]);
			this->singleDigitalThD02[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD02[i][j]);
			this->singleDigitalThD03[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD03[i][j]);
			this->singleDigitalThD04[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD04[i][j]);
			this->singleDigitalThD05[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD05[i][j]);
			this->singleDigitalThD06[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD06[i][j]);
			this->singleDigitalThD07[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD07[i][j]);
			this->singleDigitalThD08[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD08[i][j]);
			this->singleDigitalThD09[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD09[i][j]);
			this->singleDigitalThD10[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD10[i][j]);
			this->singleDigitalThD11[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD11[i][j]);
			this->singleDigitalThD12[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD12[i][j]);
			this->singleDigitalThD13[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD13[i][j]);
			this->singleDigitalThD14[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD14[i][j]);
			this->singleDigitalThD15[i][j] = auxs->ReverseBitOrder(this->singleDigitalThD15[i][j]);


		}
	}


}

void Dosepix::SetPixelDAC(int mtrx[16][16]) {
	bool bit0 = 0, bit1 = 0, bit2 = 0, bit3 = 0, bit4 = 0, bit5 = 0;
	for (int i = 0; i < 16; i++) {//loops the  rows
		for (int j = 0; j < 16; j++) { //loops the columns
			bit0 = auxs->GetBitValue(mtrx[i][j], 5);
			bit1 = auxs->GetBitValue(mtrx[i][j], 4);
			bit2 = auxs->GetBitValue(mtrx[i][j], 3);
			bit3 = auxs->GetBitValue(mtrx[i][j], 2);
			bit4 = auxs->GetBitValue(mtrx[i][j], 1);
			bit5 = auxs->GetBitValue(mtrx[i][j], 0);

			auxs->SetBitValue(this->pixelDACMtrx[i][j], 0, bit0);
			auxs->SetBitValue(this->pixelDACMtrx[i][j], 1, bit1);
			auxs->SetBitValue(this->pixelDACMtrx[i][j], 2, bit2);
			auxs->SetBitValue(this->pixelDACMtrx[i][j], 3, bit3);
			auxs->SetBitValue(this->pixelDACMtrx[i][j], 4, bit4);
			auxs->SetBitValue(this->pixelDACMtrx[i][j], 5, bit5);
		}
	}
	//this->LinearisePixelConfigMtrx();
}



/**
 * TODO: Implement checking the column number does not exceed range [0:15].
 */
void Dosepix::SetColumnSelect(int col) {
	LOG(DEBUG) << "";
	bool value = 0;
	for (int iBit = 7; iBit >= 0; iBit--) {//loops the  rows
//	for (int iBit = 0; iBit < 8; iBit++) {//loops the  rows
		value = auxs->GetBitValue(col, 7-iBit);
		auxs->SetBitValue(this->ColSelRegister, iBit, value);
    }
	LOG(DEBUG) << "ColSelRegister: " << bitset<8>(this->ColSelRegister);
	// LOG(DEBUG) << "Value: " << value;
	//this->LinearisePixelConfigMtrx();
}

void Dosepix::SetDAC( int dacIndex, int value) {
	LOG(DEBUG) << "";
	int MSB = 0;
	int LSB = 0;
	verbose = 0;
	switch (dacIndex) {
		case VCascReset:		{dacs.VCascReset = value;		MSB = VCasResetMSB; 	LSB = VCasResetLSB;			if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting VCascReset: "   << value << endl; break;}
		case IPixelDac:			{dacs.IPixelDac = value;  		MSB = IPixelDacMSB; 	LSB = IPixelDacLSB;			if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting IPixelDac" 	  << value << endl; break;}
		case IKrum:				{dacs.IKrum = value;	 		MSB = IKrumMSB; 		LSB = IKrumLSB;				if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting IKrum: " 		  << value << endl; break;}
		case VFBK:				{dacs.VFBK = value;				MSB = VFBKMSB; 			LSB = VFBKLSB;				if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting VFBK: " 		  << value << endl; break;}
		case IPreamp:			{dacs.IPreamp = value;			MSB = IPreampMSB; 		LSB = IPreampLSB;			if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting IPreamp: " 	  << value << endl; break;}
		case Vgnd:				{dacs.Vgnd = value; 			MSB = VgndMSB; 			LSB = VgndLSB;				if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting Vgnd: " 		  << value << endl; break;}
		case VCascPreamp:		{dacs.VCascPreamp = value; 		MSB = VCascPreampMSB;	LSB = VCascPreampLSB;		if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting VCascPreamp: "  << value << endl; break;}
		case IDisc1:			{dacs.IDisc1 = value; 			MSB = IDisc1MSB; 		LSB = IDisc1LSB;			if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting IDisc1: " 	  << value << endl; break;}
		case IDisc2:			{dacs.IDisc2 = value; 			MSB = IDisc2MSB; 		LSB = IDisc2LSB;			if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting IDisc2: " 	  << value << endl; break;}
		case ITPBuffIn:			{dacs.ITPBuffIn = value; 		MSB = ITPBuffInMSB; 	LSB = ITPBuffInLSB;			if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting ITPBuffIn: " 	  << value << endl; break;}
		case ITPBuffOut:		{dacs.ITPBuffOut = value; 		MSB = ITPBuffOutMSB; 	LSB = ITPBuffOutLSB;		if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting ITPBuffOut: "   << value << endl; break;}
		case VTPrefCoarse:		{dacs.VTPrefCoarse = value; 	MSB = VTPrefCoarseMSB; 	LSB = VTPrefCoarseLSB;		if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting VTPrefCoarse: " << value << endl; break;}
		case VTPrefFine:		{dacs.VTPrefFine = value; 		MSB = VTPrefFineMSB; 	LSB = VTPrefFineLSB;		if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting VTPrefFine: "   << value << endl; break;}
	    case VThA:				{dacs.VThA = value; 			MSB = VThAMSB; 			LSB = VThALSB;				if (verbose >= 2) cout << "INFO: [Dosepix::SetDAC]: DAC index " << dacIndex << ". Setting VThA: " 		  << value << endl; break;}

	    default: {
			if (verbose >= 0) cout << "ERROR: [Dosepix::SetDAC]: provided DAC index does not exist." << endl; break;
			}
	}

	for (int i = LSB; i <= MSB; i++) { this->peripheryDACRegister[i] = auxs->GetBitValue(value, i - LSB); }//
	this->UpdateRawPeripheryDACRegister();
}
// This function is supposed to put the OMR data (pre-set) into a register.
//
void Dosepix::UpdateRawOMRRegister() {
	if (verbose >= 2) cout << "\nINFO: [Dosepix::UpdateRaWOMR] Concatinating rawOMR...\n";
	LOG(DEBUG) << "       THIS IS THE START OF UpdateRawOMRRegister";
	//bool FSRDPX[DPX_FSR_LENGTH];
	//for (int i = 0; i < DPX_FSR_LENGTH; i++) FSRDPX[i] = 1;
	//for (int i = 1; i < DPX_FSR_LENGTH; i++) FSRDPX[i] = this->peripheryDACRegister[i-1];

	//for (int i = 0; i < DPX_FSR_LENGTH; i++) FSRDPX[i] = this->peripheryDACRegister[i];

	for (int i = 0; i < DPX_OMR_LENGTH / 8; i++) { //original //looping through bytes

		LOG(DEBUG) << "Value of i: " << i;

//		for (int iBit = 0; iBit < 8; iBit++) { //looping through bits
		for (int iBit = 7; iBit < 8; iBit++) { //looping through bits

			LOG(DEBUG) << "Value of iBit: " << iBit;

			//for (int iBit = 7; iBit>=0; iBit--) { //edited
			////auxs->SetBitValue(rawPeripheryDACRegister[DPX_FSR_LENGTH / 8 - 1 - i], iBit, FSRDPX[iBit + i*8]);

			auxs->SetBitValue(this->rawOMR[i], iBit, this->OMR[iBit + i*8]); //rawOMR is the position in the register      setting the value of iBit to the value stored in OMR[iBit + i*8]

			LOG(DEBUG) << "Current Position in OMR: " << this->rawOMR[i] << " Current value of OMR" << this->OMR[iBit + i*8];

			//getchar();
			//auxs->SetBitValue(this->rawOMR[i], iBit, this->OMR[iBit - i*8]); //editied
			//LOG(DEBUG) << "PISH DOESN WRTK HERE<<<<<";
		}
	}
}

void Dosepix::UpdateRawPeripheryDACRegister() {
if (verbose >= 2) cout << "\nINFO: [Dosepix::UpdateRawPeripheryDACRegister] Concatinating rawPeripheryDACRegister...\n";

for (int i = 0; i < DPX_FSR_LENGTH / 8; i++) {
	for (int iBit = 0; iBit < 8; iBit++) {
//		for (int iBit = 7; iBit < 8; iBit--) {
		//auxs->SetBitValue(rawPeripheryDACRegister[DPX_FSR_LENGTH / 8 - 1 - i], iBit, FSRDPX[iBit + i*8]);
		auxs->SetBitValue(rawPeripheryDACRegister[i], iBit, this->peripheryDACRegister[iBit + i*8]);
		}
	}
}


////////////////////////////////////////////////////////////////
////////////////////// PRINTING FUNCTIONS //////////////////////
////////////////////////////////////////////////////////////////

void Dosepix::PrintRawFSR() {
	LOG(DEBUG) << "";
	int w0 = 14; int w1 = 4; int w2 = 10;
	cout << "-------------------------------" << endl;
	cout << "Current content of rawPeripheryDACRegister" << endl;
	for (int i = 0; i < DPX_FSR_LENGTH / 8; i++) {
		cout << (int)rawPeripheryDACRegister[i] << " ";
	}
	cout << "\n";
	for (int i = 0; i < DPX_FSR_LENGTH / 8; i++) {
//		for (int iBit = 7; iBit >=0 ; iBit--) { 	// MSB first
			for (int iBit = 0; iBit <=0 ; iBit++) { 	// MSB first test this


			cout << auxs->GetBitValue(rawPeripheryDACRegister[i], iBit);//  rawPeripheryDACRegister[iBit + i*8];
		}
		cout << " ";
	}
	cout << "\n";

	//std::cout << std::hex << x << std::endl;
	for (int i = 0; i < DPX_FSR_LENGTH / 8; i++) {
		cout << std::hex << (int)rawPeripheryDACRegister[i] << " ";
	}
	cout << "\n";
}

void Dosepix::PrintFSR() {
	LOG(DEBUG) << "";
	for (int i = 0; i < DPX_FSR_LENGTH; i++) {
		//unsigned temp= 0XD400;
		//temp+= this->peripheryDACRegister[i];
		cout << this->peripheryDACRegister[i];
		//if ((i%8) == 0 && (i != 0)) cout << " ";

	}
	cout << "\n";
}

void Dosepix::PrintDACs() {
	LOG(DEBUG) << "";
	int w0 = 14; int w1 = 4; int w2 = 10;
	cout << "-----------------------------------" << endl;
	cout << "Current Dosepix DACs" << endl;
	cout << setw(w0) << "VCascReset = "			<< setw(w1) << dacs.VCascReset		<< " | " << setw(w2) << bitset<8>(dacs.VCascReset)		<< endl;
	cout << setw(w0) << "IPixelDac =  "			<< setw(w1) << dacs.IPixelDac		<< " | " << setw(w2) << bitset<8>(dacs.IPixelDac)		<< endl;
	cout << setw(w0) << "IKrum =      "			<< setw(w1) << dacs.IKrum		    << " | " << setw(w2) << bitset<8>(dacs.IKrum)			<< endl;
	cout << setw(w0) << "VFBK =       "	        << setw(w1) << dacs.VFBK            << " | " << setw(w2) << bitset<8>(dacs.VFBK) 			<< endl;
	cout << setw(w0) << "IPreamp =    "	        << setw(w1) << dacs.IPreamp      	<< " | " << setw(w2) << bitset<8>(dacs.IPreamp)			<< endl;
	cout << setw(w0) << "Vgnd =       "			<< setw(w1) << dacs.Vgnd			<< " | " << setw(w2) << bitset<8>(dacs.Vgnd)			<< endl;
	cout << setw(w0) << "VCascPreamp= "	        << setw(w1) << dacs.VCascPreamp		<< " | " << setw(w2) << bitset<8>(dacs.VCascPreamp)		<< endl;
	cout << setw(w0) << "IDisc1 =     "			<< setw(w1) << dacs.IDisc1 			<< " | " <<	setw(w2) << bitset<8>(dacs.IDisc1)			<< endl;
	cout << setw(w0) << "IDisc2 =     "		    << setw(w1) << dacs.IDisc2			<< " | " << setw(w2) << bitset<8>(dacs.IDisc2)			<< endl;
	cout << setw(w0) << "ITPBuffIn =  "			<< setw(w1) << dacs.ITPBuffIn		<< " | " << setw(w2) << bitset<8>(dacs.ITPBuffIn)		<< endl;
	cout << setw(w0) << "ITPBuffOut = "			<< setw(w1) << dacs.ITPBuffOut		<< " | " << setw(w2) << bitset<8>(dacs.ITPBuffOut)		<< endl;
	cout << setw(w0) << "VTPrefCoarse="			<< setw(w1) << dacs.VTPrefCoarse	<< " | " << setw(w2) << bitset<8>(dacs.VTPrefCoarse)	<< endl;
	cout << setw(w0) << "VTPrefFine = "			<< setw(w1) << dacs.VTPrefFine		<< " | " << setw(w2) << bitset<9>(dacs.VTPrefFine)		<< endl;
	cout << setw(w0) << "VThA = 	  "			<< setw(w1) << dacs.VThA			<< " | " << setw(w2) << bitset<13>(dacs.VThA)			<< endl;

	cout << "----------------------------------" << endl;
}
void Dosepix::PrintomrModes() {
	LOG(DEBUG) << "";
	int w0 = 7; int w1 = 4; int w2 = 10;
	cout << "-----------------------------------" << endl;
	cout << "Current Dosepix OMR signal names" << endl;
	cout << setw(w0) << "OperationMode 		="				<< setw(w1) << omr.OperationMode			<< " | " << setw(w2) << bitset<2>(omr.OperationMode)	<< endl;
	cout << setw(w0) << "GlobalShutter 		="				<< setw(w1) << omr.GlobalShutter			<< " | " << setw(w2) << bitset<1>(omr.GlobalShutter)	<< endl;
	cout << setw(w0) << "PLL          		="				<< setw(w1) << omr.PLL						<< " | " << setw(w2) << bitset<3>(omr.PLL)				<< endl;
	cout << setw(w0) << "Polarity 			="				<< setw(w1) << omr.Polarity		   			<< " | " << setw(w2) << bitset<1>(omr.Polarity)			<< endl;
	cout << setw(w0) << "AnalogOutSel 		="	        	<< setw(w1) << omr.AnalogOutSel            	<< " | " << setw(w2) << bitset<5>(omr.AnalogOutSel) 	<< endl;
	cout << setw(w0) << "AnalogInSel 		="	        	<< setw(w1) << omr.AnalogInSel      		<< " | " << setw(w2) << bitset<5>(omr.AnalogInSel)		<< endl;
	cout << setw(w0) << "DisableColClkGate 	="				<< setw(w1) << omr.DisableColClkGate		<< " | " << setw(w2) << bitset<1>(omr.DisableColClkGate)<< endl;
	cout << setw(w0) << "Unassigned			= "	        	<< setw(w1) << omr.Unassigned				<< " | " << setw(w2) << bitset<6>(omr.Unassigned)		<< endl;

	cout << "----------------------------------" << endl;
}

void Dosepix::PrintUSBTxBuffer(const char* mode, int nBytes) {
	myUSBDevice->PrintBuffer(mode, nBytes);
}
void Dosepix::PrintPixelConfig() {
	for (int i = 0; i < 16; i++) {//loops the  rows
		for (int j = 0; j < 16; j++) {
				std::cout << bitset<8>(this->pixelConfigMtrx[i][j]) << " ";
			}
			cout << std::endl;
			LOG(DEBUG)  << "The PrintPixelConfig Function has just ended.";
		}
}

void Dosepix::PrintDigitalThD() {
	for (int i = 0; i < 16; i++) {//loops the  rows
		for (int j = 0; j < 16; j++) {
//			std::cout << bitset<16>(this->singleDigitalThD00[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD01[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD02[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD03[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD04[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD05[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD06[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD07[i][j]) << " ";
			std::cout << bitset<16>(this->singleDigitalThD08[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD09[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD10[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD11[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD12[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD13[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD14[i][j]) << " ";
//			std::cout << bitset<16>(this->singleDigitalThD15[i][j]) << " ";
//
			}
			cout << std::endl;
		}
}

void Dosepix::PrintPixelDAC() {
	for (int i = 0; i < 16; i++) {//loops the  rows
		for (int j = 0; j < 16; j++) {
				std::cout << bitset<6>(this->pixelDACMtrx[i][j]) << " ";
			}
			cout << std::endl;
		}

}

void Dosepix::PrintRawOMR() {
	LOG(DEBUG) << "";
	int w0 = 14; int w1 = 4; int w2 = 10;
	cout << "-------------------------------" << endl;
	cout << "Current content of RawOMR" << endl;
//	for (int i = 0; i < DPX_OMR_LENGTH / 8; i++) {
//		cout << hex << (int)rawOMR[i] << " ";//-1 test this
//	}
//	cout << "\n";
	for (int i = 0; i < DPX_OMR_LENGTH / 8; i++) {
//		for (int iBit = 0; iBit <=0 ; iBit++) { 	// MSB first  -edited
		for (int iBit = 7; iBit >=0 ; iBit--) { 	// MSB first original
			cout << auxs->GetBitValue(rawOMR[i], iBit);//  rawPeripheryDACRegister[iBit + i*8];
		}//for (int iBit = 7; iBit >= 0; iBit--)
		cout << " ";
	}
	cout << "\n";

	//std::cout << std::hex << x << std::endl;
	for (int i = 0; i < DPX_OMR_LENGTH / 8; i++) {
//		for (int i = 24; i < DPX_OMR_LENGTH / 8; i--) {
		cout << std::hex << (int)rawOMR[i] << " ";

	}
	cout << "\n";
}

void Dosepix::PrintOMR() {
	LOG(DEBUG) << "rawOMR is going to be reversed";
//	for (int i = 24; i <= DPX_OMR_LENGTH; i--) {//edited
		for (int i=0; i<=DPX_OMR_LENGTH; i++){
		//unsigned temp= 0XD400;
		//temp+= this->peripheryDACRegister[i];
		//cout << temp;
		cout << this->OMR[i];


		//if ((i%8) == 0 && (i != 0)) cout << " ";

	}

	cout << "\n" << "rawOMR has been reversed \n";

}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// NO MORE UNCOMMENTED CODE AFTER THIS LINE ////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

//
//void Dosepix::PrintFSR(int startPosition, int endPosition) {
//	LOG(DEBUG) << "";
//	std::cout << "\tPosition\tValue" << std::endl;
//	for (int i = startPosition; i < endPosition; i++)
//		std::cout << "\t" << i << "\t" << this->peripheryDACRegister[i] << std::endl;
//}

//void Dosepix::PrintPixelConfiguration (int col, int row, const char* mode) {
//	LOG(DEBUG) << "";
//	if (strcmp (mode, "int") == 0) {
//		printf_s("\t %d\n", this->pixelConfigMtrx[col][row]);
//	}
//
//	else if (strcmp (mode, "bin") == 0) {
//		printf_s("\tPixel bits [14:0]\n", col, row);
//		printf_s("\t------------------------------\n");
//	    // printf_s("\t00000000000000");
//		printf_s("\t"); printf("[%03d][%03d]\t", col, row);
//		std::cout << bitset<TPX_BITS>(this->pixelConfigMtrx[255-col][255-row]) << std::endl;
//	}
//}
//
//int Dosepix::GetDAC(_DACs_Timepix dacNameEnum) {
//	LOG(DEBUG) << "";
//	return dacs[dacNameEnum];
//}

/**
 * @TODO This function does not update the usbTxBuffer yet!!! this->peripheryDACRegister[] only!!!
 */
//int Dosepix::SetMonitorDAC(_DACs_Timepix dacNameEnum) {
//	LOG(DEBUG) << "";
//	//dacIndex = 1;
//	int DACCode = 0;
//	int verbose = 0;
//	switch (dacNameEnum) {
//		case IKrum:				{ DACCode = 15; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring IKrum" << endl; break;}
//		case Disc:				{ DACCode = 11; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Disc" << endl; break; }
//		case Preamp:			{ DACCode = 7;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Preamp" << endl; break;}
//		case BuffAnalogA:		{ DACCode = 3;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BuffAnalogA" << endl; break;}
//		case BuffAnalogB:		{ DACCode = 4;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BuffAnalogB" << endl; break;}
//		case Hist:				{ DACCode = 9;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Hist" << endl; break;}
//		case THL:				{ DACCode = 6;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring THL" << endl; break;}
//		case THLCoarse:			{ DACCode = 6;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring THL" << endl; break;}
//		case Vcas:				{ DACCode = 12; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Vcas" << endl; break;}
//		case FBK:				{ DACCode = 10; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring FBK" << endl; break;}
//		case GND:				{ DACCode = 13; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring GND" << endl; break;}
//		case THS:				{ DACCode = 1;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring THS" << endl; break;}
//		case BiasLVDS:			{ DACCode = 2;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BiasLVDS" << endl; break;}
//		case RefLVDS:			{ DACCode = 14; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring RefLVDS" << endl; break;}
//		case BiasDAC:			{ DACCode = 0;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BiasDAC" << endl; break;}
//		case BiasOutStage:		{ DACCode = 5;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BiasOutStage" << endl; break;}
//		case vbgOnChip:			{ DACCode = 8;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring vbgOnChip" << endl; break;}
//		default: {
//			if (verbose >= 1) cout << "WARNING: [Timepix::MonitorDAC]: provided DAC index does not exist.. Setting DAC index to IKrum" << endl;
//			DACCode = 15; break;
//				}
//	}
//
//	int SenseDAC = 42;
//	int ExtDACSel = 43;
//	int DACCodeBits[] = {37, 38, 40, 41};
//
//	// Sense DAC
//	this->peripheryDACRegister[SenseDAC] = 1;
//	// Ext DAC select
//	this->peripheryDACRegister[ExtDACSel] = 0;
//	// DACCode = 0;
//
//	this->peripheryDACRegister[DACCodeBits[3]] = auxs->GetBitValue(DACCode, 3);
//	this->peripheryDACRegister[DACCodeBits[2]] = auxs->GetBitValue(DACCode, 2);
//	this->peripheryDACRegister[DACCodeBits[1]] = auxs->GetBitValue(DACCode, 1);
//	this->peripheryDACRegister[DACCodeBits[0]] = auxs->GetBitValue(DACCode, 0);
//
//	// this->UpdateRawPeripheryDACRegister();
//	return 0;
//}
//

//Please note Vcascreset not right dac code. extra such as bandgap voltage ref block(failsafe-14), [Vcas,krum(15-dac output],temp sensor o/p(11), bias vltage periphery DAC(12)
//cascode bias voltage periphery dac-13 and high z- default OMR val after global reset-15[both b1]


//void Dosepix::MonitorDAC(int dacIndex) {
//
//		//dacIndex = 1;
//		int DACCode = 0;
////		int getDacIndex(DACCode);//edited
//		switch (dacIndex) {
//			case Dosepix::VCascReset:		{DACCode = 15; if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring VCascReset" 	<< endl; break;}//not sure if correct
//			case Dosepix::IPixelDac:		{DACCode = 13; if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring IPixelDac" 		<< endl; break;}
//			case Dosepix::IKrum:			{DACCode = 12; if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring IKrum" 			<< endl; break;}
//			case Dosepix::VFBK:				{DACCode = 4;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring VFBK" 			<< endl; break;}
//			case Dosepix::IPreamp:			{DACCode = 7;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring IPreamp" 		<< endl; break;}
//			case Dosepix::Vgnd:				{DACCode = 6;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring Vgnd" 			<< endl; break;}
//			case Dosepix::VCascPreamp:		{DACCode = 3;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring VCascPreamp" 	<< endl; break;}
//			case Dosepix::IDisc1:			{DACCode = 8;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring IDisc1" 		<< endl; break;}
//			case Dosepix::IDisc2:			{DACCode = 12; if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring IDisc2" 		<< endl; break;}
//			case Dosepix::ITPBuffIn:		{DACCode = 10; if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring ITPBuffIn" 		<< endl; break;}
//			case Dosepix::ITPBuffOut:		{DACCode = 11; if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring ITPBuffOut" 	<< endl; break;}
//			case Dosepix::VTPrefCoarse:		{DACCode = 5;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring VTPrefCoarse" 	<< endl; break;}
//			case Dosepix::VTPrefFine:		{DACCode = 2;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring VTPrefFine" 	<< endl; break;}
//			case Dosepix::VThA:				{DACCode = 1;  if (verbose >= 2) cout << "INFO: [Dosepix::MonitorDAC]: DAC index " << dacIndex << ". Monitoring VThA" 			<< endl; break;}
//			default: {
//				if (verbose >= 1) cout << "WARNING: [Dosepix::MonitorDAC]: provided DAC index does not exist.. Setting DAC index to IKrum" << endl;
//				dacIndex = 12; break;
//
//					}
//		}
		//			int SenseDAC = 42;
//			int ExtDACSel = 43;
//			int DACCodeBits[] = {37, 38, 40, 41};
//
//			// Sense DAC
//			peripheryDACRegister[SenseDAC] = 1;
//			// Ext DAC select
//			peripheryDACRegister[ExtDACSel] = 0;
//
//			// DACCode = 0;
//
//			peripheryDACRegister[DACCodeBits[3]] = auxs->GetBitValue(DACCode, 3);
//			peripheryDACRegister[DACCodeBits[2]] = auxs->GetBitValue(DACCode, 2);
//			peripheryDACRegister[DACCodeBits[1]] = auxs->GetBitValue(DACCode, 1);
//			peripheryDACRegister[DACCodeBits[0]] = auxs->GetBitValue(DACCode, 0);
//
//			this->UpdateRawPeripheryDACRegister();
//		}

//commented
//
//int Dosepix::SetMode(int timepix_mode=0) {
//	bool bitP0 = 0;
//	bool bitP1 = 0;
//	if (timepix_mode == 0)			{ bitP0 = 0; bitP1 = 0; } // Dosepix
//	else if  (timepix_mode == 1)	{ bitP0 = 1; bitP1 = 0; } // TOT
//	else if  (timepix_mode == 2)	{ bitP0 = 0; bitP1 = 1; } // Timepix-1hit
//	else if  (timepix_mode == 3)	{ bitP0 = 1; bitP1 = 1; } // Timepix
//	else							{ bitP0 = 0; bitP1 = 0; } // Dosepix
//
//	for (int x = 0; x < 256; x++) {
//		for (int y = 0; y < 256; y++) {
//				auxs->SetBitValue(pixelConfigMtrx[255-x][255-y], BIT_P0, bitP0);
//				auxs->SetBitValue(pixelConfigMtrx[255-x][255-y], BIT_P1, bitP1);
//		}
//	}
//	this->LinearisePixelConfigMtrx();
//	return 0;
//}

//int Dosepix::SetMode(const int mtrx[256][256]) {
//	bool bitP0 = 0;
//	bool bitP1 = 0;
//
//	for (int x = 0; x < 256; x++) {
//		for (int y = 0; y < 256; y++) {
//			if		 (mtrx[x][y] == 0)  { bitP0 = 0; bitP1 = 0; } // Dosepix
//			else if  (mtrx[x][y] == 1)	{ bitP0 = 1; bitP1 = 0; } // TOT
//			else if  (mtrx[x][y] == 2)	{ bitP0 = 0; bitP1 = 1; } // Timepix-1hit
//			else if  (mtrx[x][y] == 3)	{ bitP0 = 1; bitP1 = 1; } // Timepix
//			else						{ bitP0 = 0; bitP1 = 0; } // if nothing fits set Dosepix mode
//
//			auxs->SetBitValue(pixelConfigMtrx[255-x][255-y], BIT_P0, bitP0);
//			auxs->SetBitValue(pixelConfigMtrx[255-x][255-y], BIT_P1, bitP1);
//		}
//	}
//	this->LinearisePixelConfigMtrx();
//	return 0;
//}

//int Dosepix::SetThreshold(const int mtrx[256][256]) {
//	bool bitTH0 = 0;
//	bool bitTH1 = 0;
//	bool bitTH2 = 0;
//	bool bitTH3 = 0;
//

//	for (int x = 0; x < 256; x++) {
//		for (int y = 0; y < 256; y++) {
//			bitTH0 = auxs->GetBitValue(mtrx[x][y], 0);
//			bitTH1 = auxs->GetBitValue(mtrx[x][y], 1);
//			bitTH2 = auxs->GetBitValue(mtrx[x][y], 2);
//			bitTH3 = auxs->GetBitValue(mtrx[x][y], 3);
//
//			auxs->SetBitValue(pixelConfigMtrx[255-y][255-x], BIT_TH0, bitTH0);
//			auxs->SetBitValue(pixelConfigMtrx[255-y][255-x], BIT_TH1, bitTH1);
//			auxs->SetBitValue(pixelConfigMtrx[255-y][255-x], BIT_TH2, bitTH2);
//			auxs->SetBitValue(pixelConfigMtrx[255-y][255-x], BIT_TH3, bitTH3);
//
//			//if (auxs->GetBitValue(mtrx[x][y], 0) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH0);
//			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH0;
//			//if (auxs->GetBitValue(mtrx[x][y], 1) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH1);
//			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH1;
//			//if (auxs->GetBitValue(mtrx[x][y], 2) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH2);
//			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH2;
//			//if (auxs->GetBitValue(mtrx[x][y], 3) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH3);
//			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH3;
//		}
//	}
//	this->LinearisePixelConfigMtrx();
//	return 0;
////	else { if (verbose >= 0) cout << "ERROR: [Timepix::LoadPixelConfigBitTH]: Cannot load threshold bit data" << endl; return -1; }
//}
//void Dosepix::SetThreshold(int mtrx[16][16]) {
//	void Dosepix::SetThreshold(int mtrx[16][16],int SingleDigitalThDCode[16][16], int bit) {
////	bool bit0=0,bit1=0,bit2=0, bit3=0,
//	bool bit4 = 0, bit5 = 0, bit6 = 0, bit7 = 0, bit8 = 0, bit9 = 0, bit10=0, bit11=0, bit12=0, bit13=0, bit14=0, bit15=0;
////	auxs->SetBitValue(pixelConfigMtrx[i][j], bit, mtrx[i][j]);
////	const unsigned char bitTH0[16][16] = 0000;
////	const unsigned char bitTH1[16][16] = 0001;
////	const unsigned char bitTH2[16][16] = 0011;
////	const unsigned char bitTH3[16][16] = 0010;
////	const unsigned char bitTH4[16][16] = 0110;
////	const unsigned char bitTH5[16][16] = 0100;
////	const unsigned char bitTH6[16][16] = 0101;
////	const unsigned char bitTH7[16][16] = 0111;
////	const unsigned char bitTH8[16][16] = 1111;
////	const unsigned char bitTH9[16][16] = 1101;
////	const unsigned char bitTH10[16][16]= 1100;
////	const unsigned char bitTH11[16][16] = 1110;
////	const unsigned char bitTH12[16][16] = 1010;
////	const unsigned char bitTH13[16][16] = 1011;
////	const unsigned char bitTH14[16][16] = 1001;
////	const unsigned char bitTH15[16][16] = 1000;
////	std::cout << bitset<4>(this->bitTH0[i][j]) << " ";
//		for (int i = 0; i < 16; i++) {//loops the  rows
//			for (int j = 0; j < 16; j++) { //loops the columns
//				auxs->SetBitValue(this->ThDRegister[i][j], bit, mtrx[i][j]);}}
//				for (int i = 0; i < 16; i++) {//loops the  rows
//							for (int j = 0; j < 16; j++) { //loops the columns
////				bit0 = auxs->GetBitValue(mtrx[i][j], 0);
////				bit1 = auxs->GetBitValue(mtrx[i][j], 1);
////				bit2 = auxs->GetBitValue(mtrx[i][j], 2);
////				bit3 = auxs->GetBitValue(mtrx[i][j], 3);
//				bit4 = auxs->GetBitValue(mtrx[i][j], 4);
//				bit5 = auxs->GetBitValue(mtrx[i][j], 5);
//				bit6 = auxs->GetBitValue(mtrx[i][j], 6);
//				bit7 = auxs->GetBitValue(mtrx[i][j], 7);
//				bit8 = auxs->GetBitValue(mtrx[i][j], 8);
//				bit9 = auxs->GetBitValue(mtrx[i][j], 9);
//				bit10 = auxs->GetBitValue(mtrx[i][j], 10);
//				bit11 = auxs->GetBitValue(mtrx[i][j], 11);
//				bit12 = auxs->GetBitValue(mtrx[i][j], 12);
//				bit13 = auxs->GetBitValue(mtrx[i][j], 13);
//				bit14 = auxs->GetBitValue(mtrx[i][j], 14);
//				bit15 = auxs->GetBitValue(mtrx[i][j], 15);
//
//
////				auxs->SetBitValue(this->pixelDACMtrx[i][j], 0, bit0);
////				auxs->SetBitValue(this->pixelDACMtrx[i][j], 1, bit1);
////				auxs->SetBitValue(this->pixelDACMtrx[i][j], 2, bit2);
////				auxs->SetBitValue(this->pixelDACMtrx[i][j], 3, bit3);
//				auxs->SetBitValue(this->ThDRegister[i][j], 4, bit4);
//				auxs->SetBitValue(this->ThDRegister[i][j], 5, bit5);
//				auxs->SetBitValue(this->ThDRegister[i][j], 6, bit6);
//				auxs->SetBitValue(this->ThDRegister[i][j], 7, bit7);
//				auxs->SetBitValue(this->ThDRegister[i][j], 8, bit8);
//				auxs->SetBitValue(this->ThDRegister[i][j], 9, bit9);
//				auxs->SetBitValue(this->ThDRegister[i][j], 10, bit10);
//				auxs->SetBitValue(this->ThDRegister[i][j], 11, bit11);
//				auxs->SetBitValue(this->ThDRegister[i][j], 12, bit12);
//				auxs->SetBitValue(this->ThDRegister[i][j], 13, bit13);
//				auxs->SetBitValue(this->ThDRegister[i][j], 14, bit14);
//				auxs->SetBitValue(this->ThDRegister[i][j], 15, bit15);
//
//
////
//
//			}
//		}
//	//	this->LinearisePixelConfigMtrx();
//	}

//			bitTH0 = auxs->GetBitValue(mtrx[i][j], 0);
//			bitTH1 = auxs->GetBitValue(mtrx[i][j], 1);
//			bitTH2 = auxs->GetBitValue(mtrx[i][j], 2);
//			bitTH3 = auxs->GetBitValue(mtrx[i][j], 3);

//
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH0);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH1);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH2);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH3);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH4);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH5);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH6);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH7);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH8);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH9);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH10);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH11);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH12);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH13);
//			auxs->SetBitValue(pixelConfigMtrx[15-i][15-j], BIT_THD, bitTH14);

//			bit0 = auxs->GetBitValue(mtrx[i][j], 0);
//						bit1 = auxs->GetBitValue(mtrx[i][j], 1);
//						bit2 = auxs->GetBitValue(mtrx[i][j], 2);
//						bit3 = auxs->GetBitValue(mtrx[i][j], 3);
//						bit4 = auxs->GetBitValue(mtrx[i][j], 4);
//						bit5 = auxs->GetBitValue(mtrx[i][j], 5);
//
//						auxs->SetBitValue(this->pixelDACMtrx[i][j], 0, bit0);
//						auxs->SetBitValue(this->pixelDACMtrx[i][j], 1, bit1);
//						auxs->SetBitValue(this->pixelDACMtrx[i][j], 2, bit2);
//						auxs->SetBitValue(this->pixelDACMtrx[i][j], 3, bit3);
//						auxs->SetBitValue(this->pixelDACMtrx[i][j], 4, bit4);
//						auxs->SetBitValue(this->pixelDACMtrx[i][j], 5, bit5);
//					}
//				}
//				this->LinearisePixelConfigMtrx();
//			}
			//if (auxs->GetBitValue(mtrx[x][y], 0) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH0);
			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH0;
			//if (auxs->GetBitValue(mtrx[x][y], 1) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH1);
			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH1;
			//if (auxs->GetBitValue(mtrx[x][y], 2) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH2);
			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH2;
			//if (auxs->GetBitValue(mtrx[x][y], 3) == 0)	pixelConfigMtrx[x][y] &= ~(1 << BIT_TH3);
//			//else										pixelConfigMtrx[x][y] |=   1 << BIT_TH3;
//		}
//	}
//	this->LinearisePixelConfigMtrx();
//	return 0;
//	else { if (verbose >= 0) cout << "ERROR: [Timepix::LoadPixelConfigBitTH]: Cannot load threshold bit data" << endl; return -1; }

//void Dosepix::PrintThDRegister() {//bool mtrx[16][16], int bit
//for (int i = 0; i < 16; i++) {//loops the  rows
//		for (int j = 0; j < 16; j++) {
//				//cout << std::hex << (int)this->pixelConfigMtrx[i][j] << " ";
//				std::cout << bitset<16>(this->ThDRegister[i][j]) << " ";
////			auxs->SetBitValue(ThDRegister[i][j], bit, mtrx[i][j]);
//			}
//			cout << std::endl;
//		}
//}


//int Dosepix::SetMask(const int mtrx[256][256]) {
//	for (int x = 0; x < 256; x++) {
//		for (int y = 0; y < 256; y++) {
//			// auxs->SetBitValue(pixelConfigMtrx[255-y][255-x], BIT_MASK, mtrx[x][y]);
//			if (mtrx[x][y] == 0)	pixelConfigMtrx[255-y][255-x] &= ~(1 << BIT_MASK);
//			else					pixelConfigMtrx[255-y][255-x] |= 1 << BIT_MASK;
//		}
//	}
//	this->LinearisePixelConfigMtrx();
//	return 0;
//}

//int Dosepix::LinearisePixelConfigMtrx() {
//	//rawConfigRegister[2*MAX_X*MAX_Y*TPX_BITS];
//	//pixelConfigMtrx[MAX_X][MAX_Y];
//	//auxs->GetBitValue();
//	//cout << "pixelConfigMtrx[x][y]: " << cout << bitset<14>(pixelConfigMtrx[0][0]) << " | " << pixelConfigMtrx[0][0] << endl;
//	int index = 0;
//	for (int y = 0; y < 256; y++) {
//		for (int bit = DPX_BITS-1; bit >= 0; bit--) // MSB goes first!
//			for (int x = 0; x < 256; x++) {
//				pixelConfigRegister[index++] = (bool)(pixelConfigMtrx[x][y] & (1 << bit));//auxs->GetBitValue(pixelConfigMtrx[x][y], bit);
//				//rawConfigRegister[index] = auxs->GetBitValue(index, bit); rawConfigRegister[index+1] = rawConfigRegister[index];
//				//if (x == MAX_X-1) { cout << "t: " << index << " x: " << x << " y: " << y << " bit: " << bit << " value: " << rawConfigRegister[index];  getchar(); }
//		}
//	}
//	return 0;
//}
//int Dosepix::LinearisePixelConfigMtrx() {
//	//rawConfigRegister[2*MAX_X*MAX_Y*TPX_BITS];
//	//pixelConfigMtrx[MAX_X][MAX_Y];
//	//auxs->GetBitValue();
//	//cout << "pixelConfigMtrx[x][y]: " << cout << bitset<14>(pixelConfigMtrx[0][0]) << " | " << pixelConfigMtrx[0][0] << endl;
//	int index = 0;
//	for (int i = 0; i < 16; i++) {
//		for (int bit = 8; bit >= 0; bit--) // MSB goes first!
//			for (int j = 0; j < 16; j++) {
//				pixelConfigRegister[index++] = (bool)(pixelConfigMtrx[i][j] & (1 << bit));//auxs->GetBitValue(pixelConfigMtrx[i][j], bit);
//				//rawConfigRegister[index] = auxs->GetBitValue(index, bit); rawConfigRegister[index+1] = rawConfigRegister[index];
//				//if (x == MAX_X-1) { cout << "t: " << index << " x: " << x << " y: " << y << " bit: " << bit << " value: " << rawConfigRegister[index];  getchar(); }
//		}
//	}
//	cout<<pixelConfigRegister;
//	// this->UpdateRawConfigRegister();
//	return 0;
//}


//int Dosepix::LinearisePixelDAC() {
//	int index = 0;
//	for (int i = 0; i < 16; i++) {
//		for (int bit = DPX_BITS-1; bit >= 0; bit--) // MSB goes first!
//			for (int j = 0; j < 16; j++) {
//				pixelConfigRegister[index++] = (bool)(pixelDACMtrx[i][j] & (1 << bit));//auxs->GetBitValue(pixelConfigMtrx[i][j], bit);
//		}
//	}
//	cout<<pixelConfigRegister;
//	return 0;
//}

//int Dosepix::LineariseColumnSelect() {
//	int index = 0;
//	for (int i = 0; i < 16; i++) {
//		for (int bit = DPX_BITS-1; bit >= 0; bit--) // MSB goes first!
//			for (int j = 0; j < 16; j++) {
//				pixelConfigRegister[index++] = (bool)(ColSelRegister[i] & (1 << bit));//auxs->GetBitValue(pixelConfigMtrx[i][j], bit);
////				pixelConfigRegister[index++] = auxs->GetBitValue(ColSelRegister[i][j], bit);
//		}
//	}
//	cout<<pixelConfigRegister;
//	return 0;
//}

//int Dosepix::LineariseThDRegister() {
//	int index = 0;
//	for (int i = 0; i < 16; i++) {
//		for (int bit = DPX_BITS-1; bit >= 0; bit--) // MSB goes first!
//			for (int j = 0; j < 16; j++) {
//				pixelConfigRegister[index++] = (bool)(ThDRegister[i][j] & (1 << bit));//auxs->GetBitValue(pixelConfigMtrx[i][j], bit);
////				pixelConfigRegister[index++] = auxs->GetBitValue(ColSelRegister[i][j], bit);
//		}
//	}
//	cout<<pixelConfigRegister;
//	return 0;
//}

//void Dosepix::SetMaskBit(const char mtrx[16][16]) {
//	unsigned char maskBit[16][16]; //for (int i = 0, j = 0; i < 16, j < 16; i++, j++) maskBit[i][j] = 0x0;
//	for (int i = 0; i < 16; i++) {
//		for (int j = 0; j < 16; j++) {
//			maskBit[i][j] = 0x0;
//		}
//	}
//	for (int i = 0; i < 16; i++) {
//		for (int j = 0; j < 16; j++) {
//			cout << std::bitset<1>(maskBit[i][j]) << " ";
//		}
//		cout << "\n";
//	}
//
//	}


//void Dosepix::SetMaskBit(unsigned char value) {
//	unsigned char MaskBit[16][16];
//	for (int i = 0; i < 16; i++) {
//		for (int j = 0; j < 16; j++) {
//			MaskBit[i][j]=0x1;
//
////			 auxs->SetBitValue(pixelConfigMtrx[15-y][15-x], BIT_MASK, mtrx[x][y]);
////			if (mtrx[x][y] == 0)	pixelConfigMtrx[15-y][15-x] &= ~(1 << BIT_MASK);
////			else					pixelConfigMtrx[15-y][15-x] |= 1 << BIT_MASK;
//		}
//		for (int i = 0; i < 16; i++) {
//			for (int j = 0; j < 16; j++) {
//				cout <<std::bitset<1>(MaskBit[i][j])<<" ";
//			}
//	}
//	}
//}
//	this->LinearisePixelConfigMtrx();
//	return 0;




//bool pixelConfigRegister[DPX_DATA_FRAME];

//int Dosepix::LinearisePixelConfigMtrx() {
//rawConfigRegister[2*MAX_X*MAX_Y*TPX_BITS];
//pixelConfigMtrx[MAX_X][MAX_Y];
//auxs->GetBitValue();
//cout << "pixelConfigMtrx[x][y]: " << cout << bitset<14>(pixelConfigMtrx[0][0]) << " | " << pixelConfigMtrx[0][0] << endl;
//int index = 0;
//for (int y = 0; y < 16; y++) {
//	for (int bit = DPX_BITS-1; bit >= 0; bit--) // MSB goes first!
//		for (int x = 0; x < 16; x++) {
//			pixelConfigRegister[index++] = (bool)(pixelConfigMtrx[x][y] & (1 << bit));//auxs->GetBitValue(pixelConfigMtrx[x][y], bit);
//			rawConfigRegister[index] = auxs->GetBitValue(index, bit); rawConfigRegister[index+1] = rawConfigRegister[index];
//			if (x == MAX_X-1) { cout << "t: " << index << " x: " << x << " y: " << y << " bit: " << bit << " value: " << rawConfigRegister[index];  getchar(); }
//	}
//}
// this->UpdateRawConfigRegister();
//return 0;
//}
//
//void Dosepix::PrintUSBTxBuffer(const char* mode, int nBytes) {
//	myUSBDevice->PrintBuffer(mode, nBytes);}


//void Dosepix::PrintColumnSelect() {
//	for (int i = 0; i < 16; i++) {//loops the  rows
//		std::cout << bitset<8>(this->ColSelRegister[i]) << " ";
//		}
//	cout << std::endl;
//}

//int Dosepix::HW_Chip_Reset() {
//	// std::cout << "Dosepix::HW_Chip_Reset()" << std::endl;
//	LOG(INFO) << "Dosepix::HW_Chip_Reset()";
//
//	unsigned char buf[1];
//	buf[0] = 0x72;
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//
//	return 0;
//}

//int Dosepix::HW_SM_Write() {
//
//	std::cout << "Command sent." << std::endl;
//	unsigned char buf[1] = {0x66};
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//
//	myUSBDevice->HW_ReceiveData();
//	return 0;
//}

//int Dosepix::HW_SM_Reset() {
//	std::cout << "Dosepix::HW_SM_Reset()" << std::endl;
//
//	unsigned char buf[1] = {0x64};
//	buf[0] = 0x64;
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//
//	return 0;
//}

//int Dosepix::HW_PeripheryDacHeader() {
//	LOG(DEBUG) << "Header: ";
//	unsigned char txBuf[1] = {0x34};//getter (0x-110100)
//	//memcpy(txBuf,&txBuff,sizeof()
//	myUSBDevice->SetBuffer(txBuf, 1);
//	myUSBDevice->HW_TransmitData();
//
////	getc()
////	cout << hex << (int)txBuf[1] << " ";
////	for (int i = 0; i < DPX_FSR_LENGTH / 8; i++) {}
//	Sleep(10000);
//	return 0;
//}

//int Dosepix::HW_OMRHeader() {
//	LOG(DEBUG) << "Header: ";
//	unsigned char txBuf[1] = {0xC4};//typedef unsigned char unit16_t output (0x-110001)
//	myUSBDevice->SetBuffer(txBuf, 1);
//	myUSBDevice->HW_TransmitData();
////	cout << hex << (int)txBuf[1] << " ";
////	for (int i = 0; i < DPX_FSR_LENGTH / 8; i++) {}
//	Sleep(10000);
//	return 0;
//
//}

//int Dosepix::HW_DataPacket(){
//	test 04
//	unsigned char* bits_;
//	unsigned char* data_;
//	unsigned int datalen_;
//protected:
//	static HW_DataPacket* free_;
//public:
//	HW_DataPacket* next_;  // for queues and the free list
//	static int hdrlen_;
//	HW_DataPacket() : bits_(0), datalen_(0), next_(0) {}
//	unsigned char* const bits() { return (bits_); }
//	HW_DataPacket* copy() const;
//	static HW_DataPacket* alloc();
//	static HW_DataPacket* alloc(int);
//	inline void allocdata(int);
//	static void free(HW_DataPacket*);
//	inline unsigned char* access(int off) {
//	if (off \< 0)
//	abort();
//	return (&bits_[off]);
//	                }
//	inline unsigned char* accessdata() { return data_; }
//	        };

//int Dosepix::HW_Chip_GetID() {
//	LOG(DEBUG) << "";
//
//	myUSBDevice->DiscardBuffer();
//
//	unsigned char buf[1] = {0x72};
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//	myUSBDevice->DiscardBuffer();
//	Sleep(100);
//	buf[0] = 0x66;
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//	myUSBDevice->HW_ReceiveData();
//
//	if ( myUSBDevice->GetBufferSize() != 265 ) {
//		LOG(ERROR) << "Expected buffer size 265, got: " << myUSBDevice->GetBufferSize();
//		return -2;
//	}
//
//	bool bits[264];
//	bool bob = 0;
//
//	for (int i = 0; i < 264; i++) {
//		bool bob = auxs->GetBitValue(myUSBDevice->GetBuffer()[i*CY_FX_GPIF_BUS_WIDTH], 1);
//		bits[263 - i] = bob;
//	}
//
//	// Chip ID reconstruction goes here.
//
//	int deviceType = 0; // For Dosepix = 0 for Timepix = 1
//	deviceType = bits[195]; char deviceTypeS[2][4] = {"MXR", "TPX"};
//	int waferNumber = 0;
//	int xWaferPosition = 0;
//	int yWaferPosition = 0; char yWaferPositionC[256];
//
//	for (int i = 0; i < 11; i++) {
//		auxs->SetBitValue(waferNumber, 10 - i, bits[i + 196]);
//	}
//
//	for (int i = 0; i < 4; i++) {
//		auxs->SetBitValue(xWaferPosition, 3 - i, bits[i + 207]);
//	} xWaferPosition = 12 - xWaferPosition;
//
//	string alphabet = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//
//	for (int i = 0; i < 4; i++) {
//		auxs->SetBitValue(yWaferPosition, 3 - i, bits[i + 211]);
//	}
//
//	sprintf_s(chipID, "%s-%c%02d-W%04d", deviceTypeS[deviceType], alphabet.at(yWaferPosition), xWaferPosition, waferNumber);
//	LOG(INFO) << "Device ID: " << chipID;
//	return 0;
//}

//int Dosepix::HW_Chip_Counting() {
//	LOG(DEBUG) << "";
//	return 0;
//}

//int Dosepix::HW_Chip_ReadSerialPixelMatrix() {
//	LOG(DEBUG) << "";
//	myUSBDevice->DiscardBuffer();
//	unsigned char buf[1] = {0x72};
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//	myUSBDevice->DiscardBuffer();
//	Sleep(2000);
//
//	buf[0] = 0x73;
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//	Sleep(100);
//
//	myUSBDevice->HW_ReceiveBigData();
//
//	//myUSBDevice->PrintBuffer("bin_msb", 1024);
//	//myUSBDevice->DiscardBuffer();
//	Sleep(2000);
//
//	buf[0] = 0x72;
//	myUSBDevice->SetBuffer(buf, 1);
//	myUSBDevice->HW_TransmitData();
//	myUSBDevice->DiscardBuffer();
//	Sleep(2000);
//	return 0;
//}

//int Dosepix::HW_Chip_ReadParallelPixelMatrix() {
//	LOG(DEBUG) << "";
//	return 0;
//}

//int Dosepix::HW_Chip_WritePixelMatrix() {
//	LOG(DEBUG) << "";
//
//	myUSBDevice->DiscardBuffer();
//
////	unsigned char buf[1] = {0x72};
////	myUSBDevice->SetBuffer(buf, 1);
////	myUSBDevice->HW_TransmitData();
////	myUSBDevice->DiscardBuffer();
////	Sleep(2000);
////
////	buf[0] = 0x62;
////	myUSBDevice->SetBuffer(buf, 1);
////	myUSBDevice->HW_TransmitData();
////	myUSBDevice->DiscardBuffer();
////	Sleep(2000);
//
//	for (int j = 0; j < 56; j++) {
//		myUSBDevice->DiscardBuffer();
//		for (int i = 0; i < USB_TX_BUFFER_SIZE; i++) {
//			auxs->SetBitValue(usbTxBuffer[i*CY_FX_GPIF_BUS_WIDTH], 0, pixelConfigRegister[j*USB_TX_BUFFER_SIZE + i]);
//		}
//		myUSBDevice->SetBuffer(usbTxBuffer, USB_TX_BUFFER_SIZE);
//		myUSBDevice->HW_TransmitData();
//	}
//
//	Sleep(2000);
////
////	buf[0] = 0x72;
////	myUSBDevice->SetBuffer(buf, 1);
////	myUSBDevice->HW_TransmitData();
////	myUSBDevice->DiscardBuffer();
////	Sleep(2000);
//
//	return 0;
//}

/**
 * Work in progress... 19/05/2015.
 * This function has not been tested yet.
 */
//int Dosepix::HW_Chip_WritePixelMatrix() {
//	LOG(DEBUG) << "";
//
//	myUSBDevice->DiscardBuffer();
//
////
////	unsigned char buf[1] = {0x72};
////	myUSBDevice->SetBuffer(buf, 1);
////	myUSBDevice->HW_TransmitData();
////	myUSBDevice->DiscardBuffer();
////	Sleep(2000);
////
////	buf[0] = 0x62;
////	myUSBDevice->SetBuffer(buf, 1);
////	myUSBDevice->HW_TransmitData();
////	myUSBDevice->DiscardBuffer();
////	Sleep(2000);
//	unsigned char dataPacket[USB_TX_BUFFER_SIZE +1];
//	unsigned char header[1];
//	header[0] = 0x13; //Threshold header, b'11_0010_xx, see page25
//
//	for (int j = 0; j < 56; j++) {
//		myUSBDevice->DiscardBuffer();
//		for (int i = 0; i < USB_TX_BUFFER_SIZE; i++) {
//			auxs->SetBitValue(usbTxBuffer[i*CY_FX_GPIF_BUS_WIDTH], 0, pixelConfigRegister[j*USB_TX_BUFFER_SIZE + i]);
//		}
//		memcpy(&dataPacket[1], this->pixelConfigRegister,USB_TX_BUFFER_SIZE);
//		myUSBDevice->SetBuffer(dataPacket, USB_TX_BUFFER_SIZE);
//		myUSBDevice->HW_TransmitData();
//	}
//
////	Sleep(2000);
////
////	buf[0] = 0x72;
////	myUSBDevice->SetBuffer(buf, 1);
////	myUSBDevice->HW_TransmitData();
////	myUSBDevice->DiscardBuffer();
////	Sleep(2000);
//
//	return 0;
//}

//int Dosepix::HW_Aux_ReadADC() {
//	LOG(DEBUG) << "";
//	return 0;
//}

/**
 * This function can be simplified.
 */

//int Dosepix::SetDAC(_DACs_Timepix dacNameEnum, int newDac) {
//	LOG(DEBUG) << "";
//	// Filling normal array with DAC values.
//	dacs[dacNameEnum] = newDac;
//
//	// Filling binary peripheryDACRegister array with DAC values from dacs[dacNameEnum].
//	int MSB = 0;
//	int LSB = 0;
//	int verbose = 0;
//
//	switch (dacNameEnum) {
//		case IKrum:			{MSB = IkrumMSB; LSB = IkrumLSB;					if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting IKrum: " << newDac << endl; break;}
//		case Disc:			{MSB = DiscMSB; LSB = DiscLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting Disc: " << newDac << endl; break; }
//		case Preamp:		{MSB = PreampMSB; LSB = PreampLSB;					if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting Preamp: " << newDac << endl; break;}
//
//		case BuffAnalogA:	{ /* Has its own special case */					if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting BuffAnalogA: " << newDac << endl; break;}
//		case BuffAnalogB:	{MSB = BuffAnalogBMSB; LSB = BuffAnalogBLSB;		if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting BuffAnalogB: " << newDac << endl; break;}
//
//		case Hist:			{MSB = HistMSB; LSB = HistLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting Hist: " << newDac << endl; break;}
//		case THL:			{MSB = THLMSB; LSB = THLLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting THL: " << newDac << endl; break;}
//		case THLCoarse:		{MSB = THLCoarseMSB; LSB = THLCoarseLSB;			if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting THL: " << newDac << endl; break;}
//		case Vcas:			{MSB = VcasMSB; LSB = VcasLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting Vcas: " << newDac << endl; break;}
//		case FBK:			{MSB = FBKMSB; LSB = FBKLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting FBK: " << newDac << endl; break;}
//		case GND:			{MSB = GNDMSB; LSB = GNDLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting GND: " << newDac << endl; break;}
//		case THS:			{MSB = THSMSB; LSB = THSLSB;						if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting THS: " << newDac << endl; break;}
//		case BiasLVDS:		{MSB = BiasLVDSMSB; LSB = BiasLVDSLSB;				if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting BiasLVDS: " << newDac << endl; break;}
//		case RefLVDS:		{MSB = RefLVDSMSB; LSB = RefLVDSLSB;				if (verbose >= 2) cout << "INFO: [Timepix::SetDAC]: DAC index " << dacNameEnum << ". Setting RefLVDS: " << newDac << endl; break;}
//		default: {
//			 if (verbose >= 0) cout << "ERROR: [Timepix::SetDAC]: provided DAC index does not exist." << endl; break;
//			}
//	}
//
//	if(dacNameEnum != BuffAnalogA) {
//		for (int i = LSB; i <= MSB; i++) {
//			peripheryDACRegister[i] = auxs->GetBitValue(dacs[dacNameEnum], i - LSB);
//			//std::cout << "peripheryDACRegister[i]: " << peripheryDACRegister[i] << endl;
//		}
//	}
//
//	if(dacNameEnum == BuffAnalogA) {
//		for (int i = BuffAnalogALSB1; i <= BuffAnalogAMSB1; i++){ peripheryDACRegister[i] = auxs->GetBitValue(dacs[dacNameEnum], i - BuffAnalogALSB1); }
//		for (int i = BuffAnalogALSB2; i <= BuffAnalogAMSB2; i++){ peripheryDACRegister[i] = auxs->GetBitValue(dacs[dacNameEnum], 4 + i - BuffAnalogALSB2); }
//	}
//
//	return 0;
//}
//unsigned int Header= 0XD4;
//Header= this->PrintFSR();


