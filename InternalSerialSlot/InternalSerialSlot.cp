// Internal Serial Enabler// Copyright (C) 2015  J. Bordens// Derrived from Eckhart K�ppen's Newton-Blunt-Support//         https://github.com/ekoeppen/Newton-Blunt-Support// This program is free software; you can redistribute it and/or modify// it under the terms of the GNU General Public License as published by// the Free Software Foundation; either version 2 of the License, or// (at your option) any later version.// This program is distributed in the hope that it will be useful,// but WITHOUT ANY WARRANTY; without even the implied warranty of// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the// GNU General Public License for more details.// You should have received a copy of the GNU General Public License along// with this program; if not, write to the Free Software Foundation, Inc.,// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.#include <NewtonGestalt.h>#include <stdio.h>#include <Objects.h>typedef long Ref;//const Ref	NILREF = 0x02;//const Ref	TRUEREF = 0x1A;//const Ref	FALSEREF = NILREF;	class TObjectIterator;class RefVar;typedef const RefVar& RefArg;typedef Ref (*MapSlotsFunction)(RefArg tag, RefArg value, ULong anything);extern Ref 	MakeInt(long i);extern Ref 	MakeChar(unsigned char c);extern Ref	MakeBoolean(int val);extern Boolean IsInt(RefArg r);	extern Boolean IsChar(RefArg r);	extern Boolean IsPtr(RefArg r);		extern Boolean IsMagicPtr(RefArg r);    extern Boolean IsRealPtr(RefArg r);   					extern long 	RefToInt(RefArg r);			extern UniChar RefToUniChar(RefArg r);		class TGPIOInterface{public:	NewtonErr ReadGPIODataD  (UByte, ULong *);	NewtonErr ReadGPIODataUS (UByte, ULong *);			NewtonErr WriteGPIODirD(UByte, UByte, UByte *);	NewtonErr WriteGPIODirUS(UByte, UByte, UByte *);			NewtonErr WriteGPIODataD(UByte, UByte, UByte *);	NewtonErr WriteGPIODataUS(UByte, UByte, UByte *);};class TBIOInterface{public:	NewtonErr ReadDIOPinsD (UByte pin, ULong *data);	NewtonErr ReadDIOPinsUS (UByte pin, ULong *data);	NewtonErr WriteDIODirD (UByte pin, UByte dir, UByte *data);	NewtonErr WriteDIODirUS (UByte pin, UByte dir, UByte *data);		NewtonErr WriteDIOPinsD (UByte pin, UByte value, UByte *data);	NewtonErr WriteDIOPinsUS (UByte pin, UByte value, UByte *data);};class TVoyagerPlatform{public:	char filler_0000[0x0010];		TGPIOInterface *fGPIOInterface;	TBIOInterface *fBIOInterface;		char filler_0014[0x00f0];	NewtonErr SerialPort0LineDriverConfigUS(UByte, UByte);		NewtonErr SerialPort3LineDriverConfigUS(UByte, UByte);	NewtonErr SerialPort0LineDriverConfigD(UByte, UByte);		NewtonErr SerialPort3LineDriverConfigD(UByte, UByte);	NewtonErr GetSubsystemPowerUS(ULong, ULong*);	NewtonErr GetSubsystemPowerD(ULong, ULong*);			NewtonErr SetSubsystemPowerUS(ULong, ULong);	NewtonErr SetSubsystemPowerD(ULong, ULong);	};extern TVoyagerPlatform *GetPlatformDriverD (void);extern TVoyagerPlatform *GetPlatformDriverUS (void);// Get the current value of the Serial Port 0 selector pinextern "C" Ref GetChannel0Selector (RefArg rcvr) {		TVoyagerPlatform *p;	ULong data = 0;	TUGestalt gestalt;	TGestaltSystemInfo info;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->fGPIOInterface->ReadGPIODataD (0x06, &data);	} else {		p = GetPlatformDriverUS ();		p->fGPIOInterface->ReadGPIODataUS (0x06, &data);	}	return MakeInt (data);}// Set the value of the Serial Port 0 selector pin// One parameter - 0=External Interconnect 1=Internal Serial Slotextern "C" Ref SetChannel0Selector (RefArg rcvr, RefArg value) {	TVoyagerPlatform *p;	TUGestalt gestalt;	TGestaltSystemInfo info;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->SerialPort0LineDriverConfigD( !RefToInt(value), 0x20 );	} else {		p = GetPlatformDriverUS ();		p->SerialPort0LineDriverConfigUS( !RefToInt(value), 0x20 );	}	return NILREF;}// Get the current value of the Serial Port 0 selector pinextern "C" Ref GetChannel3Selector (RefArg rcvr) {		TVoyagerPlatform *p;	ULong data = 0;	TUGestalt gestalt;	TGestaltSystemInfo info;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->fBIOInterface->ReadDIOPinsD(0x22, &data);	} else {		p = GetPlatformDriverUS ();		p->fBIOInterface->ReadDIOPinsUS(0x22, &data);	}		return MakeInt (data);}// Set the value of the Serial Port 3 selector pin// One parameter - 0=External Interconnect 1=Internal Serial Slotextern "C" Ref SetChannel3Selector (RefArg rcvr, RefArg value) {	TVoyagerPlatform *p;	TUGestalt gestalt;	TGestaltSystemInfo info;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->SerialPort3LineDriverConfigD( RefToInt(value), 0 );	} else {		p = GetPlatformDriverUS ();		p->SerialPort3LineDriverConfigUS( RefToInt(value), 0 );	}	return NILREF;}// Set the direction (input/output) of the GPIO pin// One parameter - 0=Input 1=Outputextern "C" Ref SetInternalSlotGPIODirection (RefArg rcvr, RefArg value) {	TVoyagerPlatform *p;	TUGestalt gestalt;	TGestaltSystemInfo info;	UByte data;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->fGPIOInterface->WriteGPIODirD (0x9, RefToInt(value), &data );	} else {		p = GetPlatformDriverUS ();		p->fGPIOInterface->WriteGPIODirUS (0x9,RefToInt(value), &data );	}	return NILREF;}// Get the current value of the GPIO pinextern "C" Ref GetInternalSlotGPIO (RefArg rcvr) {	TVoyagerPlatform *p;	TUGestalt gestalt;	TGestaltSystemInfo info;		ULong data = 0;			gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->fGPIOInterface->ReadGPIODataD (0x9, &data);	} else {		p = GetPlatformDriverUS ();		p->fGPIOInterface->ReadGPIODataUS (0x9, &data);	}	return MakeInt(data);}// Set the value of the GPIO pinextern "C" Ref SetInternalSlotGPIO (RefArg rcvr, RefArg value) {	TVoyagerPlatform *p;	TUGestalt gestalt;	TGestaltSystemInfo info;		UByte data = 0;			gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p = GetPlatformDriverD ();		p->fGPIOInterface->WriteGPIODataD (0x9, RefToInt(value), &data);	} else {		p = GetPlatformDriverUS ();		p->fGPIOInterface->WriteGPIODataUS (0x9, RefToInt(value), &data);	}	return NILREF;}extern "C" Ref GetSubsystemPower(RefArg rcvr, RefArg port) {	int subsystemIndex = RefToInt(port);	if (subsystemIndex != 1 && subsystemIndex != 3) return NILREF;	TVoyagerPlatform *p;		TUGestalt gestalt;	TGestaltSystemInfo info;			ULong power = 0;	NewtonErr err = 0;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p=GetPlatformDriverD();		err = p->GetSubsystemPowerD(subsystemIndex, &power);			} else{		p=GetPlatformDriverUS();		err = p->GetSubsystemPowerUS(subsystemIndex, &power);	}	if (err) return NILREF;	return MakeInt(power);}extern "C" Ref SetSubsystemPower(RefArg rcvr, RefArg port, RefArg power) {	int subsystemIndex = RefToInt(port);	if (subsystemIndex != 1 && subsystemIndex != 3) return NILREF;	TVoyagerPlatform *p;		TUGestalt gestalt;	TGestaltSystemInfo info;			ULong powerFlags = RefToInt(power);		if (powerFlags != powerFlags & 0xFF) return NILREF;		NewtonErr err = 0;		gestalt.Gestalt (kGestalt_SystemInfo, &info, sizeof (info));	if ((info.fROMStage & 0x30000) == 0x30000) {		p=GetPlatformDriverD();		err = p->SetSubsystemPowerD(subsystemIndex, powerFlags);			} else{		p=GetPlatformDriverUS();		err = p->SetSubsystemPowerUS(subsystemIndex, powerFlags);	}	return NILREF;}