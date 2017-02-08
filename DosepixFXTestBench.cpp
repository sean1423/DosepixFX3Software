//============================================================================
// Name        : FX3USBTestBench.cpp
// Author      : Dima Maneuski
// Version     :
// Copyright   : Your copyright notice
// Description : FX3USB. TestBench for FX3USB class.
//============================================================================


// #include <CyAPI.h>
//#include <cyu3spi.h> dont know why this is here 1/2/17
using namespace std;


//#include "../DosepixFX.dll/DosepixFX.h"
#include "C:\Documents\eclipse\c++\DosepixFX.dll/DosepixFX.h"

//#include "../"
_INITIALIZE_EASYLOGGINGPP

// adding stuff

unsigned char txBuf[16384];
long txBufSize = sizeof(txBuf);

void Test00() {
	unsigned char txBuf[16384]; for (int i = 0; i < 16384; i++) txBuf[i] = 0xFF;
	long txBufSize = sizeof(txBuf);
	//txBuf[0]  = 0x1;
	txBuf[4]  = 0x1;
	txBuf[8]  = 0x0;
	txBuf[12] = 0x1;

	txBuf[8168]  = 0x1;
	txBuf[8172]  = 0x1;
	txBuf[8176]  = 0x0;
	txBuf[8180]  = 0x1;
	txBuf[8184]  = 0x1;
	txBuf[8188]  = 0x0;


	//unsigned char rxBuf[8192];
	//long rxBufSize = sizeof(txBuf);

	//unsigned char buff[1] = {0};

	FX3USB* myUSBDevice = new FX3USB();
	//myUSBDevice->SetBuffer(foo.to_string(), 16);
	myUSBDevice->HW_Initalise();
//	//myUSBDevice->HW_TransmitDataw(writecmnd, writecmndSize);
	txBuf[0] = 0x77;
	myUSBDevice->SetBuffer(txBuf, txBufSize);
	myUSBDevice->HW_TransmitData();
//	Sleep(5000);
//	txBuf[0] = 0x77;
//	myUSBDevice->SetBuffer(txBuf, txBufSize);
//	myUSBDevice->HW_TransmitData();
	Sleep(5000);
	txBuf[0] = 0x72;
	myUSBDevice->SetBuffer(txBuf, txBufSize);
	myUSBDevice->HW_TransmitData();
//	Sleep(5000);
//	txBuf[0] = 0x72;
//	myUSBDevice->SetBuffer(txBuf, txBufSize);
//	myUSBDevice->HW_TransmitData();
//	myUSBDevice->PrintBuffer("str");
//	myUSBDevice->PrintBuffer("hex");
//	myUSBDevice->PrintBuffer("bin");
//	myUSBDevice->HW_TransmitDataf(readcmnd, readcmndSize);
//	myUSBDevice->HW_ReceiveData(rxBuf, rxBufSize);
//	myUSBDevice->PrintBuffer("str");
//	myUSBDevice->PrintBuffer("hex");
//	myUSBDevice->PrintBuffer("bin");
	myUSBDevice->HW_Finalise();
}

void Test_LargeBuffer_TxFer() {

	unsigned char txBuf[1] = {0x77};
	long txBufSize = sizeof(txBuf);

	FX3USB* myUSBDevice = new FX3USB();
	myUSBDevice->HW_Initalise();
	myUSBDevice->SetBuffer(txBuf, txBufSize);
//	myUSBDevice->PrintBuffer("bin_msb", txBufSize);

	myUSBDevice->HW_Finalise();
}

void Test_Simple_TxFer() {
	unsigned char txBuf[1] = {0x66};
	long txBufSize = sizeof(txBuf);

	LOG(INFO) << "txBufSize: " << txBufSize;
	FX3USB* myUSBDevice = new FX3USB();
	myUSBDevice->HW_Initalise();
	myUSBDevice->SetBuffer(txBuf, txBufSize);
	myUSBDevice->PrintBuffer("bin_msb", txBufSize);
	myUSBDevice->HW_TransmitData();
	myUSBDevice->HW_Finalise();
}

