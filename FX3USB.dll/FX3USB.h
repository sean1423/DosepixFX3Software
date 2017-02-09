/*
 * FX3USB.h
 *
 *  Created on: 23 Apr 2015
 *      Author: Dima
 */

#ifndef FX3USB_H_
#define FX3USB_H_

#include <iostream>
#include <bitset>
#include <Windows.h>
#include "CommonFunctions.h"

// FX3 related includes
#include <wtypes.h>
#include <dbt.h>
//#include <CyAPI.h>
//#include <C:\\Program Files (x86)\\Cypress\\EZ-USB FX3 SDK\\1.3\\library\\cpp\\inc\\CyAPI.h>
#include <C:\\Documents\\eclipse\\c++\\DosepixFX.dll\\cypress\\inc\\CyAPI.h>


#define VENDORID 0x04B4 /**< Cypress vendor ID*/
#define PRODUCTID 0x00F1
#define CY_FX_EP_USB_PRODUCER 0x01
#define CY_FX_EP_USB_CONSUMER 0x81

#define USB_TX_BUFFER_SIZE 16384
#define USB_RX_BUFFER_SIZE 16384

#define CY_FX_GPIF_BUS_WIDTH 1 /**< GPIF data bus width in bytes.*/

class FX3USB {
public:
	FX3USB();
	int HW_Initalise();
	int HW_Finalise();
	int DownloadFw(const char* firmwareFullFileName);
	//void HW_TransmitDataw(unsigned char* bufferw, long lengthw);
	//bool HW_TransmitData(unsigned char* newBuffer, long length); /**< Sends data to USB port. */
	void SetusbControlEPSettings(unsigned char* TestData, int Command); // New function which takes data and puts the data into the Control Endpoint Buffer
	int HW_TransmitDataControlEP(); // This is a new function inside the FX3USB class which can be used to send data over the Control Endpoint.
	int HW_TransmitData(); /**< Sends data to USB port using internal buffer. Use SetBuffer(...) function to fill the buffer. */
	//void HW_TransmitDataf(unsigned char* bufferf, long lengthf);
	bool HW_ReceiveData(unsigned char* extBuffer, long& length); /**< Receives data from the FX3 end point into supplied buffer. */
	int HW_ReceiveData();
	unsigned char* HW_ReceiveBigData();

	int PrintConfigList();
	//int GetBuffer(unsigned char& buffer, long& length);
	void SetBuffer(unsigned char* newBuffer, long length); /**< Use this function to set Rx and Tx data. */
	unsigned char* GetBuffer();
	long GetBufferSize() { return txBufferSize; } /**< Get receive buffer size. */
	int PrintBuffer (const char* mode, long nBytes);
	int DiscardBuffer();

	bool IsInitialised() { return hw_isInitialised; }

	virtual ~FX3USB();
private:
	bool hw_isInitialised;
	//CCyFX3Device* FX3Device;
	CCyUSBDevice* USBDevice;
	CCyUSBConfig USBConfig[32];
	CCyUSBInterface* USBInterface[32];
	CCyUSBEndPoint* USBEndPoint[32];

	int  inEndPointID;
	int outEndPointID;
	unsigned char usbTxBuffer[USB_TX_BUFFER_SIZE];
	long txBufferSize;

	unsigned char usbControlEPBuffer[512]; // Should be able to be reduced to 2 Bytes.
	long usbControlEPBufferSize;
	unsigned char bigBuffer[917768];

	//CCyUSBEndPoint* USBEndPointOut[32];
	//CCyUSBEndPoint* USBEndPointIn[32];
};

#endif /* FX3USB_H_ */
