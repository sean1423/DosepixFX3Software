/*
 * Dosepix.h
 *
 *  Created on: 17 Apr 2015
 *      Author: Dima Maneuski
 */
//#include <iomanip>

#ifndef DOSEPIX_H_
#define DOSEPIX_H_

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <bitset>
#include <windows.h>
#include <iomanip>
#include <wtypes.h>
#include <dbt.h>
#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <cstring>
#include <typeinfo>
#include "../FX3USB.dll/FX3USB.h"
#include "../FX3USB.dll/CommonFunctions.h"
//#include "C:\\Documents\\eclipse\\c++\\FX3USB.dll\\easylogging++.h"


#define DPX_FSR_LENGTH		128

#define DPX_PIXCONFIG_BIT_MASK 			3
#define DPX_PIXCONFIG_BIT_TEST_ANALOG 	2
#define DPX_PIXCONFIG_BIT_TEST_DIGITAL 	1

#define DPX_OMR_LENGTH				24
#define DPX_COLSEL_LENGTH			8
#define DPX_CONFIGBITS_LENGTH		2048 // 2048
#define DPX_DIGITAL_THD_LENGTH 		16 //4096
#define DPX_DACBITS_LENGTH			2048 // 2048

/**
 * Enumerator to hold global Dosepix DAC names. The idea is to use these to access DAC position in the array.
 * TODO: Understand whether to place this inside or outside of the Dosepix class.
 */

typedef enum DosepixDACNames {
	VCascReset = 1,
	IPixelDac = 2,
	IKrum = 3,
	VFBK = 4,
	IPreamp = 5,
	Vgnd = 6,
	VCascPreamp = 7,
	IDisc1 = 8,
	IDisc2 = 9,
	ITPBuffIn = 10,
	ITPBuffOut = 11,
	VTPrefCoarse = 12,
	VTPrefFine = 13,
	VThA = 14
} DosepixDACNames;

/**
 * Structure to hold global Dosepix DAC values. The idea is to use these to access DAC position in the array.
 * TODO: Understand whether to place this inside or outside of the Dosepix class.
 */
typedef struct DosepixDACValues {
	int VCascReset;
	int IPixelDac;
	int IKrum;
	int VFBK;
	int IPreamp;
	int Vgnd;
	int VCascPreamp;
	int IDisc1;
	int IDisc2;
	int ITPBuffIn;
	int ITPBuffOut;
	int VTPrefCoarse;
	int VTPrefFine;
	int VThA;
} DosepixDACs;

typedef enum DosepixOMRNames {
	OperationMode = 1,
	GlobalShutter = 2,
	PLL = 3,
	Polarity = 4,
	AnalogOutSel = 5,
	AnalogInSel = 6,
	DisableColClkGate = 7,
	Unassigned = 8
} DosepixOMRNames;

typedef struct DosepixOMRValues {
	int OperationMode;
	int GlobalShutter;
	int PLL;
	int Polarity;
	int AnalogOutSel;
	int AnalogInSel;
	int DisableColClkGate;
	int Unassigned;
} DosepixOMRValues;

typedef enum DosepixThDAddress {
	thD00 = 0,
	thD01 = 1

//	ThD00[i][j]=0;
//	ThD01[i][j]=1;
//	ThD02[i][j]=3;
//	ThD03[i][j]=2;
//	ThD04[i][j]=6;
//	ThD05[i][j]=4;
//	ThD06[i][j]=5;
//	ThD07[i][j]=7;
//	ThD08[i][j]=15;
//	ThD09[i][j]=13;
//	ThD10[i][j]=12;
//	ThD11[i][j]=14;
//	ThD12[i][j]=10;
//	ThD13[i][j]=11;
//	ThD14[i][j]=9;
//	ThD15[i][j]=8;

} DosepixThDAddress;


class Dosepix {

public:
	Dosepix();
	~Dosepix();

	int HW_Initalise();
	int HW_Finalise();

	int HW_Chip_GetChipID(); 					/**< NOT IMPLEMENTED. This function reads chip ID from the device.*/
	int HW_Chip_GlobalReset(); 					/**< NOT IMPLEMENTED. This function performs Global Reset of the device. See definition in the manual. Page 21.*/
	int HW_Chip_DataReset(); 					/**< NOT IMPLEMENTED. This function performs Data Reset of the device. See definition in the manual. Page 21.*/

