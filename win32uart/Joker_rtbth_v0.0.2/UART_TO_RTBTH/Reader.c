/*-----------------------------------------------------------------------------

    This is a part of the Microsoft Source Code Samples. 
    Copyright (C) 1995 Microsoft Corporation.
    All rights reserved. 
    This source code is only intended as a supplement to 
    Microsoft Development Tools and/or WinHelp documentation.
    See these sources for detailed information regarding the 
    Microsoft samples programs.

    MODULE: Reader.c

    PURPOSE: Read from comm port

    FUNCTIONS:
        OutputABufferToWindow - process incoming data destined for tty window
        OutputABufferToFile   - process incoming data destined for a file
        OutputABuffer         - called when data is read from port

-----------------------------------------------------------------------------*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "MTTTY.h"

/*
    Prototypes for functions call only within this file
*/
void OutputABufferToFile( HANDLE, char *, DWORD );

void MakePDUNameString(UCHAR* name_string, UCHAR* pdu, UCHAR pdu_dir)
{
	ULONG pdu_code = pdu[0];
    switch(pdu_dir)
    {
		 case PDU_ARRIVE:
        switch(pdu_code)
        {
            case 0x01:
                strcpy_s(name_string, 128,  "HCI Inquiry Complete Event");
            break;
            case 0x02:
                strcpy_s(name_string, 128,  "HCI Inquiry Result Event");
            break;
            case 0x03:
                strcpy_s(name_string, 128,  "HCI Connection Complete Event");
            break;
            case 0x04:
                strcpy_s(name_string, 128,  "HCI Connection Request Event");
            break;
            case 0x05:
                strcpy_s(name_string, 128,  "HCI Disconnection Complete Event");
            break;
            case 0x06:
                strcpy_s(name_string, 128,  "HCI Authentication Complete Event");
            break;
            case 0x07:
                strcpy_s(name_string, 128,  "HCI Remote Name Request Complete Event");
            break;
            case 0x08:
                strcpy_s(name_string, 128,  "HCI Encryption Change Event");
            break;
            case 0x09:
                strcpy_s(name_string, 128,  "HCI Change Connection Link Key Complete Event");
            break;
            case 0x0A:
                strcpy_s(name_string, 128,  "HCI Master Link Key Complete Event");
            break;
            case 0x0B:
                strcpy_s(name_string, 128,  "HCI Read Remote Supported Features Complete Event");
            break;
            case 0x0C:
                strcpy_s(name_string, 128,  "HCI Read Remote Version Information Complete Event");
            break;
            case 0x0D:
                strcpy_s(name_string, 128,  "HCI QoS Setup Complete Event");
            break;
            case 0x0E:
                strcpy_s(name_string, 128,  "HCI Command Complete Event");
            break;
            case 0x0F:
                strcpy_s(name_string, 128,  "HCI Command Status Event");
            break;
            case 0x10:
                strcpy_s(name_string, 128,  "HCI Hardware Error Event");
            break;
            case 0x11:
                strcpy_s(name_string, 128,  "HCI Flush Occurred Event");
            break;
            case 0x12:
                strcpy_s(name_string, 128,  "HCI Role Change Event");
            break;
            case 0x13:
                strcpy_s(name_string, 128,  "HCI Number Of Completed Packets Event");
            break;
            case 0x14:
                strcpy_s(name_string, 128,  "HCI Mode Change Event");
            break;
            case 0x15:
                strcpy_s(name_string, 128,  "HCI Return Link Keys Event");
            break;
            case 0x16:
                strcpy_s(name_string, 128,  "HCI PIN Code Request Event");
            break;
            case 0x17:
                strcpy_s(name_string, 128,  "HCI Link Key Request Event");
            break;
            case 0x18:
                strcpy_s(name_string, 128,  "HCI Link Key Notification Event");
            break;
            case 0x19:
                strcpy_s(name_string, 128,  "HCI Loopback Command Event");
            break;
            case 0x1A:
                strcpy_s(name_string, 128,  "HCI Data Buffer Overflow Event");
            break;
            case 0x1B:
                strcpy_s(name_string, 128,  "HCI Max Slots Change Event");
            break;
            case 0x1C:
                strcpy_s(name_string, 128,  "HCI Read Clock Offset Complete Event");
            break;
            case 0x1D:
                strcpy_s(name_string, 128,  "HCI Connection Packet Type Changed Event");
            break;
            case 0x1E:
                strcpy_s(name_string, 128,  "HCI QoS Violation Event");
            break;
            case 0x1F:
                strcpy_s(name_string, 128,  "HCI Page Scan Mode Change Event");
            break;
            case 0x20:
                strcpy_s(name_string, 128,  "HCI Page Scan Repetition Mode Change Event");
            break;
            case 0x21:
                strcpy_s(name_string, 128,  "HCI Flow Specification Complete Event");
            break;
            case 0x22:
                strcpy_s(name_string, 128,  "HCI Inquiry Result with RSSI Event");
            break;
            case 0x23:
                strcpy_s(name_string, 128,  "HCI Read Remote Extended Features Complete Event");
            break;
            case 0x24:
                strcpy_s(name_string, 128,  "HCI Fixed Address Event");
            break;
            case 0x25:
                strcpy_s(name_string, 128,  "HCI Alias Address Event");
            break;
            case 0x26:
                strcpy_s(name_string, 128,  "HCI Generate Alias Request Event");
            break;
            case 0x27:
                strcpy_s(name_string, 128,  "HCI Active Address Event");
            break;
            case 0x28:
                strcpy_s(name_string, 128,  "HCI Allow Private Pairing Event");
            break;
            case 0x29:
                strcpy_s(name_string, 128,  "HCI Alias Address Request Event");
            break;
            case 0x2A:
                strcpy_s(name_string, 128,  "HCI Alias Not Recognised Event");
            break;
            case 0x2B:
                strcpy_s(name_string, 128,  "HCI Fixed Address Attempt Event");
            break;
            case 0x2C:
                strcpy_s(name_string, 128,  "HCI Synchonous Connection Complete Event");
            break;
            case 0x2D:
                strcpy_s(name_string, 128,  "HCI Synchonous Connection Changed Event");
            break;
            
			/* Ralink modify start, Tom, 2008/1/20 */
			case 0x38:
                 strcpy_s(name_string, 128,  "HCI Link Supervision Timeout Changed Event");
            break;
            /* Ralink modify end, Tom, 2008/1/20 */
            case 0xFF:
                switch(pdu[2])
                {
                case 0x00:
                    strcpy_s(name_string, 128,  "TCI Test Control Complete Event");
                    break;
                case 0x01:
                    strcpy_s(name_string, 128,  "TCI Read Pump Monitors Complete Event");
                    break;
                case 0x02:
                    strcpy_s(name_string, 128,  "TCI Read Master Slave Switch Clocks Event");
                    break;
                case 0x22:
                    strcpy_s(name_string, 128,  "TCI Trace LMP Message Complete Event");
                    break;
                default:
                    strcpy_s(name_string, 128,  "Unknown TCI Event");
                }
            break;
            default:
                strcpy_s(name_string, 128,  "Invalid HCI Event");
        }
    break;
		
		
	
		case PDU_EXIT:
        switch((pdu[1] >> 2) & 0x3F)
        {
			//#if 0
			case 0: // No-Op
				if(pdu[0] == 0)
					strcpy_s(name_string, 128, "No-op");
				else
					strcpy_s(name_string, 128,  "Invalid HCI Command");
			break;
            case 1: /* Link Control Commands */
                switch(pdu[0])
                {
                case 0x01:
                    strcpy_s(name_string, 128,  "HCI Inquiry");
                break;
                case 0x02:
                    strcpy_s(name_string, 128,  "HCI Inquiry Cancel");
                break;
                case 0x03:
                    strcpy_s(name_string, 128,  "HCI Periodic Inquiry Mode");
                break;
                case 0x04:
                    strcpy_s(name_string, 128,  "HCI Exit Periodic Inquiry Mode");
                break;
                case 0x05:
                    strcpy_s(name_string, 128,  "HCI Create Connection");
                break;
                case 0x06:
                    strcpy_s(name_string, 128,  "HCI Disconnect");
                break;
                case 0x07:
                    strcpy_s(name_string, 128,  "HCI Add SCO Connection");
                break;
                case 0x08:
                    strcpy_s(name_string, 128,  "HCI Create Connection Cancel");
                break;
                case 0x09:
                    strcpy_s(name_string, 128,  "HCI Accept Connection Request");
                break;
                case 0x0A:
                    strcpy_s(name_string, 128,  "HCI Reject Connection Request");
                break;
                case 0x0B:
                    strcpy_s(name_string, 128,  "HCI Link Key Request Reply");
                break;
                case 0x0C:
                    strcpy_s(name_string, 128,  "HCI Link Key Request Negative Reply");
                break;
                case 0x0D:
                    strcpy_s(name_string, 128,  "HCI PIN Code Request Reply");
                break;
                case 0x0E:
                    strcpy_s(name_string, 128,  "HCI PIN Code Request Negative Reply");
                break;
                case 0x0F:
                    strcpy_s(name_string, 128,  "HCI Change Connection Packet Type");
                break;

                case 0x11:
                    strcpy_s(name_string, 128,  "HCI Authentication Requested");
                break;

                case 0x13:
                    strcpy_s(name_string, 128,  "HCI Set Connection Encryption");
                break;

                case 0x15:
                    strcpy_s(name_string, 128,  "HCI Change Connection Link Key");
                break;

                case 0x17:
                    strcpy_s(name_string, 128,  "HCI Master Link Key");
                break;

                case 0x19:
                    strcpy_s(name_string, 128,  "HCI Remote Name Request");
                break;
                case 0x1A:
                    strcpy_s(name_string, 128,  "HCI Remote Name Request Cancel");
                break;
                case 0x1B:
                    strcpy_s(name_string, 128,  "HCI Read Remote Supported Features");
                break;
                case 0x1C:
                    strcpy_s(name_string, 128,  "HCI Read Remote Extended Features");
                break;
                case 0x1D:
                    strcpy_s(name_string, 128,  "HCI Read Remote Version Information");
                break;
                case 0x1F:
                    strcpy_s(name_string, 128,  "HCI Read Clock Offset");
                break;
                case 0x20:
                    strcpy_s(name_string, 128,  "HCI Read LMP Handle");
                break;
                case 0x21:
                    strcpy_s(name_string, 128,  "HCI Exchange Fixed Info");
                break;
                case 0x22:
                    strcpy_s(name_string, 128,  "HCI Exchange Alias Info");
                break;
                case 0x23:
                    strcpy_s(name_string, 128,  "HCI Private Pairing Request Reply");
                break;
                case 0x24:
                    strcpy_s(name_string, 128,  "HCI Private Pairing Request Negative Reply");
                break;
                case 0x25:
                    strcpy_s(name_string, 128,  "HCI Generated Alias");
                break;
                case 0x26:
                    strcpy_s(name_string, 128,  "HCI Alias Address Request Reply");
                break;
                case 0x27:
                    strcpy_s(name_string, 128,  "HCI Alias Address Request Negative Reply");
                break;
                case 0x28:
                    strcpy_s(name_string, 128,  "HCI Setup Synchronous Connection");
                break;
                case 0x29:
                    strcpy_s(name_string, 128,  "HCI Accept Synchronous Connection Request");
                break;
                case 0x2A:
                    strcpy_s(name_string, 128,  "HCI Reject Synchronous Connection Request");
                break;
                default:
                    strcpy_s(name_string, 128,  "Invalid Link Control Command");
                }
            break;
            case 2: /* Link Policy Commands */
                switch(pdu[0])
                {
                case 0x01:
                    strcpy_s(name_string, 128,  "HCI Hold Mode");
                break;
                case 0x03:
                    strcpy_s(name_string, 128,  "HCI Sniff Mode");
                break;
                case 0x04:
                    strcpy_s(name_string, 128,  "HCI Exit Sniff Mode");
                break;
                case 0x05:
                    strcpy_s(name_string, 128,  "HCI Park State");
                break;
                case 0x06:
                    strcpy_s(name_string, 128,  "HCI Exit Park State");
                break;
                case 0x07:
                    strcpy_s(name_string, 128,  "HCI QoS Setup");
                break;
                case 0x09:
                    strcpy_s(name_string, 128,  "HCI Role Discovery");
                break;
                case 0x0B:
                    strcpy_s(name_string, 128,  "HCI Switch Role");
                break;
                case 0x0C:
                    strcpy_s(name_string, 128,  "HCI Read Link Policy Settings");
                break;
                case 0x0D:
                    strcpy_s(name_string, 128,  "HCI Write Link Policy Settings");
                break;
                case 0x0E:
                    strcpy_s(name_string, 128,  "HCI Read Default Link Policy Settings");
                break;
                case 0x0F:
                    strcpy_s(name_string, 128,  "HCI Write Default Link Policy Settings");
                break;
                case 0x10:
                    strcpy_s(name_string, 128,  "HCI Flow Specification");
                break;
                default:
                    strcpy_s(name_string, 128,  "Invalid HCI Link Policy Command");
                }
            break;
            case 3: /* HC/BB Commands */
                switch(pdu[0])
                {
                case 0x01:
                    strcpy_s(name_string, 128,  "HCI Set Event Mask");
                    break;
                case 0x03:
                    strcpy_s(name_string, 128,  "HCI Reset");
                    break;
                case 0x05:
                    strcpy_s(name_string, 128,  "HCI Set Event Filter");
                    break;
                case 0x08:
                    strcpy_s(name_string, 128,  "HCI Flush");
                    break;
                case 0x09:
                    strcpy_s(name_string, 128,  "HCI Read PIN Type");
                    break;
                case 0x0A:
                    strcpy_s(name_string, 128,  "HCI Write PIN Type");
                    break;
                case 0x0B:
                    strcpy_s(name_string, 128,  "HCI Create New Unit Key");
                    break;
                case 0x0D:
                    strcpy_s(name_string, 128,  "HCI Read Stored Link Key");
                    break;
                case 0x11:
                    strcpy_s(name_string, 128,  "HCI Write Stored Link Key");
                    break;
                case 0x12:
                    strcpy_s(name_string, 128,  "HCI Delete Stored Link Key");
                    break;
                case 0x13:
                    strcpy_s(name_string, 128,  "HCI Write Local Name");
                    break;
                case 0x14:
                    strcpy_s(name_string, 128,  "HCI Read Local Name");
                break;
                case 0x15:
                    strcpy_s(name_string, 128,  "HCI Read Connection Accept Timeout");
                    break;
                case 0x16:
                    strcpy_s(name_string, 128,  "HCI Write Connection Accept Timeout");
                    break;
                case 0x17:
                    strcpy_s(name_string, 128,  "HCI Read Page Timeout");
                    break;
                case 0x18:
                    strcpy_s(name_string, 128,  "HCI Write Page Timeout");
                    break;
                case 0x19:
                    strcpy_s(name_string, 128,  "HCI Read Scan Enable");
                    break;
                case 0x1A:
                    strcpy_s(name_string, 128,  "HCI Write Scan Enable");
                    break;
                case 0x1B:
                    strcpy_s(name_string, 128,  "HCI Read Page Scan Activity");
                    break;
                case 0x1C:
                    strcpy_s(name_string, 128,  "HCI Write Page Scan Activity");
                    break;
                case 0x1D:
                    strcpy_s(name_string, 128,  "HCI Read Inquiry Scan Activity");
                    break;
                case 0x1E:
                    strcpy_s(name_string, 128,  "HCI Write Inquiry Scan Activity");
                    break;
                case 0x1F:
                    strcpy_s(name_string, 128,  "HCI Read Authentication Enable");
                    break;
                case 0x20:
                    strcpy_s(name_string, 128,  "HCI Write Authentication Enable");
                    break;
                case 0x21:
                    strcpy_s(name_string, 128,  "HCI Read Encryption Mode");
                    break;
                case 0x22:
                    strcpy_s(name_string, 128,  "HCI Write Encryption Mode");
                    break;
                case 0x23:
                    strcpy_s(name_string, 128,  "HCI Read Class of Device");
                    break;
                case 0x24:
                    strcpy_s(name_string, 128,  "HCI Write Class of Device");
                    break;
                case 0x25:
                    strcpy_s(name_string, 128,  "HCI Read Voice Setting");
                    break;
                case 0x26:
                    strcpy_s(name_string, 128,  "HCI Write Voice Setting");
                    break;
                case 0x27:
                    strcpy_s(name_string, 128,  "HCI Read Automatic Flush Timeout");
                    break;
                case 0x28:
                    strcpy_s(name_string, 128,  "HCI Write Automatic Flush Timeout");
                    break;
                case 0x29:
                    strcpy_s(name_string, 128,  "HCI Read Num Broadcast Retransmissions");
                    break;
                case 0x2A:
                    strcpy_s(name_string, 128,  "HCI Write Num Broadcast Retransmissions");
                    break;
                case 0x2B:
                    strcpy_s(name_string, 128,  "HCI Read Hold Mode Activity");
                    break;
                case 0x2C:
                    strcpy_s(name_string, 128,  "HCI Write Hold Mode Activity");
                    break;
                case 0x2D:
                    strcpy_s(name_string, 128,  "HCI Read Transmit Power Level");
                    break;
                case 0x2E:
                    strcpy_s(name_string, 128,  "HCI Read Synchronous Flow Control Enable");
                    break;
                case 0x2F:
                    strcpy_s(name_string, 128,  "HCI Write Synchronous Flow Control Enable");
                    break;
                case 0x31:
                    strcpy_s(name_string, 128,  "HCI Set Controller to Host Flow Control");
                    break;
                case 0x33:
                    strcpy_s(name_string, 128,  "HCI Host Buffer Size");
                    break;
                case 0x35:
                    strcpy_s(name_string, 128,  "HCI Host Number Of Completed Packets");
                    break;
                case 0x36:
                    strcpy_s(name_string, 128,  "HCI Read Link Supervision Timeout");
                    break;
                case 0x37:
                    strcpy_s(name_string, 128,  "HCI Write Link Supervision Timeout");
                    break;
                case 0x38:
                    strcpy_s(name_string, 128,  "HCI Read Number of Supported IAC");
                    break;
                case 0x39:
                    strcpy_s(name_string, 128,  "HCI Read Current IAC LAP");
                    break;
                case 0x3A:
                    strcpy_s(name_string, 128,  "HCI Write Current IAC LAP");
                    break;
                case 0x3B:
                    strcpy_s(name_string, 128,  "HCI Read Page Scan Period Mode");
                    break;
                case 0x3C:
                    strcpy_s(name_string, 128,  "HCI Write Page Scan Period Mode");
                    break;
                case 0x3D:
                    strcpy_s(name_string, 128,  "HCI Read Page Scan Mode");
                    break;
                case 0x3E:
                    strcpy_s(name_string, 128,  "HCI Write Page Scan Mode");
                    break;
                case 0x3F:
                    strcpy_s(name_string, 128,  "HCI Set AFH Host Channel Classification");
                    break;

                case 0x42:
                    strcpy_s(name_string, 128,  "HCI Read Inquiry Scan Type");
                    break;
                case 0x43:
                    strcpy_s(name_string, 128,  "HCI Write Inquiry Scan Type");
                    break;
                case 0x44:
                    strcpy_s(name_string, 128,  "HCI Read Inquiry Mode");
                    break;
                case 0x45:
                    strcpy_s(name_string, 128,  "HCI Write Inquiry Mode");
                    break;
                case 0x46:
                    strcpy_s(name_string, 128,  "HCI Read Page Scan Type");
                    break;
                case 0x47:
                    strcpy_s(name_string, 128,  "HCI Write Page Scan Type");
                    break;
                case 0x48:
                    strcpy_s(name_string, 128,  "HCI Read AFH Channel Assessment Mode");
                    break;
                case 0x49:
                    strcpy_s(name_string, 128,  "HCI Write AFH Channel Assessment Mode");
                    break;
                case 0x4A:
                    strcpy_s(name_string, 128,  "HCI Read Anonymity Mode");
                    break;
                case 0x4B:
                    strcpy_s(name_string, 128,  "HCI Write Anonymity Mode");
                    break;
                case 0x4C:
                    strcpy_s(name_string, 128,  "HCI Read Alias Authentication Enable");
                    break;
                case 0x4D:
                    strcpy_s(name_string, 128,  "HCI Write Alias Authentication Enable");
                    break;
                case 0x4E:
                    strcpy_s(name_string, 128,  "HCI Read Anonymous Address Change Parameters");
                    break;
                case 0x4F:
                    strcpy_s(name_string, 128,  "HCI Write Anonymous Address Change Parameters");
                    break;
                case 0x50:
                    strcpy_s(name_string, 128,  "HCI Reset Fixed Address Attempts Counter");
                    break;
                default:
                    strcpy_s(name_string, 128,  "Invalid HCI Host Controller & Baseband Command");
                }
            break;
            case 4: /* Informational Parameters */
                switch(pdu[0])
                {
                case 0x01:
                    strcpy_s(name_string, 128,  "HCI Read Local Version Information");
                break;
                case 0x02:
                    strcpy_s(name_string, 128,  "HCI Read Local Supported Commands");
                break;
                case 0x03:
                    strcpy_s(name_string, 128,  "HCI Read Local Supported Features");
                break;
                case 0x04:
                    strcpy_s(name_string, 128,  "HCI Read Local Extended Features");
                break;
                case 0x05:
                    strcpy_s(name_string, 128,  "HCI Read Buffer Size");
                break;
                case 0x07:
                    strcpy_s(name_string, 128,  "HCI Read Country Code");
                break;
                case 0x09:
                    strcpy_s(name_string, 128,  "HCI Read BD_ADDR");
                break;
                default:
                    strcpy_s(name_string, 128,  "Invalid HCI Informational Parameter");
                }
            break;
            case 5: /* Status Parameters */
                switch(pdu[0])
                {
                case 0x01:
                    strcpy_s(name_string, 128,  "HCI Read Failed Contact Counter");
                break;
                case 0x02:
                    strcpy_s(name_string, 128,  "HCI Reset Failed Contact Counter");
                break;
                case 0x03:
                    strcpy_s(name_string, 128,  "HCI Read Link Quality");
                break;
                case 0x05:
                    strcpy_s(name_string, 128,  "HCI Read RSSI");
                break;
                case 0x06:
                    strcpy_s(name_string, 128,  "HCI Read AFH Channel Map");
                break;
                case 0x07:
                    strcpy_s(name_string, 128,  "HCI Read Clock");
                break;
                default:
                    strcpy_s(name_string, 128,  "Invalid HCI Status Parameter Command");
                }
            break;

			case 6: /* Testing Commands */
                switch(pdu[0])
                {
                case 0x01:
                     strcpy_s(name_string, 128,   "HCI Read Loopback Mode");
                    break;
                case 0x02:
                     strcpy_s(name_string, 128,   "HCI Write Loopback Mode");
                    break;
                case 0x03:
                     strcpy_s(name_string, 128,   "HCI Enable Device Under Test Mode");
                    break;
                default:
                     strcpy_s(name_string, 128,   "Invalid HCI Testing Command");
                }
            break;
            case 0x3F: /* TCI Commands */
                switch(pdu[0])
                {
                case 0x02:
                 strcpy_s(name_string, 128,   "TCI_Activate_Remote_DUT");
                break;
                case 0x03:
                 strcpy_s(name_string, 128,   "TCI_Control_Remote_DUT");
                break;
                case 0x04:
                 strcpy_s(name_string, 128,   "TCI_Increase_Remote_Power");
                break;
                case 0x05:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Hop_Frequencies");
                break;
                case 0x06:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Hardware_Version");
                break;
                case 0x07:
                 strcpy_s(name_string, 128,   "TCI_Decrease_Remote_Power");
                break;
                case 0x08:
                 strcpy_s(name_string, 128,   "TCI_Increase_Local_Volume");
                break;
                case 0x09:
                 strcpy_s(name_string, 128,   "TCI_Decrease_Local_Volume");
                break;
                case 0x0A:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Native_Clock");
                break;
                case 0x0B:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Native_Clock");
                break;
                case 0x0C:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Relative_Mips");
                break;
                case 0x0D:
                 strcpy_s(name_string, 128,   "TCI_Type_Approval_Tester_Control");
                break;
                case 0x0E:
                 strcpy_s(name_string, 128,   "TCI_Increment_Local_Failed_Attempts_Counter");
                break;
                case 0x0F:
                 strcpy_s(name_string, 128,   "TCI_Clear_Local_Failed_Attempts_Counter");
                break;
                case 0x10:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Default_Packet_Type");
                break;
                case 0x11:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Default_Packet_Type");
                break;
                case 0x12:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_SyncWord");
                break;
                case 0x13:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Hopping_Mode");
                break;
                case 0x14:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Hopping_Mode");
                break;
                case 0x15:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Whitening_Enable");
                break;
                case 0x16:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Whitening_Enable");
                break;
                case 0x17:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Radio_Power");
                break;
                case 0x18:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Radio_Power");
                break;
                case 0x19:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Am_Addr");
                break;
                case 0x1A:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Device_Address");
                break;
                case 0x1B:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Link_Key_Type");
                break;
                case 0x1C:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Link_Key_Type");
                break;
                case 0x1D:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Extended_Features");
                break;
                case 0x1E:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Features");
                break;
                case 0x1F:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Extended_Features");
                break;
                case 0x2A:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Timing_Information");
                break;
                case 0x2B:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Timing_Information");
                break;
                case 0x2C:
                 strcpy_s(name_string, 128,   "TCI_Read_Remote_Timing_Information");
                break;
                case 0x2D:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Hardware_Register");
                break;
                case 0x2E:
                 strcpy_s(name_string, 128,   "TCI_Reset_Local_Baseband_Monitors");
                break;
                case 0x2F:
                 strcpy_s(name_string, 128,   "TCI_Update_Manufacturing_Information");
                break;
                case 0x30:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Radio_Register");
                break;
                case 0x31:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Radio_Register");
                break;
                case 0x32:
                 strcpy_s(name_string, 128,   "TCI_Change_Radio_Modulation");
                break;
                case 0x33:
                 strcpy_s(name_string, 128,   "TCI_Read_Radio_Modulation");
                break;
                case 0x34:
                 strcpy_s(name_string, 128,   "TCI_Write_UART_Baud_Rate");
                break;

                case 0x3A:
                 strcpy_s(name_string, 128,   "TCI_Reset_Local_Pump_Monitors");
                break;
                case 0x3B:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Pump_Monitors");
                break;
                case 0x3C:
                 strcpy_s(name_string, 128,   "TCI_Write_Local_Encryption_Key_Length");
                break;
                case 0x3D:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Encryption_Key_Length");
                break;
                case 0x3E:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Hop_Frequencies");
                break;
                case 0x3F:
                 strcpy_s(name_string, 128,   "TCI_Read_Local_Baseband_Monitors");
                break;

                case 0x40:
                 strcpy_s(name_string, 128,   "TCI_Disable_Low_Power_Mode");
                break;
                case 0x41:
                 strcpy_s(name_string, 128,   "TCI_Enable_Low_Power_Mode");
                break;
                case 0x42:
                 strcpy_s(name_string, 128,   "TCI_Read_Minimum_Search_Window");
                break;
                case 0x43:
                 strcpy_s(name_string, 128,   "TCI_Write_Minimum_Search_Window");
                break;
				case 0x44:
				 strcpy_s(name_string, 128,   "TCI_Disable_SCO_Repeater_Mode");
                break;
				case 0x45:
				 strcpy_s(name_string, 128,   "TCI_Enable_SCO_Repeater_Mode");
                break;
				case 0x46:
				 strcpy_s(name_string, 128,   "TCI_Write_eSCO_Retransmission_Mode");
                break;
				case 0x47:
				 strcpy_s(name_string, 128,   "TCI_Read_eSCO_Retransmission_Mode");
                break;

                case 0x50:
                 strcpy_s(name_string, 128,   "TCI_Write_Park_Parameters");
                break;
                case 0x51:
                 strcpy_s(name_string, 128,   "TCI_Read_Unused_Stack_Size");
                break;

                case 0x60:
                 strcpy_s(name_string, 128,   "TCI_Write_AFH_Control");
                break;
                case 0x61:
                 strcpy_s(name_string, 128,   "TCI_Read_Raw_RSSI");
                break;
                case 0x62:
                 strcpy_s(name_string, 128,   "TCI_Read_BER");
                break;
                case 0x63:
                 strcpy_s(name_string, 128,   "TCI_Read_PER");
                break;
                case 0x64:
                 strcpy_s(name_string, 128,   "TCI_Read_Raw_RSSI_PER_BER");
                break;
                default:
                 strcpy_s(name_string, 128,   "Unknown TCI Command");
                }
            break;
            default:
                 strcpy_s(name_string, 128,   "Invalid HCI OGF");

			
			//#endif	
        }
		break;

		  default:
        	strcpy_s(name_string, 128,   "Invalid HCI PDU");
    }
}