void Test_Rx_Benchmark() {
	unsigned char* bigBuffer;

	ofstream myfile;
	myfile.open ("C:\\example.bin", ios::out | ios::app | ios::binary);

	FX3USB* myUSBDevice = new FX3USB();
	myUSBDevice->HW_Initalise();

	for (int i = 0; i < 143; i++) {
		bigBuffer = myUSBDevice->HW_ReceiveBigData();
		myfile << bigBuffer;
	}
	myfile.close();

	for (int i = 0; i < 10; i ++) {
		printf_s("0x%.2X ", bigBuffer[i]);
		if (((i+1)%CY_FX_GPIF_BUS_WIDTH) == 0) printf_s ("\n\t");
	}
//	myUSBDevice->PrintBuffer("hex", myUSBDevice->GetBufferSize());
//	myUSBDevice->HW_ReceiveBigData();
	// myUSBDevice->PrintConfigList();
//	myUSBDevice->HW_Finalise();
//}
}
void Test01() {
	unsigned char txBuf[6] = {0xAA,0xAA,0xFF,0xAA,0xFF,0xAA};//typedef unsigned char unit16_t output

	FX3USB* myUSBDevice = new FX3USB();
	myUSBDevice->HW_Initalise();
	myUSBDevice->SetBuffer(txBuf, 6);
	myUSBDevice->HW_TransmitData();
	myUSBDevice->HW_Finalise();
}


//  Periphery DAC test

void Test02() {
	Dosepix* myDevice = new Dosepix();
	myDevice->HW_Initalise();
//	myDevice->PrintDACs();
	for (int i = 0; i < 8191; i++) {
	myDevice->SetDAC(1,i);
	myDevice->PrintRawFSR();
	myDevice->HW_Chip_WritePeripheryDACRegister();
		Sleep(5000);
	}
	myDevice->PrintFSR();
	myDevice->PrintRawFSR();
	myDevice->HW_Finalise();
}
/**
 *Header test
 */
//void Test03() {
//
//	unsigned char txBuf[1] = {0x34};
//	Dosepix* myDevice = new Dosepix();
//	myDevice->HW_Initalise();
//	LOG(DEBUG) << "";
//	myDevice->HW_PeripheryDacHeader();
//	myDevice->HW_Finalise();
////	myDevice->PrintRawFSR();
//}


/**
 *Header command + Data payload test
 */
//
//void Test07() {
//	Dosepix* myDevice = new Dosepix();
//	myDevice->HW_Initalise();
//	myDevice->PrintDACs();
//	struct DacDataPayload {bool FSR[DPX_FSR_LENGTH];};
//	struct Command {
//		struct PeripheryDacHeader{
////			unsigned char txBuf[1] = {0x34};
//			unsigned char id;
//			unsigned char arg_size;
////			int dacHeader;
////			Header= *(int*)txBuf;
//		};
//		for (int i = 0; i < 8191; i++) {
//		myDevice->SetDAC(14,i);
//		myDevice->PrintRawFSR();}
//
//		PeripheryDacHeader hdr;
//		bool arg[DPX_FSR_LENGTH];
//
//	};
//
//
//
//	myDevice->HW_Initalise();
//	myDevice->HW_PeripheryDacHeader();
//
//	myDevice->SetDAC(14, 8191);
//	myDevice->PrintDACs();
//
//	unsigned int temp= 212 + 8191;//212
//	myDevice->SetDAC(1,Header);
//	myDevice->PrintFSR();
//	myDevice->PrintRawFSR();
//	myDevice->HW_Chip_WriteFSR();
//	Sleep(10);
//	myDevice->HW_Finalise();
//}
//void Test04() {
//	Dosepix* myDevice = new Dosepix();
//	myDevice->HW_Initalise();
//	myDevice->SetDAC(14, 0);
//	myDevice->PrintDACs();
//	myDevice->PrintRawFSR();
//	myDevice->PrintFSR();
//	myDevice->HW_Chip_WriteFSR();
//	Sleep(10);
//	myDevice->HW_Finalise();
//}