	// Writing programming data to the chip. Page 25.
	//int HW_Chip_Write_DAC_DATA();				//   This function sends data from the computer to the FX3 in order to be sent via SPI.
	int HW_Chip_WriteOMR(); 					/**< This function sets the Dosepix chip operation mode data.*/
	int HW_Chip_SetConfigurationBits(); 		/**< This function sets configuration of each pixel: mask, analog and digital tests.*/
	int HW_Chip_SetSingleDigitalThreshold();	/**< This function sets single digital threshold registers of the chip.*/
	int HW_Chip_SetPixelDACBits();				/**< This function sets individual pixels trimming bits.*/
	int HW_Chip_WritePeripheryDACRegister(); 	/**< This function programs global DACs of the Dosepix chip.*/
	int HW_Chip_SetColSelRegister();			/**< This function selects column for readout.*/

	// Operation Mode Register related functions. These to be simplified and put into one function with a selector.
	void SetOMR_OperationMode(int value);
	void SetOMR_GlobalShutter(int value);
	void SetOMR_PLL(int value);
	void SetOMR_Polarity(int value);
	void SetOMR_AnalogOutSel(int value);
	void SetOMR_AnalogInSel(int value);
	void SetOMR_DisableColClkGate(int value);
	void SetOMR_Unassigned(int value);

	// This group of functions assigns values in internal buffers to individual pixels before transmitting data.
	void SetPixelConfigBit(bool mtrx[16][16], int bit);
	void SetDigitalThreshold (int thesholdNumber,unsigned short mtrx[16][16]); /**< Sets a specified digital threshold within the pixels*/
	void SetPixelDAC(int mtrx[16][16]);
	void SetDAC (int dacIndex, int newDac);		/**< Sets value for specific global DAC of the chip.*/
	void SetColumnSelect(int col);


private:

    void UpdateRawOMRRegister();
	void UpdateRawPeripheryDACRegister();
	void ResetOMRBuffer() { for (int i = 0; i < DPX_OMR_LENGTH; i++) OMR[i] = 1; }

	DosepixDACs dacs;
	DosepixOMRValues omr;
	DosepixThDAddress thDAddress;
	CommonFunctions* auxs; //private:
	int verbose;//0=ERROR, 1=WARNING, 2=INFO, 3=ALL

	bool hw_isInitialised; 												/**< Shows whether hardware was initialised.*/
	FX3USB* myUSBDevice;												/**< Interface to FX3 hardware.*/
	char chipID[128];

	unsigned char rawPeripheryDACRegister[DPX_FSR_LENGTH / 8];			/**< Raw peripheryDACRegister buffer.*/
	bool peripheryDACRegister[DPX_FSR_LENGTH];							/**< peripheryDACRegister buffer of 1 and 0.*/
	unsigned char pixelConfigMtrx[16][16];								/**< Pixel level configuration matrix (see page 18 of the Dosepix manual).*/
	//unsigned short singleDigitalThD[16][16][16];						/**< Pixel level digital threshold (see page 18 of the Dosepix manual). [Col][Row][TH]*/
	unsigned char pixelDACMtrx[16][16];									/**< Pixel level threshold adjustment DAC matrix (see page 18 of the Dosepix manual).*/


	unsigned short singleDigitalThD00[16][16];
	unsigned short singleDigitalThD01[16][16];
	unsigned short singleDigitalThD02[16][16];
	unsigned short singleDigitalThD03[16][16];
	unsigned short singleDigitalThD04[16][16];
	unsigned short singleDigitalThD05[16][16];
	unsigned short singleDigitalThD06[16][16];
	unsigned short singleDigitalThD07[16][16];
	unsigned short singleDigitalThD08[16][16];
	unsigned short singleDigitalThD09[16][16];
	unsigned short singleDigitalThD10[16][16];
	unsigned short singleDigitalThD11[16][16];
	unsigned short singleDigitalThD12[16][16];
	unsigned short singleDigitalThD13[16][16];
	unsigned short singleDigitalThD14[16][16];
	unsigned short singleDigitalThD15[16][16];

