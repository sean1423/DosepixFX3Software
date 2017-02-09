/*
 * FX3USB.cpp
 *
 *  Created on: 23 Apr 2015
 *      Author: Dima Maneuski
 */
//
#include "FX3USB.h"

//changes 2

using namespace std;

FX3USB::FX3USB() {
	txBufferSize = -1;
	//cout << "FX3USB::FX3USB()" << endl;
	hw_isInitialised = false;
	USBDevice = NULL;
	//FX3Device = NULL;
	for (int i = 0; i < 32; i++) {
		USBInterface[i] = NULL;
		USBEndPoint[i] = NULL;
		//USBEndPointOut[i] = NULL;
		//USBEndPointIn[i] = NULL;
	}
	inEndPointID = -1;
	outEndPointID = -1;
	memset(usbTxBuffer, 0, sizeof(usbTxBuffer));
//	buffer = NULL;
//	bufferSize = 0;
	// USBConfig = NULL;
	// USBInterface = NULL;
	// USBEndPointOut = NULL;
	// USBEndPointIn = NULL;
	usbControlEPBufferSize = 0;
	memset(usbControlEPBuffer, 0, sizeof(usbControlEPBuffer));

}

FX3USB::~FX3USB() {
	this->HW_Finalise();
}

int FX3USB::HW_Initalise() {
	LOG(DEBUG) << ":: FX3USB :: " << "";
	//cout << "FX3USB::HW_Initalise()" << endl;
	USBDevice = new CCyUSBDevice(NULL);
	int nDevices = USBDevice->DeviceCount();
	LOG(INFO) << ":: FX3USB :: " << "Number of FX3 USB devices found: " << nDevices;
	if (nDevices == 0) {
		LOG(ERROR) << ":: FX3USB :: " << "No FX3USB devices found!";
		return -1;
	}

	int BytesWritten = 0;
	int OurDevice = 0;
	for (int iDevice = 0; iDevice < nDevices; iDevice++){
		USBDevice->Open(iDevice);
		if ((USBDevice->VendorID == VENDORID) && (USBDevice->ProductID == PRODUCTID)) {
			OurDevice = iDevice;
			//int nEndPoints = USBDevice->EndPointCount();
			//printf("\nDevice found (%d)", iDevice);
			USBDevice->Close();
			//printf("\nClosing device (%d).", iDevice);
		}
		else {
			USBDevice->Close();
			//printf("\nClosing device (%d).", iDevice);
		}

		//printf("\nOurDevice is (%d)", OurDevice);
	}

	USBDevice->Open(OurDevice);
	if (!USBDevice->IsOpen())  {
		LOG(ERROR) << ":: FX3USB :: " << "Cannot open device: " << OurDevice;
		return -1;
	}
	LOG(INFO) << ":: FX3USB :: " << "Device opened OK: " << USBDevice->FriendlyName;
	LOG(INFO) << ":: FX3USB :: " << "Looking for correct endpoints...";

	int nEndPoints = USBDevice->EndPointCount();
	LOG(INFO) << ":: FX3USB :: " << "Number of FX3 USB nEndPoints found: " << nEndPoints;

	for (int iEP = 0; iEP < nEndPoints; iEP++) {
		if ((USBDevice->EndPoints[iEP]->Address) == (UCHAR)(0x01)) {
			outEndPointID = iEP;
			USBEndPoint[outEndPointID] = (CCyUSBEndPoint*) USBDevice->EndPoints[outEndPointID];
			printf_s("\t\tEndpointAddress: 0x%x\n", USBDevice->EndPoints[outEndPointID]->Address);
			LOG(INFO) << ":: FX3USB :: " << "EndpointAddress: " <<  std::hex << USBDevice->EndPoints[outEndPointID]->Address;
		}
		if ((USBDevice->EndPoints[iEP]->Address) == (UCHAR)(0x81)) {
			inEndPointID = iEP;
			USBEndPoint[inEndPointID] = (CCyUSBEndPoint*) USBDevice->EndPoints[inEndPointID];
			printf_s("\t\tEndpointAddress: 0x%x\n", USBDevice->EndPoints[inEndPointID]->Address);
			LOG(INFO) << ":: FX3USB :: " << "EndpointAddress: " <<  std::hex << USBDevice->EndPoints[outEndPointID]->Address;
		}
		if ((USBDevice->EndPoints[iEP]->Address) == (UCHAR)(0x00)) {
			outEndPointID = iEP;
			USBEndPoint[outEndPointID] = (CCyUSBEndPoint*) USBDevice->EndPoints[outEndPointID];
			printf_s("\t\tEndpointAddress: 0x%x\n", USBDevice->EndPoints[outEndPointID]->Address);
			LOG(INFO) << ":: FX3USB :: " << "EndpointAddress: " <<  std::hex << USBDevice->EndPoints[outEndPointID]->Address;
		}
	}

	// Testing that in and out end points are properly inialised and exit...
	//cout << "\tUSBEndPoint[outEndPointID]: " << USBEndPoint[outEndPointID] << endl;
	//cout << "\tUSBEndPoint[inEndPointID] : " << USBEndPoint[inEndPointID]  << endl;
	hw_isInitialised = true;
	return 0;
}
/**
 * Download firmware into FX3 device from file.
 */
