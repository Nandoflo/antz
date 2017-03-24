/* -----------------------------------------------------------------------------
*
*  nposc.cpp
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2016 by Shane Saxon - saxon@openantz.com
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named LICENSE.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */
/* 
*  This file provides a C-Wrapper for the C++ based oscpack library
*
* --------------------------------------------------------------------------- */

#include "nposc.h"		  //JJ-zz

#include "nposcpack.h"	  //C++ oscpack library adapter

#include "../../npctrl.h"       // diff from osx, lde
#include "../../os/npos.h"

#include "../npmouse.h"
#include "../gl/npcolor.h"

/*
TX
force feedback
lighting controls
sound output
remote antz clusters
peer-to-peer collaboration

RX
GPS track data
realtime sensors: temperature, position, pressure, eeg, 6-dof
commands
tracks
selection
applying to selection / all / current
flying the camera
syncing commands/data

cleanup nposcpack and move majority of functions over to nposc...
*/

// ---------------------------------------------------------------------------
// this only works for type tags 'i' 'f' 's' and 'ff'
void npInitOSC (void* dataRef )
{
	int i = 0;
	int err = 0;
	char msg[128];

	pData data = (pData) dataRef;

//	pNPconnect connect = NULL;
	pNPoscConn oscItem = NULL;									//zz debug

	//JJ - start any UDP listeners specified on command line
	// or in globals (CSV) file, was npStartListeners()					//zz-JJ
	for( i = 0; i < data->io.osc.count; i++ )		
	{
		oscItem = &data->io.osc.conns[i];

		err = npInitOscPackListener( oscItem, data );
		if( err )
			printf("npInitOSC Error!!!\n");
		else
		{
			nposBeginThread( npOscListenerThread, oscItem );

			sprintf( msg, "OSC listener port RX: %d  TX: %d", 
						oscItem->rxPort, oscItem->txPort );

			npPostMsg(msg, kNPmsgOSC, data );
		}
	}
}

// ---------------------------------------------------------------------------
//zz osc
/// @todo need to create thread sync buffers 
//zz currently only works for type tags 'i' 'f' 's' and 'ff'
void npOscRx (int oscID, const char* addr, const char* tag, void** arguments, void* dataRef )
{
	static float prevX = 0.5, prevY = 0.5;
	float deltaX = 0.0f, deltaY = 0.0f;

	float x = 0.0f, y = 0, z = 0;
	int xInt; // yInt, zInt;

	int index = 0;
	int argCount = 0;

	char msg[128];

	pNPgl gl = NULL;
	pData data = (pData) dataRef;
	pNPmapLink mapItem = NULL;
	
	pNPmouse mouse = &data->io.mouse;

	//note example is inefficient should first call
	mapItem = npMapAddressToMapTypeItem(addr, data);
	if (mapItem == NULL) return;
	argCount = strlen(tag);

//	printf(" oscID: %d\n",oscID);


	//emulate keyboard repeat through a up/down handler... OFF/ON
	//for now add cases...
	if (argCount == 1)
	{ switch (*tag) 
	  {
		case 'f' : x = *(float*)arguments[0]; xInt = (int)x; break;
		case 'i' : xInt = *(int*)arguments[0]; x = (float)xInt; break;
		case 's' : 
		//		strcpy( (char*)mapItem->mapPtr, (char*)arguments[index] ); 
			break;
	} }

	switch( mapItem->elementID )
	{	
		case kNPcmdTrigger :
			switch( mapItem->itemA )
			{	
				case kNPcmdSelectAll :
					if (x)
						npCtrlCommand(kNPcmdSelectAll, data);
					else
						npCtrlCommand(kNPcmdSelectNone, data);
					break;
				//HACKERY using the list ordering pairs...
				case kNPcmdXincrease : //kNPcmdXdecreaseOff
				case kNPcmdXdecrease : //kNPcmdXdecreaseOff
				case kNPcmdYincrease : //kNPcmdYincreaseOff
				case kNPcmdYdecrease : // continues...
				case kNPcmdZincrease : // ...
				case kNPcmdZdecrease : // .
				case kNPcmdRotateLeft :
				case kNPcmdRotateRight :
				case kNPcmdRotateUp :
				case kNPcmdRotateDown :
				case kNPcmdRotateCCW :
				case kNPcmdRotateCW :
				case kNPcmdZoomOn :
					if (x)
						npCtrlCommand(mapItem->itemA, data);
					else
						npCtrlCommand(mapItem->itemA + 1, data);
					break;				//hack using +1 for OFF
				//add key repeat precedure
				default : //only call if state x != 0
					if(x) npCtrlCommand(mapItem->itemA, data); break;
		  	}
			break;
		case kNPcmdSubsample :
		//	gl = data->io.gl[id];
			gl = &data->io.gl;// [mapItem->itemA];	//map address
			gl->subsample = 1 + (int)(x * x * 1023.0f);

			//bounds check in case input is not 0.0 to 1.0
			if (gl->subsample > 1024)
				gl->subsample = 1024;
			else if (gl->subsample < 1)
				gl->subsample = 1;

			sprintf(msg, "%s: %d", mapItem->elementA, data->io.gl.subsample);	
			npPostMsg (msg, kNPmsgCtrl, dataRef);

			break;
		default : break; //npCtrlCommand(mapItem->itemA, data); break;
	}

	if (argCount == 2)
	{
		x = *(float*)arguments[0]; 
		y = *(float*)arguments[1]; 

		deltaX = x - prevX;
		deltaY = y - prevY;

		prevX = x;
		prevY = y;

		sprintf(msg, "%s: %d", mapItem->elementA, data->io.gl.subsample);	
		npPostMsg (msg, kNPmsgCtrl, dataRef);

		//map XY to an RGB value and assign to background color
		data->io.clear = npHSLtoRGBA(x, 1.0f, y, 1.0f);

		if (mouse->camMode == kNPmouseModeNull)
		{
			mouse->camMode = kNPmouseModeDragXY;	//left button mode

			// "/3/toggle4" = true = mouse->buttonR
		//	if (mouse->buttonR)
		//		mouse->camMode = kNPmouseModeDragXZ;	//right button mode
		}
/*			
		if ( mouse->buttonL && !mouse->buttonR )	//left button
			mouse->camMode = kNPmouseModeDragXY;
		if ( mouse->buttonL && mouse->buttonR )		//both buttons
			mouse->camMode = kNPmouseModeDragXZ;
		if ( !mouse->buttonL && mouse->buttonR )	//right button
			mouse->camMode = kNPmouseModeDragXZ;
*/		
		x = 1.5f * x * (float)data->io.gl.width;
		y = 1.5f * y * (float)data->io.gl.height;
	
		// HACKERY to deal with not having an touchDown/Up event from TouchOSC
		// allows finger to pickup move over an keep sliding (as user expects)
		//zz debug, needs improvement!!!
		if (deltaX < -0.1f || deltaX > 0.1f || deltaY < -0.1f || deltaY > 0.15f )
		{
			mouse->previous.x = (int)x;
			mouse->previous.y = (int)y;
			mouse->delta.x = 0.0f;
			mouse->delta.y = 0.0f;
			mouse->cmDX = 0.0f;
			mouse->cmDY = 0.0f;
		}

		//calls mouse event handler, multiple calls ok, has accumulator sync
		//multiply screen dimensions by 3 to help with range
		npMouseMotion( (int)x, (int)y );

//		mouse->camMode = kNPmouseModeNull;

	//	npMouseEvent(button, state, x, y);
	}
	else
	{
		for (index=0; index < argCount; index++)
		  switch (*tag++) 
		  {
			case 'f' :
				*(float*)mapItem->mapPtr = *(float*)arguments[index]; 
				break;
			case 'i' : 
				*(int*)mapItem->mapPtr = *(int*)arguments[index]; 
				break;
			case 's' : 
				strcpy( (char*)mapItem->mapPtr, (char*)arguments[index] ); 
				break;
		  }
	}

	if(data->io.osc.msgMode)
	{
		sprintf (msg, "RX OSC : %s", addr);
		npPostMsg (msg, data->io.osc.msgMode, data);
	}
}