/**
 * OMR test
 */

void Test05() { // OMR test

	cout << "         Beginning Test05      Beginning Test05         Beginning Test05         Beginning Test05";

//	unsigned char txBuf[1] = {0xC4};
	Dosepix* myDevice = new Dosepix();
	//getchar();
	myDevice->HW_Initalise();
	//getchar();
//	myDevice->HW_OMRHeader();
//	myDevice->HW_Finalise();
//	myDevice->HW_Initalise();
	myDevice->SetOMR_OperationMode(0);
//	myDevice->SetOMR_OperationMode(1);
//	myDevice->SetOMR_OperationMode(2);
	//myDevice->SetOMR_OperationMode(3);
	//getchar();
	//myDevice->SetOMR_GlobalShutter(0);
	//getchar();
	//myDevice->SetOMR_GlobalShutter(1);

	//myDevice->SetOMR_PLL(0);
	//getchar();
	myDevice->SetOMR_PLL(1);
//	myDevice->SetOMR_PLL(2);
//	myDevice->SetOMR_PLL(3);
//	myDevice->SetOMR_PLL(4);
//	myDevice->SetOMR_PLL(5);
//	myDevice->SetOMR_PLL(6);
//	myDevice->SetOMR_PLL(7);


	myDevice->SetOMR_Polarity(0);
	//getchar();
//	myDevice->SetOMR_Polarity(1);

	myDevice->SetOMR_AnalogOutSel(1);   // Sets the AnalogOutSel to the specified value.
	//getchar();
//	myDevice->SetOMR_AnalogOutSel(2);
//	myDevice->SetOMR_AnalogOutSel(3);
//	myDevice->SetOMR_AnalogOutSel(4);
//	myDevice->SetOMR_AnalogOutSel(5);
//	myDevice->SetOMR_AnalogOutSel(6);
//	myDevice->SetOMR_AnalogOutSel(7);
//	myDevice->SetOMR_AnalogOutSel(8);
//	myDevice->SetOMR_AnalogOutSel(9);
//	myDevice->SetOMR_AnalogOutSel(10);
//	myDevice->SetOMR_AnalogOutSel(11);
//	myDevice->SetOMR_AnalogOutSel(12);
//	myDevice->SetOMR_AnalogOutSel(13);
//	myDevice->SetOMR_AnalogOutSel(14);
//	myDevice->SetOMR_AnalogOutSel(15);
//	myDevice->SetOMR_AnalogOutSel(27);
//	myDevice->SetOMR_AnalogOutSel(28);
//	myDevice->SetOMR_AnalogOutSel(29);
//	myDevice->SetOMR_AnalogOutSel(31);

	myDevice->SetOMR_AnalogInSel(1);   // Sets the AnalogInSel to the specified value.
//	myDevice->SetOMR_AnalogInSel(2);
	//myDevice->SetOMR_AnalogInSel(3);
//	myDevice->SetOMR_AnalogInSel(4);
//	myDevice->SetOMR_AnalogInSel(5);
//	myDevice->SetOMR_AnalogInSel(6);
//	myDevice->SetOMR_AnalogInSel(7);
//	myDevice->SetOMR_AnalogInSel(8);
//	myDevice->SetOMR_AnalogInSel(9);
//	myDevice->SetOMR_AnalogInSel(10);
//	myDevice->SetOMR_AnalogInSel(11);
//	myDevice->SetOMR_AnalogInSel(12);
//	myDevice->SetOMR_AnalogInSel(13);
//	myDevice->SetOMR_AnalogInSel(14);
//	myDevice->SetOMR_AnalogInSel(15);
//	myDevice->SetOMR_AnalogInSel(28);
//	myDevice->SetOMR_AnalogInSel(29);
//	myDevice->SetOMR_AnalogInSel(31);

	//myDevice->SetOMR_DisableColClkGate(0); // Chooses to enable column level clock gating
	myDevice->SetOMR_DisableColClkGate(1); // Chooses to disable column level clock gating

	myDevice->SetOMR_Unassigned(0);

	myDevice->PrintRawOMR(); // Prints Raw OMR Data to the console
	myDevice->PrintOMR();    // Prints OMR Binary Data to the console
	myDevice->PrintomrModes(); // Prints all OMR Modes
	myDevice->HW_Chip_WriteOMR(); //Writes the data to the OMR
	Sleep(10);
	myDevice->HW_Finalise();
}