int FX3USB::DownloadFw(const char* firmwareFullFileName) {
	printf("\nLoadFirmware Revision 0.1");
	int retCode = -1;
	int seconds = 0;
	int devCount = 0;
	bool go = false;
	DWORD FileSize;
	LONG BytesWritten;
	unsigned char FileBuffer[512 * 1024];		// All images so far have been about 70KB

	CCyFX3Device *FX3Device;
	HANDLE FileHandle;
	FileHandle = CreateFile(firmwareFullFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE) printf("\nCould not open: %s", firmwareFullFileName);
	else {
		retCode = ReadFile(FileHandle, FileBuffer, sizeof(firmwareFullFileName), &FileSize, 0);
		if (!retCode) printf("\nCould not read from: %s", firmwareFullFileName);
		else {
			if (FileSize == sizeof(FileBuffer)) printf("\nInternal Buffer too small, increase!");
			else {
				CloseHandle(FileHandle);
				printf("\nFirmware file: %s", firmwareFullFileName);
				printf("\nBytes read from file: %d", FileSize);
				go = true;
			}
		}
	}

	if (go) {
		// Look for a BootLoader device
		printf("\n");
		FX3_FWDWNLOAD_ERROR_CODE DownloadStatus = FAILED;
		for (seconds = 30; seconds > 0; seconds--) {
			FX3Device = new CCyFX3Device();
			printf("Waiting for a BootLoader %d  \r", seconds);
			devCount = FX3Device->DeviceCount();
			for (int i = 0; i < devCount; i++) {
				if (FX3Device->Open(i)) {
					if (FX3Device->IsBootLoaderRunning()) {
						printf("\nBootLoader found! Downloading firmware...");
						DownloadStatus = FX3Device->DownloadFw((char*)firmwareFullFileName, RAM);
					}
					FX3Device->Close();
				}
			}
			Sleep(1000);
			delete FX3Device;
			if (DownloadStatus == SUCCESS) break;
		}
		if (DownloadStatus != SUCCESS) printf("\nFirmware download failed (%d)", DownloadStatus);
	}
	return retCode;
}

void FX3USB::SetusbControlEPSettings(unsigned char* TestData, int Command) //Sets data to be set and gets what SPI command to execute.
{

	CCyControlEndPoint* ControlEndPoint = (CCyControlEndPoint*)USBEndPoint[0];
	memcpy(TestData, usbControlEPBuffer, usbControlEPBufferSize);
	ControlEndPoint -> Target = TGT_DEVICE;
	ControlEndPoint -> ReqType= REQ_VENDOR;
	ControlEndPoint -> Direction = DIR_TO_DEVICE;

	switch(Command)
	{
	case 1:

		ControlEndPoint -> ReqCode = 0xB0;
		ControlEndPoint -> Value = 0x0000;
		ControlEndPoint -> Index = 0x0000;
		usbControlEPBufferSize = 8;
		break;

	case 2:
		ControlEndPoint -> ReqCode = 0xC2;
		ControlEndPoint -> Value = 0x0000;
		ControlEndPoint -> Index = 0x0000;
		usbControlEPBufferSize = sizeof(TestData);
		break;

	case 3:
		ControlEndPoint -> ReqCode = 0xC3;
		ControlEndPoint -> Value = 0x0000;
		ControlEndPoint -> Index = 0x0000;
		usbControlEPBufferSize = sizeof(TestData);
		break;

	case 4:
		ControlEndPoint -> ReqCode = 0xC4;
		ControlEndPoint -> Value = 0x0000;
		ControlEndPoint -> Index = 0x0000;
		usbControlEPBufferSize = 0;
		break;

	case 5:
		ControlEndPoint -> ReqCode = 0xC4;
		ControlEndPoint -> Value = 0x0000;
		ControlEndPoint -> Index = 0x0000;
		usbControlEPBufferSize = 1;
		break;
	default:
		LOG(DEBUG) << "Default case hit. Case wasn't recognised";
		break;

	}
}

