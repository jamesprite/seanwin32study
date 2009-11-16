#include <windows.h>
#include <stdio.h>
#include <winioctl.h>

#include "RTBTH_USB.h"
#include "mttty.h"


/*

The sample to use RTBTH Sample by SeanWang @Raink

*/

#define pNULL NULL

extern VOID DBG_DumpBuffer(PVOID Buffer, LONG Length, ULONG Offset);

typedef unsigned char u_int8;       /* unsigned integer,  8 bits long */
typedef unsigned short u_int16;     /* unsigned integer, 16 bits long */
typedef unsigned int u_int32;       /* unsigned integer, 32 bits long */

typedef unsigned int u_int;

typedef signed char s_int8;         /* unsigned integer,  8 bits long */
typedef signed short s_int16;       /* unsigned integer,  8 bits long */
typedef signed long s_int32;        /* unsigned integer,  8 bits long */


#define USB_DEFAULTDRIVERNAME "\\\\.\\CSR0"

// Driver Handle
static HANDLE Driver_Handle;

static DWORD Error_Code;
static BOOL Thread_Exit;

// Thread Handle
static HANDLE Ind_Thread_Handle;


// Overlayed Reader/Writer Handle
static OVERLAPPED osReader = {0};
static OVERLAPPED osWriter = {0};


// Sync Event
static HANDLE Thread_Sync_Event;



int USB_HCIt_PDU_Tx(u_int8 *pdu, u_int8 len) 
{
	u_int8 pkt_head[5];
	u_int8 *buffer;
	DWORD Bytes_Written, Error_Code;

	/* merge packet into a single buffer for write call */
	buffer=(u_int8 *)malloc(sizeof(u_int8) * len);
	memcpy(buffer,(u_int8 *)pdu , len);

	/* write to the driver */
	ResetEvent(osWriter.hEvent);
		
	if(!DeviceIoControl(Driver_Handle,RTBTH_IOCTL_CODE_HCI_SEND_CMD,buffer, len, NULL , 0 , &Bytes_Written, &osWriter))
	{ 
		Error_Code=GetLastError();

		if (Error_Code==ERROR_IO_PENDING) 
		{
			if (!GetOverlappedResult(Driver_Handle, &osWriter, &Bytes_Written, TRUE)) 
			{
				Error_Code=GetLastError();  
			} 
			else 
			{
				Error_Code=0;
			}
		}			
	} 
	else 
	{
		Error_Code=0;
	}

	free(buffer);


	if (Error_Code!=0 ) 
	{
		return BT_HCITERROR;
	}

	return BT_NOERROR;
}


int SendCommand(u_int8 *data, u_int8 len)
{	
	return USB_HCIt_PDU_Tx(data, len); 	
}


DWORD WINAPI Ind_Thread(LPVOID Arg)
{
	UCHAR *Read_Buffer;
	DWORD  Bytes_Read;
	

	SetEvent(Thread_Sync_Event);  /* Notify the init function that the thread is running */
	do
	{
		/* allocate a buffer for the PDU */        
		if (!(Read_Buffer=(UCHAR *)malloc(sizeof(UCHAR) * READ_BUFFER_SIZE))) 
		{  
			return BT_NORESOURCES;
		}                             
		do 
		{
			ResetEvent(osReader.hEvent);
			if(!DeviceIoControl(Driver_Handle,RTBTH_IOCTL_CODE_GET_HCI_EVENT, NULL, 0, Read_Buffer, READ_BUFFER_SIZE,&Bytes_Read, &osReader ))
			{ 
				Error_Code=GetLastError();
				if (Error_Code==ERROR_IO_PENDING) 
				{
					if (!GetOverlappedResult(Driver_Handle, &osReader, &Bytes_Read, TRUE)) 
					{
						Error_Code=GetLastError();  
					} 
					else 
					{
						Error_Code=0;
					}
				}
			} 
			else 
			{
				Error_Code=0;
			}
		} while (Bytes_Read==0);
		
		if((Bytes_Read > 0) && (Bytes_Read < 256))
		{
			UCHAR buffer[64];
			
			//_snprintf(buffer,64, "[EVENT] Bytes_Read=%d =>", Bytes_Read);
			//OutputDebugString(buffer);
			
			//{	UCHAR uc;
			//	for(uc=0 ; uc < Bytes_Read ; uc++)
			//	{
			//		_snprintf(buffer, 64,">>> %02x ", (UCHAR)*(Read_Buffer+uc));
			//		OutputDebugString(buffer);
			//	}
			//}

			{
				CHAR pdu_buffer[512];
				CHAR pdu_print[512];
				UCHAR uc;

				//
				// Return 0x04
				//
				pdu_print[0]=pdu_buffer[0]=0x04;
				WriterGeneric(pdu_buffer, 1);

				//
				// Return EventCode and Length
				//
				pdu_print[1]= pdu_buffer[0]=(UCHAR)*(Read_Buffer);
				pdu_print[2]= pdu_buffer[1]=(UCHAR)*(Read_Buffer+1);
				WriterGeneric(pdu_buffer, 2);

				//
				// Return Remaining Bytes
				//
				for(uc=0 ; uc < Bytes_Read - 2 ; uc++)
				{
						pdu_print[3+uc]=pdu_buffer[uc]= (UCHAR)*(Read_Buffer + 2 + uc);
				}
				
				WriterGeneric(pdu_buffer, Bytes_Read - 2);

				{
					UCHAR pdu_name[128];
					UCHAR buffer[128];	

					MakePDUNameString(pdu_name, &pdu_print[1] , PDU_ARRIVE );
					_snprintf(buffer,128, "------> Send Event : << %s >> ", pdu_name);
					OutputDebugString(buffer);

					DBG_DumpBuffer(pdu_print, Bytes_Read + 1, 0);
					OutputDebugString("<------ \n");
				}
			}
		}
		free(Read_Buffer);

	}while (!Thread_Exit);

	
	return 0;
}