//void Test06() {
//	unsigned char txBuf[1] = {0xC4};
//	Dosepix* myDevice = new Dosepix();
//	myDevice->HW_Initalise();
////	myDevice->SetBuffer(txBuf,1);
////	LOG(DEBUG) << "";
//	myDevice->HW_OMRHeader();
//	myDevice->HW_Finalise();
////	myDevice->PrintRawFSR();
//}

void Test08() {
	Dosepix* myDevice = new Dosepix();
	myDevice->HW_Initalise();
	bool MaskBit[16][16];
	bool TestBit_Analog[16][16];
	bool TestBit_Digital[16][16];
	for (int i = 0; i < 16; i++) { //loops the  rows
		for (int j = 0; j < 16; j++) { //loops the columns
			MaskBit[i][j] = 0;
			TestBit_Analog[i][j] = 1;
			TestBit_Digital[i][j] = 0;
		}
	}
	myDevice->SetPixelConfigBit(MaskBit, 2);
	myDevice->SetPixelConfigBit(TestBit_Analog, 1);
	myDevice->SetPixelConfigBit(TestBit_Digital, 0);
	//myDevice->LinearisePixelConfigMtrx();
	//myDevice->PrintPixelConfig();
	myDevice->HW_Chip_SetConfigurationBits();
	myDevice->HW_Finalise();
}

void Test09() {
	Dosepix* myDevice = new Dosepix();
	myDevice->HW_Initalise();
	int thAdjustmentCode[16][16];

	for (int i = 0; i < 16; i++) { //loops the  rows
		for (int j = 0; j < 16; j++) { //loops the columns
			thAdjustmentCode[i][j] = 1;
		}
	}
	myDevice->SetPixelDAC(thAdjustmentCode);
	myDevice->HW_Chip_SetPixelDACBits();
	myDevice->HW_Finalise();
}

void Test10() {
	Dosepix* myDevice = new Dosepix();
	myDevice->HW_Initalise();
	int colSel = 10;
//	int colSel1[16];
//	int colSel2[16];
	for (int i = 0; i < 16; i++) { //loops the  rows
		myDevice->SetColumnSelect(i);
		myDevice->HW_Chip_SetColSelRegister();
		Sleep(1000);
	}
	myDevice->HW_Finalise();
}

void Test12() {
	unsigned short init = 4095;
	unsigned short bit = 15;
	std::cout << std::bitset<16>(init) << std::endl;
	bit <<= 12;
	init += bit;
	std::cout << std::bitset<16>(init) << std::endl;

}

void Test11(){
	Dosepix* myDevice = new Dosepix();
	myDevice->HW_Initalise();
	unsigned short SingleDigitalThDCode[16][16];
	int counter = 1;
//for (int th = 0; th < 16; th++) {
		for (int i = 0; i < 16; i++) { //loops the  rows
			for (int j = 0; j < 16; j++) { //loops the columns
				SingleDigitalThDCode[i][j] = 255; // 12 bit threshold value, in binary code
			//}
		}
	}

	//for (int th = 0; th < 1; th++) {
//		myDevice->SetDigitalThreshold(0,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(1,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(2,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(3,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(4,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(5,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(6,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(7,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(8,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(9,  SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(10, SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(11, SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(12, SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(13, SingleDigitalThDCode);
//		myDevice->SetDigitalThreshold(14, SingleDigitalThDCode);
		myDevice->SetDigitalThreshold(15, SingleDigitalThDCode);
		myDevice->PrintDigitalThD();
		myDevice->HW_Chip_SetSingleDigitalThreshold();

	//}

	myDevice->HW_Finalise();
}