int FX3USB::HW_TransmitDataControlEP()
{

	LOG(DEBUG) << "HW_TransmitDataControlEP";
	int retCode = -1;
	if (USBEndPoint[outEndPointID] != NULL)

	{


		retCode = USBEndPoint[0] -> XferData(this ->usbControlEPBuffer ,this ->usbControlEPBufferSize);
		LOG(DEBUG) << "Transfer Complete. Data Transfered: " << usbControlEPBuffer;
	}
	LOG(DEBUG) << retCode;
	return retCode;
}


/**
 * Transmits data to the FX3 end point.
 */
int FX3USB::HW_TransmitData() {
	LOG(DEBUG) << ":: FX3USB :: " << "";
	int retCode = -1;
	//printf_s("\tTxBuffer size: %d\n", this->bufferSize);
	// printf_s("\tTxBuffer content: %s\n", this->usbTxBuffer);
	//long bufSize = sizeof(usbTxBuffer);

	if (USBEndPoint[outEndPointID] != NULL) {
		//if retcode = USBEndPoint[outEndPointID] = 0x00??
		//_SETUP_PACKET is a struct and we need to put our vendor command data into it and send it to the command end point
		retCode = USBEndPoint[outEndPointID]->XferData(this->usbTxBuffer, this->txBufferSize);
		//printf_s("\tTxBuffer size: %d\n", this->txBufferSize);
		LOG(DEBUG) << ":: FX3USB :: " << "txBufferSize: " << this->txBufferSize;
	} else {
		LOG(ERROR) << ":: FX3USB :: " << "USBEndPoint[outEndPointID] == NULL";
	}

	return retCode;
}

/**
 * Receives data from the FX3 end point.
 */
int FX3USB::HW_ReceiveData() {
	LOG(DEBUG) << "";
	this->DiscardBuffer();
	int retCode = -1;
	long bufLength = USB_TX_BUFFER_SIZE;
	//printf_s("FX3USB::HW_ReceiveData()\n");

	if (USBEndPoint[inEndPointID] != NULL) {
		retCode = USBEndPoint[inEndPointID]->XferData(usbTxBuffer, bufLength);
	}
	LOG(DEBUG) << ":: FX3USB :: " << "retCode: " << retCode;
	LOG(DEBUG) << ":: FX3USB :: " << "bufLength: " << bufLength;
	this->txBufferSize = bufLength;
	return retCode;
}

/**
 * Receives data from the FX3 end point into supplied buffer.
 */
bool FX3USB::HW_ReceiveData(unsigned char* extBuffer, long& length) {
	LOG(DEBUG) << ":: FX3USB :: " << "";
	this->DiscardBuffer();
	int retCode = -1;
	//long bufLength = length;
	//printf_s("FX3USB::HW_ReceiveData()\n");

	if (USBEndPoint[inEndPointID] != NULL) {
		retCode = USBEndPoint[inEndPointID]->XferData(usbTxBuffer, length);
	}
	memcpy(extBuffer, usbTxBuffer, length);
	LOG(DEBUG) << ":: FX3USB :: " << "extBuffer: " << extBuffer;
	LOG(DEBUG) << ":: FX3USB :: " << "retCode: " << retCode;
	LOG(DEBUG) << ":: FX3USB :: " << "bufLength: " << length;
	this->txBufferSize = length;
	return retCode;
}
/**
 * HW_ReceiveBigData.
 */
unsigned char* FX3USB::HW_ReceiveBigData() {
	// LOG(DEBUG) << "";
	int retCode = -1;
	long bufLength = 7*8*16384;
	if (USBEndPoint[inEndPointID] != NULL) {
		retCode = USBEndPoint[inEndPointID]->XferData(bigBuffer, bufLength);
		//LOG(DEBUG) << "Receive XferData retCode: " << retCode;
		//LOG(DEBUG) << "bufLength: " << bufLength;
	}
	return bigBuffer;
}