#if 0
void MakePDUNameString(CString* name_string, UCHAR* pdu, UCHAR pdu_dir)
{
    u_int32 pdu_code = pdu[0];
    switch(pdu_dir)
    {
    case PDU_ARRIVE:
        switch(pdu_code)
        {
            case 0x01:
                strcpy(name_string, "HCI Inquiry Complete Event";
            break;
            case 0x02:
                strcpy_s(name_string, 128,  "HCI Inquiry Result Event";
            break;
            case 0x03:
                *name_string += "HCI Connection Complete Event";
            break;
            case 0x04:
                *name_string += "HCI Connection Request Event";
            break;
            case 0x05:
                *name_string += "HCI Disconnection Complete Event";
            break;
            case 0x06:
                *name_string += "HCI Authentication Complete Event";
            break;
            case 0x07:
                *name_string += "HCI Remote Name Request Complete Event";
            break;
            case 0x08:
                *name_string += "HCI Encryption Change Event";
            break;
            case 0x09:
                *name_string += "HCI Change Connection Link Key Complete Event";
            break;
            case 0x0A:
                *name_string += "HCI Master Link Key Complete Event";
            break;
            case 0x0B:
                *name_string += "HCI Read Remote Supported Features Complete Event";
            break;
            case 0x0C:
                *name_string += "HCI Read Remote Version Information Complete Event";
            break;
            case 0x0D:
                *name_string += "HCI QoS Setup Complete Event";
            break;
            case 0x0E:
                *name_string += "HCI Command Complete Event";
            break;
            case 0x0F:
                *name_string += "HCI Command Status Event";
            break;
            case 0x10:
                *name_string += "HCI Hardware Error Event";
            break;
            case 0x11:
                *name_string += "HCI Flush Occurred Event";
            break;
            case 0x12:
                *name_string += "HCI Role Change Event";
            break;
            case 0x13:
                *name_string += "HCI Number Of Completed Packets Event";
            break;
            case 0x14:
                *name_string += "HCI Mode Change Event";
            break;
            case 0x15:
                *name_string += "HCI Return Link Keys Event";
            break;
            case 0x16:
                *name_string += "HCI PIN Code Request Event";
            break;
            case 0x17:
                *name_string += "HCI Link Key Request Event";
            break;
            case 0x18:
                *name_string += "HCI Link Key Notification Event";
            break;
            case 0x19:
                *name_string += "HCI Loopback Command Event";
            break;
            case 0x1A:
                *name_string += "HCI Data Buffer Overflow Event";
            break;
            case 0x1B:
                *name_string += "HCI Max Slots Change Event";
            break;
            case 0x1C:
                *name_string += "HCI Read Clock Offset Complete Event";
            break;
            case 0x1D:
                *name_string += "HCI Connection Packet Type Changed Event";
            break;
            case 0x1E:
                *name_string += "HCI QoS Violation Event";
            break;
            case 0x1F:
                *name_string += "HCI Page Scan Mode Change Event";
            break;
            case 0x20:
                *name_string += "HCI Page Scan Repetition Mode Change Event";
            break;
            case 0x21:
                *name_string += "HCI Flow Specification Complete Event";
            break;
            case 0x22:
                *name_string += "HCI Inquiry Result with RSSI Event";
            break;
            case 0x23:
                *name_string += "HCI Read Remote Extended Features Complete Event";
            break;
            case 0x24:
                *name_string += "HCI Fixed Address Event";
            break;
            case 0x25:
                *name_string += "HCI Alias Address Event";
            break;
            case 0x26:
                *name_string += "HCI Generate Alias Request Event";
            break;
            case 0x27:
                *name_string += "HCI Active Address Event";
            break;
            case 0x28:
                *name_string += "HCI Allow Private Pairing Event";
            break;
            case 0x29:
                *name_string += "HCI Alias Address Request Event";
            break;
            case 0x2A:
                *name_string += "HCI Alias Not Recognised Event";
            break;
            case 0x2B:
                *name_string += "HCI Fixed Address Attempt Event";
            break;
            case 0x2C:
                *name_string += "HCI Synchonous Connection Complete Event";
            break;
            case 0x2D:
                *name_string += "HCI Synchonous Connection Changed Event";
            break;
            
			/* Ralink modify start, Tom, 2008/1/20 */
			case 0x38:
                 *name_string += "HCI Link Supervision Timeout Changed Event";
            break;
            /* Ralink modify end, Tom, 2008/1/20 */
            case 0xFF:
                switch(pdu[2])
                {
                case 0x00:
                    *name_string += "TCI Test Control Complete Event";
                    break;
                case 0x01:
                    *name_string += "TCI Read Pump Monitors Complete Event";
                    break;
                case 0x02:
                    *name_string += "TCI Read Master Slave Switch Clocks Event";
                    break;
                case 0x22:
                    *name_string += "TCI Trace LMP Message Complete Event";
                    break;
                default:
                    *name_string += "Unknown TCI Event";
                }
            break;
            default:
                *name_string += "Invalid HCI Event";
        }
    break;
    case PDU_EXIT:
        switch((pdu[1] >> 2) & 0x3F)
        {
            case 0: // No-Op
                if(pdu[0] == 0)
                    *name_string += "No-op";
                else
                    *name_string += "Invalid HCI Command";
            break;
            case 1: /* Link Control Commands */
                switch(pdu[0])
                {
                case 0x01:
                    *name_string += "HCI Inquiry";
                break;
                case 0x02:
                    *name_string += "HCI Inquiry Cancel";
                break;
                case 0x03:
                    *name_string += "HCI Periodic Inquiry Mode";
                break;
                case 0x04:
                    *name_string += "HCI Exit Periodic Inquiry Mode";
                break;
                case 0x05:
                    *name_string += "HCI Create Connection";
                break;
                case 0x06:
                    *name_string += "HCI Disconnect";
                break;
                case 0x07:
                    *name_string += "HCI Add SCO Connection";
                break;
                case 0x08:
                    *name_string += "HCI Create Connection Cancel";
                break;
                case 0x09:
                    *name_string += "HCI Accept Connection Request";
                break;
                case 0x0A:
                    *name_string += "HCI Reject Connection Request";
                break;
                case 0x0B:
                    *name_string += "HCI Link Key Request Reply";
                break;
                case 0x0C:
                    *name_string += "HCI Link Key Request Negative Reply";
                break;
                case 0x0D:
                    *name_string += "HCI PIN Code Request Reply";
                break;
                case 0x0E:
                    *name_string += "HCI PIN Code Request Negative Reply";
                break;
                case 0x0F:
                    *name_string += "HCI Change Connection Packet Type";
                break;

                case 0x11:
                    *name_string += "HCI Authentication Requested";
                break;

                case 0x13:
                    *name_string += "HCI Set Connection Encryption";
                break;

                case 0x15:
                    *name_string += "HCI Change Connection Link Key";
                break;

                case 0x17:
                    *name_string += "HCI Master Link Key";
                break;

                case 0x19:
                    *name_string += "HCI Remote Name Request";
                break;
                case 0x1A:
                    *name_string += "HCI Remote Name Request Cancel";
                break;
                case 0x1B:
                    *name_string += "HCI Read Remote Supported Features";
                break;
                case 0x1C:
                    *name_string += "HCI Read Remote Extended Features";
                break;
                case 0x1D:
                    *name_string += "HCI Read Remote Version Information";
                break;
                case 0x1F:
                    *name_string += "HCI Read Clock Offset";
                break;
                case 0x20:
                    *name_string += "HCI Read LMP Handle";
                break;
                case 0x21:
                    *name_string += "HCI Exchange Fixed Info";
                break;
                case 0x22:
                    *name_string += "HCI Exchange Alias Info";
                break;
                case 0x23:
                    *name_string += "HCI Private Pairing Request Reply";
                break;
                case 0x24:
                    *name_string += "HCI Private Pairing Request Negative Reply";
                break;
                case 0x25:
                    *name_string += "HCI Generated Alias";
                break;
                case 0x26:
                    *name_string += "HCI Alias Address Request Reply";
                break;
                case 0x27:
                    *name_string += "HCI Alias Address Request Negative Reply";
                break;
                case 0x28:
                    *name_string += "HCI Setup Synchronous Connection";
                break;
                case 0x29:
                    *name_string += "HCI Accept Synchronous Connection Request";
                break;
                case 0x2A:
                    *name_string += "HCI Reject Synchronous Connection Request";
                break;
                default:
                    *name_string += "Invalid Link Control Command";
                }
            break;
            case 2: /* Link Policy Commands */
                switch(pdu[0])
                {
                case 0x01:
                    *name_string += "HCI Hold Mode";
                break;
                case 0x03:
                    *name_string += "HCI Sniff Mode";
                break;
                case 0x04:
                    *name_string += "HCI Exit Sniff Mode";
                break;
                case 0x05:
                    *name_string += "HCI Park State";
                break;
                case 0x06:
                    *name_string += "HCI Exit Park State";
                break;
                case 0x07:
                    *name_string += "HCI QoS Setup";
                break;
                case 0x09:
                    *name_string += "HCI Role Discovery";
                break;
                case 0x0B:
                    *name_string += "HCI Switch Role";
                break;
                case 0x0C:
                    *name_string += "HCI Read Link Policy Settings";
                break;
                case 0x0D:
                    *name_string += "HCI Write Link Policy Settings";
                break;
                case 0x0E:
                    *name_string += "HCI Read Default Link Policy Settings";
                break;
                case 0x0F:
                    *name_string += "HCI Write Default Link Policy Settings";
                break;
                case 0x10:
                    *name_string += "HCI Flow Specification";
                break;
                default:
                    *name_string += "Invalid HCI Link Policy Command";
                }
            break;
            case 3: /* HC/BB Commands */
                switch(pdu[0])
                {
                case 0x01:
                    *name_string += "HCI Set Event Mask";
                    break;
                case 0x03:
                    *name_string += "HCI Reset";
                    break;
                case 0x05:
                    *name_string += "HCI Set Event Filter";
                    break;
                case 0x08:
                    *name_string += "HCI Flush";
                    break;
                case 0x09:
                    *name_string += "HCI Read PIN Type";
                    break;
                case 0x0A:
                    *name_string += "HCI Write PIN Type";
                    break;
                case 0x0B:
                    *name_string += "HCI Create New Unit Key";
                    break;
                case 0x0D:
                    *name_string += "HCI Read Stored Link Key";
                    break;
                case 0x11:
                    *name_string += "HCI Write Stored Link Key";
                    break;
                case 0x12:
                    *name_string += "HCI Delete Stored Link Key";
                    break;
                case 0x13:
                    *name_string += "HCI Write Local Name";
                    break;
                case 0x14:
                    *name_string += "HCI Read Local Name";
                break;
                case 0x15:
                    *name_string += "HCI Read Connection Accept Timeout";
                    break;
                case 0x16:
                    *name_string += "HCI Write Connection Accept Timeout";
                    break;
                case 0x17:
                    *name_string += "HCI Read Page Timeout";
                    break;
                case 0x18:
                    *name_string += "HCI Write Page Timeout";
                    break;
                case 0x19:
                    *name_string += "HCI Read Scan Enable";
                    break;
                case 0x1A:
                    *name_string += "HCI Write Scan Enable";
                    break;
                case 0x1B:
                    *name_string += "HCI Read Page Scan Activity";
                    break;
                case 0x1C:
                    *name_string += "HCI Write Page Scan Activity";
                    break;
                case 0x1D:
                    *name_string += "HCI Read Inquiry Scan Activity";
                    break;
                case 0x1E:
                    *name_string += "HCI Write Inquiry Scan Activity";
                    break;
                case 0x1F:
                    *name_string += "HCI Read Authentication Enable";
                    break;
                case 0x20:
                    *name_string += "HCI Write Authentication Enable";
                    break;
                case 0x21:
                    *name_string += "HCI Read Encryption Mode";
                    break;
                case 0x22:
                    *name_string += "HCI Write Encryption Mode";
                    break;
                case 0x23:
                    *name_string += "HCI Read Class of Device";
                    break;
                case 0x24:
                    *name_string += "HCI Write Class of Device";
                    break;
                case 0x25:
                    *name_string += "HCI Read Voice Setting";
                    break;
                case 0x26:
                    *name_string += "HCI Write Voice Setting";
                    break;
                case 0x27:
                    *name_string += "HCI Read Automatic Flush Timeout";
                    break;
                case 0x28:
                    *name_string += "HCI Write Automatic Flush Timeout";
                    break;
                case 0x29:
                    *name_string += "HCI Read Num Broadcast Retransmissions";
                    break;
                case 0x2A:
                    *name_string += "HCI Write Num Broadcast Retransmissions";
                    break;
                case 0x2B:
                    *name_string += "HCI Read Hold Mode Activity";
                    break;
                case 0x2C:
                    *name_string += "HCI Write Hold Mode Activity";
                    break;
                case 0x2D:
                    *name_string += "HCI Read Transmit Power Level";
                    break;
                case 0x2E:
                    *name_string += "HCI Read Synchronous Flow Control Enable";
                    break;
                case 0x2F:
                    *name_string += "HCI Write Synchronous Flow Control Enable";
                    break;
                case 0x31:
                    *name_string += "HCI Set Controller to Host Flow Control";
                    break;
                case 0x33:
                    *name_string += "HCI Host Buffer Size";
                    break;
                case 0x35:
                    *name_string += "HCI Host Number Of Completed Packets";
                    break;
                case 0x36:
                    *name_string += "HCI Read Link Supervision Timeout";
                    break;
                case 0x37:
                    *name_string += "HCI Write Link Supervision Timeout";
                    break;
                case 0x38:
                    *name_string += "HCI Read Number of Supported IAC";
                    break;
                case 0x39:
                    *name_string += "HCI Read Current IAC LAP";
                    break;
                case 0x3A:
                    *name_string += "HCI Write Current IAC LAP";
                    break;
                case 0x3B:
                    *name_string += "HCI Read Page Scan Period Mode";
                    break;
                case 0x3C:
                    *name_string += "HCI Write Page Scan Period Mode";
                    break;
                case 0x3D:
                    *name_string += "HCI Read Page Scan Mode";
                    break;
                case 0x3E:
                    *name_string += "HCI Write Page Scan Mode";
                    break;
                case 0x3F:
                    *name_string += "HCI Set AFH Host Channel Classification";
                    break;

                case 0x42:
                    *name_string += "HCI Read Inquiry Scan Type";
                    break;
                case 0x43:
                    *name_string += "HCI Write Inquiry Scan Type";
                    break;
                case 0x44:
                    *name_string += "HCI Read Inquiry Mode";
                    break;
                case 0x45:
                    *name_string += "HCI Write Inquiry Mode";
                    break;
                case 0x46:
                    *name_string += "HCI Read Page Scan Type";
                    break;
                case 0x47:
                    *name_string += "HCI Write Page Scan Type";
                    break;
                case 0x48:
                    *name_string += "HCI Read AFH Channel Assessment Mode";
                    break;
                case 0x49:
                    *name_string += "HCI Write AFH Channel Assessment Mode";
                    break;
                case 0x4A:
                    *name_string += "HCI Read Anonymity Mode";
                    break;
                case 0x4B:
                    *name_string += "HCI Write Anonymity Mode";
                    break;
                case 0x4C:
                    *name_string += "HCI Read Alias Authentication Enable";
                    break;
                case 0x4D:
                    *name_string += "HCI Write Alias Authentication Enable";
                    break;
                case 0x4E:
                    *name_string += "HCI Read Anonymous Address Change Parameters";
                    break;
                case 0x4F:
                    *name_string += "HCI Write Anonymous Address Change Parameters";
                    break;
                case 0x50:
                    *name_string += "HCI Reset Fixed Address Attempts Counter";
                    break;
                default:
                    *name_string += "Invalid HCI Host Controller & Baseband Command";
                }
            break;
            case 4: /* Informational Parameters */
                switch(pdu[0])
                {
                case 0x01:
                    *name_string += "HCI Read Local Version Information";
                break;
                case 0x02:
                    *name_string += "HCI Read Local Supported Commands";
                break;
                case 0x03:
                    *name_string += "HCI Read Local Supported Features";
                break;
                case 0x04:
                    *name_string += "HCI Read Local Extended Features";
                break;
                case 0x05:
                    *name_string += "HCI Read Buffer Size";
                break;
                case 0x07:
                    *name_string += "HCI Read Country Code";
                break;
                case 0x09:
                    *name_string += "HCI Read BD_ADDR";
                break;
                default:
                    *name_string += "Invalid HCI Informational Parameter";
                }
            break;
            case 5: /* Status Parameters */
                switch(pdu[0])
                {
                case 0x01:
                    *name_string += "HCI Read Failed Contact Counter";
                break;
                case 0x02:
                    *name_string += "HCI Reset Failed Contact Counter";
                break;
                case 0x03:
                    *name_string += "HCI Read Link Quality";
                break;
                case 0x05:
                    *name_string += "HCI Read RSSI";
                break;
                case 0x06:
                    *name_string += "HCI Read AFH Channel Map";
                break;
                case 0x07:
                    *name_string += "HCI Read Clock";
                break;
                default:
                    *name_string += "Invalid HCI Status Parameter Command";
                }
            break;
            case 6: /* Testing Commands */
                switch(pdu[0])
                {
                case 0x01:
                    *name_string += "HCI Read Loopback Mode";
                    break;
                case 0x02:
                    *name_string += "HCI Write Loopback Mode";
                    break;
                case 0x03:
                    *name_string += "HCI Enable Device Under Test Mode";
                    break;
                default:
                    *name_string += "Invalid HCI Testing Command";
                }
            break;
            case 0x3F: /* TCI Commands */
                switch(pdu[0])
                {
                case 0x02:
                *name_string += "TCI_Activate_Remote_DUT";
                break;
                case 0x03:
                *name_string += "TCI_Control_Remote_DUT";
                break;
                case 0x04:
                *name_string += "TCI_Increase_Remote_Power";
                break;
                case 0x05:
                *name_string += "TCI_Write_Local_Hop_Frequencies";
                break;
                case 0x06:
                *name_string += "TCI_Read_Local_Hardware_Version";
                break;
                case 0x07:
                *name_string += "TCI_Decrease_Remote_Power";
                break;
                case 0x08:
                *name_string += "TCI_Increase_Local_Volume";
                break;
                case 0x09:
                *name_string += "TCI_Decrease_Local_Volume";
                break;
                case 0x0A:
                *name_string += "TCI_Write_Local_Native_Clock";
                break;
                case 0x0B:
                *name_string += "TCI_Read_Local_Native_Clock";
                break;
                case 0x0C:
                *name_string += "TCI_Read_Local_Relative_Mips";
                break;
                case 0x0D:
                *name_string += "TCI_Type_Approval_Tester_Control";
                break;
                case 0x0E:
                *name_string += "TCI_Increment_Local_Failed_Attempts_Counter";
                break;
                case 0x0F:
                *name_string += "TCI_Clear_Local_Failed_Attempts_Counter";
                break;
                case 0x10:
                *name_string += "TCI_Read_Local_Default_Packet_Type";
                break;
                case 0x11:
                *name_string += "TCI_Write_Local_Default_Packet_Type";
                break;
                case 0x12:
                *name_string += "TCI_Write_Local_SyncWord";
                break;
                case 0x13:
                *name_string += "TCI_Write_Local_Hopping_Mode";
                break;
                case 0x14:
                *name_string += "TCI_Read_Local_Hopping_Mode";
                break;
                case 0x15:
                *name_string += "TCI_Write_Local_Whitening_Enable";
                break;
                case 0x16:
                *name_string += "TCI_Read_Local_Whitening_Enable";
                break;
                case 0x17:
                *name_string += "TCI_Write_Local_Radio_Power";
                break;
                case 0x18:
                *name_string += "TCI_Read_Local_Radio_Power";
                break;
                case 0x19:
                *name_string += "TCI_Write_Local_Am_Addr";
                break;
                case 0x1A:
                *name_string += "TCI_Write_Local_Device_Address";
                break;
                case 0x1B:
                *name_string += "TCI_Write_Local_Link_Key_Type";
                break;
                case 0x1C:
                *name_string += "TCI_Read_Local_Link_Key_Type";
                break;
                case 0x1D:
                *name_string += "TCI_Read_Local_Extended_Features";
                break;
                case 0x1E:
                *name_string += "TCI_Write_Local_Features";
                break;
                case 0x1F:
                *name_string += "TCI_Write_Local_Extended_Features";
                break;
                case 0x2A:
                *name_string += "TCI_Read_Local_Timing_Information";
                break;
                case 0x2B:
                *name_string += "TCI_Write_Local_Timing_Information";
                break;
                case 0x2C:
                *name_string += "TCI_Read_Remote_Timing_Information";
                break;
                case 0x2D:
                *name_string += "TCI_Write_Local_Hardware_Register";
                break;
                case 0x2E:
                *name_string += "TCI_Reset_Local_Baseband_Monitors";
                break;
                case 0x2F:
                *name_string += "TCI_Update_Manufacturing_Information";
                break;
                case 0x30:
                *name_string += "TCI_Write_Local_Radio_Register";
                break;
                case 0x31:
                *name_string += "TCI_Read_Local_Radio_Register";
                break;
                case 0x32:
                *name_string += "TCI_Change_Radio_Modulation";
                break;
                case 0x33:
                *name_string += "TCI_Read_Radio_Modulation";
                break;
                case 0x34:
                *name_string += "TCI_Write_UART_Baud_Rate";
                break;

                case 0x3A:
                *name_string += "TCI_Reset_Local_Pump_Monitors";
                break;
                case 0x3B:
                *name_string += "TCI_Read_Local_Pump_Monitors";
                break;
                case 0x3C:
                *name_string += "TCI_Write_Local_Encryption_Key_Length";
                break;
                case 0x3D:
                *name_string += "TCI_Read_Local_Encryption_Key_Length";
                break;
                case 0x3E:
                *name_string += "TCI_Read_Local_Hop_Frequencies";
                break;
                case 0x3F:
                *name_string += "TCI_Read_Local_Baseband_Monitors";
                break;

                case 0x40:
                *name_string += "TCI_Disable_Low_Power_Mode";
                break;
                case 0x41:
                *name_string += "TCI_Enable_Low_Power_Mode";
                break;
                case 0x42:
                *name_string += "TCI_Read_Minimum_Search_Window";
                break;
                case 0x43:
                *name_string += "TCI_Write_Minimum_Search_Window";
                break;
				case 0x44:
				*name_string += "TCI_Disable_SCO_Repeater_Mode";
                break;
				case 0x45:
				*name_string += "TCI_Enable_SCO_Repeater_Mode";
                break;
				case 0x46:
				*name_string += "TCI_Write_eSCO_Retransmission_Mode";
                break;
				case 0x47:
				*name_string += "TCI_Read_eSCO_Retransmission_Mode";
                break;

                case 0x50:
                *name_string += "TCI_Write_Park_Parameters";
                break;
                case 0x51:
                *name_string += "TCI_Read_Unused_Stack_Size";
                break;

                case 0x60:
                *name_string += "TCI_Write_AFH_Control";
                break;
                case 0x61:
                *name_string += "TCI_Read_Raw_RSSI";
                break;
                case 0x62:
                *name_string += "TCI_Read_BER";
                break;
                case 0x63:
                *name_string += "TCI_Read_PER";
                break;
                case 0x64:
                *name_string += "TCI_Read_Raw_RSSI_PER_BER";
                break;
                default:
                *name_string += "Unknown TCI Command";
                }
            break;
            default:
                *name_string += "Invalid HCI OGF";
        }
    break;
    default:
        *name_string += "Invalid HCI PDU";
    }
}
#endif




VOID DBG_DumpBuffer(PVOID Buffer, LONG Length, ULONG Offset)
{
    PUCHAR  DataPtr = Buffer;
    LONG    Remain = Length;
    int     i;

    while(Buffer && (Remain > 0))
    {
        UCHAR   HexBuffer[60];
        UCHAR   AsciiBuffer[20];
		WCHAR  	dumpBuffer[80];
        PUCHAR  HexPtr   = HexBuffer;
        PUCHAR  AsciiPtr = AsciiBuffer;

        for(i=0; i<16; i++)
        {
            *HexPtr++ = (UCHAR)((i == 8) ? '-' : ' ');

            if(Remain-- > 0)
            {
                UCHAR   Value, HiDigit, LoDigit;

                Value = *DataPtr++;

                HiDigit = (UCHAR)(Value >> 4);
                LoDigit = (UCHAR)(Value & 0x0F);

                *HexPtr++ = (UCHAR)((HiDigit > 9) ? 
                            (HiDigit - 10 + 'A') : (HiDigit - 9 + '9'));

                *HexPtr++ = (UCHAR)((LoDigit > 9) ?
                            (LoDigit - 10 + 'A') : (LoDigit - 9 + '9'));

                *AsciiPtr++ = (UCHAR)(((Value > 32) && (Value < 128)) ? 
                              Value : '.');
            }
            else 
            {
// 
// No more data
//
                *HexPtr++ = ' ';
                *HexPtr++ = ' ';

                *AsciiPtr++ = ' ';
            }
        }
// 
// Terminate the line buffer strings 
//
        *HexPtr = *AsciiPtr = '\0';
		_snprintf(dumpBuffer, 80, "%04x: %s   %s\n", Offset, HexBuffer, AsciiBuffer);
        OutputDebugString(dumpBuffer);

// 
// Adjust offset for display of next line
//
        Offset += 16;
    }
}



/*-----------------------------------------------------------------------------

FUNCTION: OutputABufferToWindow(HWND, char *, DWORD)

PURPOSE: Updates TTY Buffer with characters just received.

PARAMETERS:
    hTTY     - handle to the TTY child window
    lpBuf    - address of data buffer
    dwBufLen - size of data buffer

HISTORY:   Date       Author      Comment
            5/ 8/91   BryanW      Wrote it
           10/27/95   AllenD      Modified for MTTTY Sample

-----------------------------------------------------------------------------*/
void OutputABufferToWindow(HWND hTTY, char * lpBuf, DWORD dwBufLen)
{
    RECT rect;

    /*
        update screen buffer with new buffer
        need to do a character by character check
        for special characters
    */
    int i;
	UCHAR tmp2[128];

	if((dwBufLen >= 1)&& ((UCHAR)*(lpBuf) == 0x01))
	{
		UCHAR pdu_name[128];
		UCHAR buffer[128];	
		
		MakePDUNameString(pdu_name, lpBuf+1, PDU_EXIT );
		_snprintf(buffer,128, "------> BT Command : << %s >> ", pdu_name);
		OutputDebugString(buffer);
		
		DBG_DumpBuffer(lpBuf, dwBufLen, 0);
		OutputDebugString("<------ \n");

		
		

		SendCommand(lpBuf+1, dwBufLen-1);


	}

	#if 0
    for ( i = 0 ; i < (int) dwBufLen; i++) {
        switch (lpBuf[ i ]) {
            case ASCII_BEL:                // BELL CHAR
                MessageBeep( 0 ) ;
                break ;

            case ASCII_BS:                 // Backspace CHAR
                if (COLUMN( TTYInfo ) > 0)
                   COLUMN( TTYInfo ) -- ;
                break ;

            case ASCII_CR:                 // Carriage Return
                COLUMN( TTYInfo ) = 0 ;
                if (!NEWLINE( TTYInfo ))
                    break;

                //
                // FALL THROUGH
                //

            case ASCII_LF:                 // Line Feed
                if (ROW( TTYInfo )++ == MAXROWS - 1)
                {
                    MoveMemory( (LPSTR) (SCREEN( TTYInfo )),
                                  (LPSTR) (SCREEN( TTYInfo ) + MAXCOLS),
                                  (MAXROWS - 1) * MAXCOLS ) ;
                    FillMemory((LPSTR) (SCREEN( TTYInfo ) + (MAXROWS - 1) * MAXCOLS),
                                  MAXCOLS,  ' ' ) ;
                    InvalidateRect( hTTY, NULL, FALSE ) ;
                    ROW( TTYInfo )-- ;
                }
                break ;

            default:                       // standard character
                //SCREENCHAR(TTYInfo, COLUMN(TTYInfo), ROW(TTYInfo)) = lpBuf[ i ];
                {
                	UCHAR local_char_idx=0;
					UCHAR tmp[128];
						
					sprintf(tmp, "%02x - \n\r", lpBuf[i]);
					OutputDebugString(tmp);
					
					while((local_char_idx++) <= 3 )
					{
						if(local_char_idx == 1)
						{
	    					SCREENCHAR(TTYInfo, COLUMN(TTYInfo), ROW(TTYInfo)) = tmp[0];
						}
						else if(local_char_idx == 2)
						{
							SCREENCHAR(TTYInfo, COLUMN(TTYInfo), ROW(TTYInfo)) = tmp[1];
						}
						else if(local_char_idx == 3)
						{
							SCREENCHAR(TTYInfo, COLUMN(TTYInfo), ROW(TTYInfo)) = ' ';
						}
					
		                rect.left = (COLUMN( TTYInfo ) * XCHAR( TTYInfo )) -
		                            XOFFSET( TTYInfo ) ;
		                rect.right = rect.left + XCHAR( TTYInfo ) ;
		                rect.top = (ROW( TTYInfo ) * YCHAR( TTYInfo )) -
		                           YOFFSET( TTYInfo ) ;
		                rect.bottom = rect.top + YCHAR( TTYInfo ) ;
		                InvalidateRect( hTTY, &rect, FALSE ) ;



		                // 
		                // Line wrap
		                //
		                if (COLUMN( TTYInfo ) < MAXCOLS-1 )
		                    COLUMN( TTYInfo )++ ;
		                else if (AUTOWRAP( TTYInfo ))
		                    OutputABufferToWindow(hTTY, "\r\n", 2 ) ;
					}
                }
				
				
				
                break;
        }
    }

    MoveTTYCursor(hTTY);
	#endif
	
    return;
}

/*-----------------------------------------------------------------------------

FUNCTION: OutputABufferToFile(HANDLE, char *, DWORD)

PURPOSE: Output a rec'd buffer to a file

PARAMETERS:
    hFile    - handle of file save data into
    lpBuf    - address of data buffer
    dwBufLen - size of data buffer

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
void OutputABufferToFile(HANDLE hFile, char * lpBuf, DWORD dwBufLen)
{
    DWORD dwWritten;

    //
    // place buffer into file, report any errors
    //
    if (!WriteFile(hFile, lpBuf, dwBufLen, &dwWritten, NULL))
        ErrorReporter("WriteFile in file capture");

    if (dwBufLen != dwWritten)
        ErrorReporter("WriteFile");
    
    //
    // update transfer progress bar
    //
    PostMessage(GetDlgItem(ghWndStatusDlg, IDC_TRANSFERPROGRESS), PBM_STEPIT, 0, 0);

    return;
}

/*-----------------------------------------------------------------------------

FUNCTION: OutputABuffer(HWND, char *, DWORD)

PURPOSE: Send a rec'd buffer to the approprate location

PARAMETERS:
    hTTY     - handle to the TTY child window
    lpBuf    - address of data buffer
    dwBufLen - size of data buffer

COMMENTS: If buffer is 0 length, then do nothing.

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
void OutputABuffer(HWND hTTY, char * lpBuf, DWORD dwBufLen)
{
    if (dwBufLen == 0) {
        OutputDebugString("NULL Buffer in OutputABuffer\n\r");
        return;
    }

    switch(gdwReceiveState)
    {
        case RECEIVE_TTY:
            OutputABufferToWindow(hTTY, lpBuf, dwBufLen);
            break;

        case RECEIVE_CAPTURED:
            OutputABufferToFile(ghFileCapture, lpBuf, dwBufLen);
            break;

        default:
            OutputDebugString("Unknown receive state!\n\r");
    }

    return;
}