// Test 13 is used to configure the SPI block on the FX3 device and use MOSI, MISO, SCK and CS lines.
void Test13()
{

	unsigned char DAC_Data[2] = {0xFF,0xFF};//typedef unsigned char unit16_t output

	FX3USB* myUSBDevice = new FX3USB();
	LOG(DEBUG) << "Class has been called" ;
	myUSBDevice->HW_Initalise();
	LOG(DEBUG) << "Initialising" ;
	LOG(DEBUG) << "" ;
	myUSBDevice->SetBuffer(DAC_Data, 2);
	LOG(DEBUG) << "Setting Buffer" ;
	LOG(DEBUG) << "" ;
	myUSBDevice->HW_TransmitData();
	LOG(DEBUG) << "Transmitting" ;
	LOG(DEBUG) << "" ;
	myUSBDevice->HW_Finalise();

}




//void process_buffer(unsigned char *txBuf, size_t, *rawOMR)
//{
////	int result;
////	int recv;
////	int sock;
////	int process_buffer;
////	unsigned char recv_buff[DPX_OMR_LENGTH+1];
//	unsigned char recv_buff[25];
////	size_t recv_len=0;
////	result=recv(sock,recv_buff+25,25-recv_len,0);
////			if (result >0){
////				recv_len +=result;
////				process_buffer(recv_buff,&recv_len);
////			}
////
//	unsigned char txBuf[1] = {0xC4};
//	Dosepix* myDevice = new Dosepix();
//	myDevice->HW_Initalise();
//	myDevice->HW_OMRHeader();
//	myDevice->HW_Finalise();
//
//	while (*len >=25){
//		unsigned payload_len =rawOMR[DPX_OMR_LENGTH];
//		if (*len <3 +payload_len){
//			break;
//		}
//		do_command(buffer[0],buffer[1],payload_len, &buffer[3]);
//		 *len -= 3 + payload_len;
//		        if (*len > 0)
//		            memmove(buffer, buffer + 3 + payload_len, *len);
//		    }
//
//	}