/**
 * Temporary debug function to set internal buffer.
 */
void FX3USB::SetBuffer(unsigned char* newBuffer, long length) {
	LOG(DEBUG) << ":: FX3USB :: " << "";
	this->DiscardBuffer();
	bool retCode = 0;
	//LOG(DEBUG) << "FX3USB::SetBuffer()";
	//this->buffer = new unsigned char[length];
	memcpy_s(this->usbTxBuffer, USB_TX_BUFFER_SIZE, newBuffer, length);
	this->txBufferSize = length;
	LOG(DEBUG) << ":: FX3USB :: " << "usbTxBuffer: " << this->usbTxBuffer;
	// printf_s("\tTxBuffer size: %d\n", this->bufferSize);
	// printf_s("\tTxBuffer content: %s\n", this->usbTxBuffer);
	// printf_s("\tTxBuffer size: %d\n", this->txBufferSize);
}

/**
 * Print nBytes from the internal buffer on the screen. Available modes: str, hex, bin and bin_msb
 */
int FX3USB::PrintBuffer(const char* mode = "bin_msb", long nBytes = 0) {
	printf_s("FX3USB::PrintBuffer()\n");
	if (strcmp (mode, "str") == 0) {
		printf_s("\t %s\n", this->GetBuffer());
	}

	else if (strcmp (mode, "hex") == 0) {
		printf_s("\t");
		for (int i = 0; i < nBytes; i ++) {
			printf_s("0x%.2X ", this->GetBuffer()[i]);
			if (((i+1)%CY_FX_GPIF_BUS_WIDTH) == 0) printf_s ("\n\t");
		}
		printf_s("\n");
	}

	else if (strcmp (mode, "bin") == 0) {
		printf_s("\tFX3 DQ pins [7:0 15:8 23:16 31:24]\n");
		printf_s("\t-----------------------------------\n");
		printf_s("\t");
		for (int i = 0; i < nBytes; i ++) {
			cout << bitset<8>(this->GetBuffer()[i]) << " ";
			if (((i+1)%CY_FX_GPIF_BUS_WIDTH) == 0) printf_s ("\n\t");
		}
		printf_s("\n");
	} else if (strcmp (mode, "bin_msb") == 0) {
		printf_s("\t--- USE THIS MODE WITH CAUTION ----\n");
		printf_s("\tFX3 DQ pins [0:7 8:15 16:23 24:31]\n");
		printf_s("\t-----------------------------------\n");
		printf_s("\t");
		for (int i = 0; i < nBytes; i ++) {
			char b = this->GetBuffer()[i];
			b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
			b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
			b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
			cout << bitset<8>(b) << " ";
			if (((i+1)%CY_FX_GPIF_BUS_WIDTH) == 0) printf_s ("\n\t");
		}
		printf_s("\n");
	}

	return 0;
}

/**
 * For just now this function returns pointer to the internal data buffer.
 * \\TODO implement validity of buffer.
 * \\TODO implement passing size of the buffer as well as argument.
 */
unsigned char* FX3USB::GetBuffer(){
	//if (usbTxBuffer == NULL) return 0;
	return usbTxBuffer;
}

int FX3USB::DiscardBuffer() {
	//if (buffer != NULL) {
	//	delete [] buffer;
	//	buffer = NULL;
	//}
	this->txBufferSize = 0;
	memset(usbTxBuffer, 0, sizeof(usbTxBuffer));
	return 0;
}

int FX3USB::HW_Finalise() {
	LOG(DEBUG) << ":: FX3USB :: " << "";
	// LOG(DEBUG) << "FX3USB::HW_Finalise()";
	this->DiscardBuffer();
	USBDevice->Close();
	hw_isInitialised = false;
	return 0;
}