/*
void npTxOSC (int oscID, char* addr, char* tag, void** arguments, void* dataRef ) 
{
	cppTx (oscID, addr, tag, arguments, dataRef );
}
*/
//------------------------------------------------------------------------------
int npTestOSC(void* dataRef)
{
	*(float*)npMapAddressToMapTypeItem ("/1/fader1\0", dataRef)->mapPtr = 0.6f;
	*(float*)npMapAddressToMapTypeItem ("/1/fader2\0", dataRef)->mapPtr = 0.4f;
	*(float*)npMapAddressToMapTypeItem ("/1/fader3\0", dataRef)->mapPtr = 0.6f;

	return 1;
}

//------------------------------------------------------------------------------
pNPoscMsg npOscNextMsgRX( pNPoscConn conn );
pNPoscMsg npOscNextMsgRX( pNPoscConn conn )
{
	return NULL;
}

//------------------------------------------------------------------------------
pNPoscMsg npOscNextMsgTX( pNPoscConn conn );
pNPoscMsg npOscNextMsgTX( pNPoscConn conn )
{
	return NULL;
}

//------------------------------------------------------------------------------
void npOscAddMsgRX( pNPoscMsg oscMsg );
void npOscAddMsgRX( pNPoscMsg oscMsg )
{
	return;
}

//------------------------------------------------------------------------------
void npOscProcessMsgRX( pNPoscMsg oscMsg );
void npOscProcessMsgRX( pNPoscMsg oscMsg )
{
	return;
}

//------------------------------------------------------------------------------
void npOscProcessMsgTX( pNPoscMsg oscMsg );
void npOscProcessMsgTX( pNPoscMsg oscMsg )
{
	return;
}

/// Processes RX/TX messages for all connections in sync with the main loop
//------------------------------------------------------------------------------
void npUpdateOsc( void* dataRef )
{
	int i = 0;

	pData data = (pData) dataRef;
	pNPosc osc = &data->io.osc;
	pNPoscConn conn = NULL;
	pNPoscMsg oscMsg = NULL;

	return;
	/// Process the threaded RX listener messages from all connections
	for( i=0; i < osc->count; i++ )
	{
		conn = &osc->conns[i];	///< get the connection
		
		while( oscMsg = npOscNextMsgRX(conn) )
			npOscProcessMsgRX( oscMsg );
	}

	/// May want to add additional control processing here

	/// Process the TX buffers
	for( i=0; i < osc->count; i++ )
	{
		conn = &osc->conns[i];	///< get the connection
		
		while( oscMsg = npOscNextMsgTX(conn) )
			npOscProcessMsgTX( oscMsg );
	}
}