//Dosepix::SetDAC(_DACs_Timepix dacNameEnum, int newDac) { {//int Dosepix::SetDAC(int dacNameEnum, int newDac) {
//	LOG(DEBUG) << "";
//	// Filling normal array with DAC values.
//	dacs[dacNameEnum] = newDac;
//
//	// Filling binary PeripheryDACRegister array with DAC values from dacs[dacNameEnum].
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
//			PeripheryDACRegister[i] = auxs->GetBitValue(dacs[dacNameEnum], i - LSB);
//			//std::cout << "PeripheryDACRegister[i]: " << PeripheryDACRegister[i] << endl;
//		}
//	}
//
//	if(dacNameEnum == BuffAnalogA) {
//		for (int i = BuffAnalogALSB1; i <= BuffAnalogAMSB1; i++){ PeripheryDACRegister[i] = auxs->GetBitValue(dacs[dacNameEnum], i - BuffAnalogALSB1); }
//		for (int i = BuffAnalogALSB2; i <= BuffAnalogAMSB2; i++){ PeripheryDACRegister[i] = auxs->GetBitValue(dacs[dacNameEnum], 4 + i - BuffAnalogALSB2); }
//	}
//
//	return 0;
//}
//
//void Dosepix::PrintFSR(int startPosition, int endPosition) {
//	LOG(DEBUG) << "";
//	std::cout << "\tPosition\tValue" << std::endl;
//	for (int i = startPosition; i < endPosition; i++)
//		std::cout << "\t" << i << "\t" << PeripheryDACRegister[i] << std::endl;
//}
//
//
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
//int Dosepix::GetDAC(_DACs_Timepix dacNameEnum) {//_DACs_Timepix dacNameEnum
//	LOG(DEBUG) << "";
//	return dacs[dacNameEnum];
//}
////int Dosepix::SetMonitorDAC(_DACs_Timepix dacNameEnum) {
////	LOG(DEBUG) << "";
////	//dacIndex = 1;
////	int DACCode = 0;
////	int verbose = 0;
////	switch (dacNameEnum) {
////		case IKrum:				{ DACCode = 15; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring IKrum" << endl; break;}
////		case Disc:				{ DACCode = 11; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Disc" << endl; break; }
////		case Preamp:			{ DACCode = 7;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Preamp" << endl; break;}
////		case BuffAnalogA:		{ DACCode = 3;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BuffAnalogA" << endl; break;}
////		case BuffAnalogB:		{ DACCode = 4;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BuffAnalogB" << endl; break;}
////		case Hist:				{ DACCode = 9;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Hist" << endl; break;}
////		case THL:				{ DACCode = 6;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring THL" << endl; break;}
////		case THLCoarse:			{ DACCode = 6;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring THL" << endl; break;}
////		case Vcas:				{ DACCode = 12; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring Vcas" << endl; break;}
////		case FBK:				{ DACCode = 10; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring FBK" << endl; break;}
////		case GND:				{ DACCode = 13; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring GND" << endl; break;}
////		case THS:				{ DACCode = 1;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring THS" << endl; break;}
////		case BiasLVDS:			{ DACCode = 2;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BiasLVDS" << endl; break;}
////		case RefLVDS:			{ DACCode = 14; if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring RefLVDS" << endl; break;}
////		case BiasDAC:			{ DACCode = 0;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BiasDAC" << endl; break;}
////		case BiasOutStage:		{ DACCode = 5;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring BiasOutStage" << endl; break;}
////		case vbgOnChip:			{ DACCode = 8;  if (verbose >= 2) cout << "INFO: [Timepix::MonitorDAC]: DAC index " << dacNameEnum << ". Monitoring vbgOnChip" << endl; break;}
////		default: {
////			if (verbose >= 1) cout << "WARNING: [Timepix::MonitorDAC]: provided DAC index does not exist.. Setting DAC index to IKrum" << endl;
////			DACCode = 15; break;
////				}
////	}
////
////	int SenseDAC = 42;
////	int ExtDACSel = 43;
////	int DACCodeBits[] = {37, 38, 40, 41};
////
////	// Sense DAC
////	PeripheryDACRegister[SenseDAC] = 1;
////	// Ext DAC select
////	PeripheryDACRegister[ExtDACSel] = 0;
////	// DACCode = 0;
////
////	PeripheryDACRegister[DACCodeBits[3]] = auxs->GetBitValue(DACCode, 3);
////	PeripheryDACRegister[DACCodeBits[2]] = auxs->GetBitValue(DACCode, 2);
////	PeripheryDACRegister[DACCodeBits[1]] = auxs->GetBitValue(DACCode, 1);
////	PeripheryDACRegister[DACCodeBits[0]] = auxs->GetBitValue(DACCode, 0);
////
////	// this->UpdateRawFSR();
////	return 0;
////}


// }
int main(int argc, _TCHAR* argv[]) {
	LOG(DEBUG) << "main";
//	setvbuf(stdout, NULL, _IONBF, 0);
//	setvbuf(stderr, NULL, _IONBF, 0); // these two lines make console output work in Eclipse. Don't ask why and how...
//	Dosepix* myDevice = new Dosepix();
//	myDevice->PrintDACs();
//	myDevice->SetDAC(17, 8191);
//	myDevice->SetDAC(1, 255);
//	myDevice->PrintFSR();
//	myDevice->PrintRawFSR();
// 	Test_LargeBuffer_TxFer();
// 	Test_Simple_TxFer();
//	Test_Rx_Benchmark();
//	Test01();
//	Test02(); // PeripheryDACRegister*/
//	Test03(); // Header test
//	Test04(); //Concatenating header and command data payload
//	Test05(); // OMR test;
//	Test06(); //OMR header test
//	Test07(); // header+data payload take 2
//	Test08(); // SetPixelConfigurationBits
//	Test11(); //Set Single Digital Threshold
//	Test12(); // bitwise operations
	Test13(); //SPI Block Test
	return 0;
}