int FX3USB::PrintConfigList() {
	int retCode = 0;
	char buf[512];
	string s;
	for (int c=0; c<USBDevice->ConfigCount(); c++) {
		CCyUSBConfig cfg = USBDevice->GetUSBConfig(c);
		sprintf_s(buf,"bLength: 0x%x\n",cfg.bLength); s.append(buf);
		sprintf_s(buf,"bDescriptorType: %d\n",cfg.bDescriptorType); s.append(buf);
		sprintf_s(buf,"wTotalLength: %d (0x%x)\n",cfg.wTotalLength,cfg.wTotalLength); s.append(buf);
		sprintf_s(buf,"bNumInterfaces: %d\n",cfg.bNumInterfaces); s.append(buf);
		sprintf_s(buf,"bConfigurationValue: %d\n",cfg.bConfigurationValue); s.append(buf);
		sprintf_s(buf,"iConfiguration: %d\n",cfg.iConfiguration); s.append(buf);
		sprintf_s(buf,"bmAttributes: 0x%x\n",cfg.bmAttributes); s.append(buf);
		sprintf_s(buf,"MaxPower: %d\n",cfg.MaxPower); s.append(buf);
		s.append("**********************************\n");
		cout << s.c_str();
		s.clear();
		for (int i=0; i<cfg.AltInterfaces; i++) {
			CCyUSBInterface *ifc = cfg.Interfaces[i];
			sprintf_s(buf,"Interface Descriptor:%d\n",(i+1));s.append(buf);
			sprintf_s(buf,"--------------------------------\n");s.append(buf);
			sprintf_s(buf,"bLength: 0x%x\n",ifc->bLength); s.append(buf);
			sprintf_s(buf,"bDescriptorType: %d\n",ifc->bDescriptorType); s.append(buf);
			sprintf_s(buf,"bInterfaceNumber: %d\n",ifc->bInterfaceNumber); s.append(buf);
			sprintf_s(buf,"bAlternateSetting: %d\n",ifc->bAlternateSetting); s.append(buf);
			sprintf_s(buf,"bNumEndpoints: %d\n",ifc->bNumEndpoints); s.append(buf);
			sprintf_s(buf,"bInterfaceClass: %d\n",ifc->bInterfaceClass); s.append(buf);
			sprintf_s(buf,"**********************************\n");s.append(buf);
			cout << s.c_str();
			s.clear();
			for (int e=0; e<ifc->bNumEndpoints; e++) {
				CCyUSBEndPoint *ept =ifc->EndPoints[e+1];
				sprintf_s(buf,"EndPoint Descriptor:%d\n",(e+1));s.append(buf);
				sprintf_s(buf,"--------------------------------\n");s.append(buf);
				sprintf_s(buf,"bLength: 0x%x\n",ept->DscLen); s.append(buf);
				sprintf_s(buf,"bDescriptorType: %d\n",ept->DscType); s.append(buf);
				sprintf_s(buf,"bEndpointAddress: 0x%x\n",ept->Address); s.append(buf);
				sprintf_s(buf,"bmAttributes: 0x%x\n",ept->Attributes); s.append(buf);
				sprintf_s(buf,"wMaxPacketSize: %d\n",ept->MaxPktSize); s.append(buf);
				sprintf_s(buf,"bInterval: %d\n",ept->Interval); s.append(buf);
				s.append("**********************************\n");
				cout << s.c_str();
				s.clear();
			}
		}
	}
	return retCode;
}

/*
void FX3USB::HW_TransmitDataf(unsigned char* newBuffer, long length) {
	printf("Toggle the control pin now!");
	//Sleep(5000);
	this->DiscardBuffer();
	bool retCode = 0;
	printf_s("FX3USB::HW_TransmitData()\n");
	printf_s("\t TxBuffer size: %d\n", length);
	printf_s("\t TxBuffer content: %s\n", newBuffer);
	buffer = new unsigned char[length];
	memcpy_s(this->buffer,length, newBuffer, length); bufferSize = length;
	retCode = USBEndPoint[outEndPointID]->XferData(this->buffer, this->bufferSize);
	//return retCode;
}
*/

/*
void FX3USB::HW_TransmitDataw(unsigned char* newBuffer, long length) {
	printf("Toggle the control pin now!");
	//Sleep(5000);
	this->DiscardBuffer();
	bool retCode = 0;
	printf_s("FX3USB::HW_TransmitData()\n");
	printf_s("\t TxBuffer size: %d\n", length);
	printf_s("\t TxBuffer content: %s\n", newBuffer);
	buffer = new unsigned char[length];
	memcpy_s(this->buffer,length, newBuffer, length); bufferSize = length;
	retCode = USBEndPoint[outEndPointID]->XferData(this->buffer, this->bufferSize);
	//return retCode;
}
*/