    unsigned char rawOMR[DPX_OMR_LENGTH / 8];
    bool OMR[DPX_OMR_LENGTH];

    unsigned char ColSelRegister;
    unsigned char  ThDRegister[16][16];

	static const int VCasResetMSB= 127;				static const int VCasResetLSB= 120;
	static const int IPixelDacMSB= 119;				static const int IPixelDacLSB= 112;
	static const int IKrumMSB= 111;					static const int IKrumLSB= 104;
	static const int VFBKMSB= 103;					static const int VFBKLSB= 96;
	static const int IPreampMSB	= 95;				static const int IPreampLSB	= 88;
	static const int VgndMSB= 87;					static const int VgndLSB= 80;
	static const int VCascPreampMSB	= 79;			static const int VCascPreampLSB	= 72;
	static const int IDisc1MSB	= 71;				static const int IDisc1LSB	= 64;
	static const int IDisc2MSB	= 63;				static const int IDisc2LSB	= 56;
	static const int ITPBuffInMSB= 55;				static const int ITPBuffInLSB= 48;
	static const int ITPBuffOutMSB= 47;				static const int ITPBuffOutLSB= 40;
	static const int VTPrefCoarseMSB= 39;			static const int VTPrefCoarseLSB= 32;
	static const int VTPrefFineMSB= 24;				static const int VTPrefFineLSB	= 16;
	static const int VThAMSB= 12;					static const int VThALSB= 0;

public:

	// Extra debug functions go here...
	//	void PrintFSR(int startPosition = 0, int endPosition = 175); // commented by me
	void PrintPixelConfiguration(int col = 0, int row = 0, const char* mode = "bin"); /**< Available modes: bin, dec.*/
	void PrintUSBTxBuffer(const char* mode = "str", int nBytes = USB_TX_BUFFER_SIZE); /**< Prints buffer to be sent over USB.*/
	void PrintThDRegister();//bool mtrx[16][16], int bit
	void PrintDACs();
	void PrintRawFSR();
	void PrintFSR();
	int LoadDACsFromFile(const char* dacsFileName);
	void PrintPixelConfig();
	void PrintDigitalThD();
	void PrintPixelDAC();
	void PrintColumnSelect();
	void PrintRawOMR();
	void PrintOMR();
	void PrintomrModes();

	//	void SetConfigBits();
	//	void Dosepix::SetMaskBit(unsigned char value); //allows individula pixels to be disabled digitally 0=normal 1=disable dig processing circuits in this pixel
	//	void SetTestBit_Analog(unsigned char value);// 0=i/p to preamp= charge from sensor 1= i/p to preamp is Qtest
	//	void SetTestBit_Digital(unsigned char value);//0=i/p to digital circuits is the discrimminator o/p
	//	void SetConfig_Unassigned(unsigned char value);//0=i/p to digital circuits is testpulse

	//	int HW_Chip_Counting();
	//	int HW_Chip_ReadSerialPixelMatrix();
	//	int HW_Chip_ReadParallelPixelMatrix();
	//	int HW_Chip_WritePixelMatrix();


	//	int HW_PeripheryDacHeader();
	//	int HW_OMRHeader();
	//	int HW_Aux_ReadADC();
	//  int _DACs_Timepix dacNameEnum;
	//	int GetDAC(_DACs_Timepix dacNameEnum);
	//	int SetMonitorDAC(_DACs_Timepix dacNameEnum);

	//	const char* GetChipID() { return chipID; }

	//	Set individual Timepix matrix bits
	//	int LinearisePixelConfigMtrx();
	//	int LinearisePixelDAC();
	//	int LineariseColumnSelect();
	//	int LineariseThDRegister();
	//	int SetMode(int timepix_mode);
	//	int SetMode(const int mtrx[256][256]);
	//	int SetMask(const int mtrx[256][256]);
	//	void SetThreshold(const int mtrx[16][16], int bit);
	//	void SetMaskBit(const char mtrx[16][16]);
	//	int SetTestBit(const int mtrx[256][256]); /**< Will not be active for the time being.*/
};

#endif /* DOSEPIX_H_ */