BOOL Driver_Open()
{
	char driverName[MAX_PATH];

	strncpy(driverName,USB_DEFAULTDRIVERNAME,sizeof(driverName));
	Driver_Handle=CreateFile(driverName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

	if (Driver_Handle==INVALID_HANDLE_VALUE) 
	{
		Error_Code=GetLastError();
		return FALSE;
	}  

	return TRUE;

} 


int HCI_Init() 
{
	DWORD Ind_Thread_ID;
	Driver_Handle=INVALID_HANDLE_VALUE;
	Ind_Thread_Handle=INVALID_HANDLE_VALUE;

	Thread_Exit=0;

	if (Driver_Open()) 
	{
		osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		osWriter.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
		Thread_Sync_Event=CreateEvent(NULL,TRUE,FALSE,NULL);	
		if (Thread_Sync_Event==INVALID_HANDLE_VALUE) 
		{
			Error_Code=GetLastError();
			return BT_NORESOURCES;
		}
		
		Ind_Thread_Handle=CreateThread(NULL,0,Ind_Thread,NULL,0,&Ind_Thread_ID);
		if (Ind_Thread_Handle==INVALID_HANDLE_VALUE) 
		{
			Error_Code=GetLastError();
			return BT_NORESOURCES;
		}
		
		WaitForSingleObject(Thread_Sync_Event,INFINITE);
		CloseHandle(Thread_Sync_Event);

		return BT_NOERROR;
	} 
	else 
	{
		return BT_HCITERROR;
	}	
}




int HCI_Shutdown(void) 
{
	Thread_Exit=TRUE;
	if (Driver_Handle!=INVALID_HANDLE_VALUE) 
	{
		CloseHandle(Driver_Handle);
	}
	Sleep(100); // let thread terminate grecefully if possible
	if (Ind_Thread_Handle!=INVALID_HANDLE_VALUE) 
	{
		TerminateThread(Ind_Thread_Handle,0);
		CloseHandle(Ind_Thread_Handle);
	}
	
	CloseHandle(osReader.hEvent);
	CloseHandle(osWriter.hEvent);
	
	
	return BT_NOERROR;
}


int test_main()
{
	UCHAR tmp,c;
	int status;
	unsigned char pdu[1024];
	
	status = HCI_Init();
	if(status == BT_HCITERROR)
	{
		goto exit;
	}
	do
	{
		printf("----------------------\n");
		printf("1 ... reset\n");
		printf("2 ... read RF\n");
		printf("3 ... write RF\n");
		printf("4 ... read BB\n");
		printf("5 ... write BB\n");
		printf("6 ... read MAC\n");
		printf("7 ... write MAC\n");
	
		scanf("%d", &c);
		

		switch(c)
		{
			case 1:
			{
				pdu[0]=0x03;
				pdu[1]=0x0C;
				pdu[2]=0x00;
				SendCommand(pdu, 3);
			}
			break;

			case 2: //SEND WRITE RF COMMAND
			{
				pdu[0]=0x31; //fixed
				pdu[1]=0xFC; //fixed
				pdu[2]=0x01; //fixed
				pdu[3]=0x42; //This reprents reg number
				SendCommand(pdu, 4);
			
			}
			break;

			case 3: //SEND READ RF COMMAND
			{
				pdu[0]=0x30; //fixed
				pdu[1]=0xFC; //fixed
				pdu[2]=0x02; //fixed
				pdu[3]=0x42; //INPUT, This represents reg number.
				pdu[4]=0x10; //INPUT, This represents reg value.

				SendCommand(pdu, 5);	
			
			}
			break;

			case 4: //SEND READ BBP COMMAND
			{
				pdu[0]=0x70; //fixed
				pdu[1]=0xFC; //fixed
				pdu[2]=0x01; //fixed
				pdu[3]=0x00; //INPUT, This represents reg number.
			
				SendCommand(pdu, 4);	
			}


			break;

			case 5://SEND WRITE BBP COMMAND
			{
				pdu[0]=0x71; //fixed
				pdu[1]=0xFC; //fixed
				pdu[2]=0x02; //fixed;
				pdu[3]=0x00; //INPUT, This presents reg number.
				pdu[4]=0x10; //INPUT, This presents reg value.

				SendCommand(pdu, 5);	
			}
			break;

			case 6:
			{
				pdu[0]=0x72; //fixed
				pdu[1]=0xFC; //fixed
				pdu[2]=0x04; //fixed
				pdu[3]=0x00; //INPUT, This represents reg number.
				pdu[4]=0x00; //fixed	
				pdu[5]=0x00; //fixed 
				pdu[6]=0x00; //fixed
			
				SendCommand(pdu, 7);
			}
			break;


			case 7:
			{
				pdu[0]=0x73; //fixed
				pdu[1]=0xFC; //fixed
				pdu[2]=0x08; //fixed;
				pdu[3]=0x00; //INPUT, This presents reg number.
				pdu[4]=0x00; //fixed
				pdu[5]=0x00; //fixed
				pdu[6]=0x00; //fixed
				pdu[7]=0x00; //INPUT, This present reg value
				pdu[8]=0x00; //fixed
				pdu[9]=0x00; //fixed
				pdu[10]=0x00;//fixed	

				SendCommand(pdu, 11);	
			}
			break;
		}
		
		

	}while(1);

exit:

	HCI_Shutdown();	

	return 0;
}
