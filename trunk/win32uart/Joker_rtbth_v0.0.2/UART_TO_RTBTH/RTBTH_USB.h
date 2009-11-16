#ifndef RTBTH_USB_H 
#define RTBTH_USB_H

#include <setupapi.h>
#include <initguid.h>


#define READ_BUFFER_SIZE 65540


typedef unsigned char u_int8;       /* unsigned integer,  8 bits long */
typedef unsigned short u_int16;     /* unsigned integer, 16 bits long */
typedef unsigned int u_int32;       /* unsigned integer, 32 bits long */

typedef unsigned int u_int;

typedef signed char s_int8;         /* unsigned integer,  8 bits long */
typedef signed short s_int16;       /* unsigned integer,  8 bits long */
typedef signed long s_int32;        /* unsigned integer,  8 bits long */

struct host_buf 
{

	u_int8 *data;
	u_int32 len;
	u_int32 totalsize;
	u_int8 *head;
	u_int8 *tail;

	struct host_buf *next;
	struct host_buf *parent;
	u_int16 child_count;
	u_int16 flags; 
	u_int16 hci_handle_flags;
	u_int16 hci_length;
	
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	I/O Control Code Switch Index Definition
//


#define RTBTH_IOCTL_INDEX  0x0000

#define RTBTH_IOCTL_CODE_HCI_SEND_CMD  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX, METHOD_BUFFERED, FILE_ANY_ACCESS )
    
#define RTBTH_IOCTL_CODE_HCI_SEND_CMD_LEGACY  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX, METHOD_OUT_DIRECT, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_GET_HCI_EVENT  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+1, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_GET_VERSION  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+2, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_GET_DRIVER_NAME  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+3, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_GET_CONFIG_DESCRIPTOR  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+4, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_GET_DEVICE_DESCRIPTOR  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+5, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_RESET_DEVICE  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+6, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_SEND_HCI_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+10, METHOD_BUFFERED, FILE_ANY_ACCESS )
    
#define RTBTH_IOCTL_CODE_SEND_HCI_DATA_LEGACY  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+10, METHOD_OUT_DIRECT, FILE_ANY_ACCESS )


#define RTBTH_IOCTL_CODE_RCV_HCI_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+11, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_BLOCK_HCI_EVENT  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+12, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_BLOCK_HCI_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+13, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_SEND_CONTROL_TRANSFER  \
    CTL_CODE( FILE_DEVICE_UNKNOWN,  RTBTH_IOCTL_INDEX+14, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_START_SCO_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+16, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_SEND_SCO_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+17, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_RCV_SCO_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+18, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_STOP_SCO_DATA  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+19, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_SET_EVENT_LENGTH  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+20, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_HCI_HID_SWITCH_COMMAND  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+21, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define RTBTH_IOCTL_CODE_RECV_SCO_DATA_MD  \
    CTL_CODE( FILE_DEVICE_UNKNOWN, RTBTH_IOCTL_INDEX+22, METHOD_OUT_DIRECT, FILE_ANY_ACCESS )

// BT STATUS CODE

#define BT_NOERROR			    0x00 
#define BT_PENDING			    0x01 
#define BT_INVALIDPARAM		    0x02 
#define BT_SECFAIL			    0x03 
#define BT_PINFAIL			    0x04 
#define BT_FLOWCTRL			    0x05
#define BT_NORESOURCES		    0x06 
#define BT_UNSUPPORTED		    0x07 
#define BT_HCITERROR		    0x08
#define BT_NOPRIV			    0x09
#define BT_HWERROR			    0x0A
#define BT_HOSTERROR		    0x0B
#define BT_UNKNOWNERROR		    0x0C
#define BT_HOSTINACTIVE		    0x0D
#define BT_RETRY			    0x0E
#define BT_AUTHORISATIONFAIL    0x0F
#define BT_AUTHENTICATIONFAIL   0x10
#define BT_ENCRYPTFAIL		    0x11
#define BT_TIMEOUT			    0x12
#define BT_PROTOERROR		    0x13
#define BT_DISALLOWED           0x14
#define BT_BUFTOOSMALL		    0x15
#define BT_DISCONNECTED         0x16

#define BT_INVALIDERRORCODE 0xFE 


int HCI_Init(VOID);
int HCI_Shutdown(VOID);
BOOL Driver_Open();
DWORD WINAPI Ind_Thread(LPVOID Arg);
int SendCommand(u_int8 *data, u_int8 len);
int USB_HCIt_PDU_Tx(u_int8 *pdu, u_int8 len);

#endif