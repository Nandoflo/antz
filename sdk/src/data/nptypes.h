/* -----------------------------------------------------------------------------
*
*  nptypes.h
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

#ifndef NPTYPES_H_
#define NPTYPES_H_

#define kNPappName "ANTz"		///< This application's name.
#define kNPvMajor 0				///< Major version, API significantly changed.
#define kNPvMinor 205			///< Minor version, new or enhanced features.
#define kNPvPatch 1				///< Patch version, bugs fixed.

#include "stdbool.h"
//#include "../io/npkey.h"			///< @todo zz move key codes to npkey.h


//! App framework OR use NP_API_C to build dynamic lib .dll or .ro
//----------------------------------------------------------------------------
// #define NP_APP_CONSOLE		///< console app works without drawing = no GL
#define NP_APP_FREEGLUT			///< freeglut app framework on MSW and linux
// #define NP_APP_APPLE_GLUT	///< Apple GLUT app framework for OSX
// #define NP_APP_SDL			///< SDL app on Linux, MSW, OSX, iOS & Android
// #define NP_APP_OFX			///< Openframeworks

/*! We support addons and (have future plans for) plugins
    Addons are compiled with the app, where as plugins are found at runtime.
    To activate an addon you uncomment its define statement.
    To de-activiate (reduces package size) simply comment out the macro.
//------------------------------------------------------------------------ */

// OpenGL friendly library addons
#define NP_ADDON_ASSIMP				///< 3D models load and save, non-KML
#define NP_ADDON_FREEIMAGE			///< load and save images, includes zlib
// #define NP_ADDON_SOIL			///< texture images load and save DDS
// #define NP_ADDON_FREETYPE		///< multi-language font support for GL
// #define NP_ADDON_LIBTESS			///< SGI based polygon tesselator
// #define NP_ADDON_KAZMATH			///< Matrix math, rotate, translate...
// #define NP_ADDON_GLEW			///< OpenGL extension manager
// #define NP_ADDON_OPENCL			///< OpenCL compute language

// Video hardware addons
// #define NP_ADDON_AMD				///< AMD SDK for low-latency video
// #define NP_ADDON_NVIDIA			///< nVidia SDK for low-latency video
// #define NP_ADDON_BLACKMAGIC		///< Blackmagic video IO hardware
// #define NP_ADDON_GSTREAMER		///< Video files and streaming
// #define NP_ADDON_VLC				///< Video files, streaming and IO
// http://gstreamer.freedesktop.org/data/doc/gstreamer/head/pwg/pwg.pdf

// VR addons
// #define NP_ADDON_EQUALIZER		///< Cluster Rendering for CAVE's
// #define NP_ADDON_VRPN			///< Virtual Reality Peripherial Network
// #define NP_ADDON_ZSPACE			///< Desktop VR
// #define NP_ADDON_VIVE			///< VR headset
// #define NP_ADDON_MLEAP			///< AR vaporware

// Network addons
#define NP_ADDON_CURL				///< network ftp, http, smtp, ssh, etc...
#define NP_ADDON_JANNSON			///< JSON parser
#define NP_ADDON_OSCPACK			///< OSC network communication
// define NP_ADDON_XML				///< XML parsing
// define NP_ADDON_SNMP				///< SNMP tree graph of network hardware

// Database addons
#define NP_ADDON_MYSQL				///< MySQL client database access
// #define NP_ADDON_MARIADB			///< MySQL compatible LGPL client
// #define NP_ADDON_POSTGRESQL		///< PostgreSQL DB access
// #define NP_ADDON_SQLITE			///< SQLite in memory DB

// Misc  addons
// #define NP_ADDON_DOT				///< GraphViz DOT graphing language
#define NP_ADDON_GITVIZ				///< GitViz requires CURL and JANNSON
// #define NP_ADDON_KISSFFT			///< FFT analysis for int and float
// #define NP_ADDON_MINIZ			///< ZIP file compression

// Python and C Plugins based on (GLib) GObjects 
// #define NP_ADDON_GLIB			///< Provides GObject data structures
// #define NP_ADDON_LIBPEAS			///< GObject based plugins engine

// Plugins
// #define NP_PLUGINS_PYTHON		///< Python scripting and plugins


//!> Additional type defines in the following addon headers
//-----------------------------------------------------------------------------
#ifdef NP_ADDON_GITVIZ
#include "npgitviztypes.h"			///< Types for GitViz, CURL and JSON
#endif

#ifdef NP_ADDON_OSCPACK
#include "../io/net/nposcpack.h"	///< @todo zz remove this dependency
#endif

#ifdef NP_ADDON_MYSQL
#include "npdbtypes.h"				///< Types for DB structs and MySQL
#endif

// possibly switch data structures to GTK.org and/or GObject

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

#define kNPpathLegacyCSV	"usr/csv"		///< legacy CSV path
#define kNPpathLegacyImages	"usr/images"	///< legacy images path

/// 'usr' is the default root path for all data, ie 'usr/global/images'
#define kNPpathUser			"usr"			///< user data path

#define kNPpathGlobal		"global"		///< global path, used at startup
#define kNPpathPreset		"preset"		///< preset path

/// Dataset folder set
#define kNPpathCSV			"csv"			///< CSV table path
#define kNPpathImages		"images"		///< Texture images path
#define kNPpathModels		"models"		///< 3D models path
#define kNPpathPlugins		"plugin"		///< plugin path
#define kNPpathAudio		"audio"			///< audio path
#define kNPpathVideo		"video"			///< video path

#define kNPglMipmaps		true			///< Mipmaps require OpenGL 3.0+
#define kNPpaletteMax		4096			///< max number of color palettes
#define kNPtexListMax		100				///< max texture maps

#define kNPkeyMapSize		256				///< keyboard map
#define kNPkeyEventTypeSize 8				///< SHIFT, CTRL and ALT combos
											///  134217728 = 128MB, 
#define kNPmapFileBufferMax	67108864		///< 512MB = 536870912		//zzhp
#define kNPfileBlockSize	8388608			///< 8MB buffer is fairly optimal, can run AJA drive test to fine tune.

#define	kNPnodeMax			4002200			///< 4194304 16MB with set of 32bit ptr, 16MB if 64bit	//zzhp
#define kNPnodeRootMax		kNPnodeMax		///< 1048576 4MB with 32bit ptr, 8MB if 64bit	//zzhp 262144
#define kNPnodeChildArrayMin 4				///< Initial child array size allocated
											///< C99 max fixed array size is 16383
											///< 266 fills a sphere at 15 deg

#define kNPbranchLevelMax	28				///< max branch depth for child nodes, zz debug add error handling
											///< limit of GL matrix stack max 32

#define kNPcameraCount		4				///< default number of camera views

#define kNPfacetCube		6				///< cube facet count

#define kNPcubeLength		1.4142136f		///< cube edge length is sqrt(2)
#define kNPsphereRadius		1.0f			///< default radius of a sphere
#define kNPcylinderRadius	1.0f			///< default radius of a cylinder
#define kNPtorusRadius		1.5f			///< default radius of torus (center of tube)
#define kNPdefaultRatio		0.1f			///< default radius of torus tube

#define kNPoffsetUnit		1.0f			///< default surface offset
#define kNPoffsetCube		0.7071068f		///< (0.5f * kNPcubeLength)	// cube center height
#define kNPoffsetTorus		0.75f			///< (0.5f * kNPtorusRadius) // half torus radius
#define kNPoffsetPin		5.0f			///< cone point to center of dome
#define kNPoffsetRod		10.0f			///< rod length

#define	kNPscaleDefault		0.5f
#define	kNPscaleRod			1.0f			///< parent rod, child not scaled
#define	kNPscalePin			1.0f			///< parent pin, non-pin child
#define	kNPscalePinPin		0.5f			///< parent and child pin scale 1/2
#define kNPscaleCube		0.5f			///< parent cube, child is 1/3
#define kNPscaleSphere		0.25f			///< parent sphere, child is 1/4
#define kNPscaleCylinder	0.25f			///< parent cylinder, child is 1/4
#define	kNPscaleTorus		0.25f			///< parent torus, child is 1/4
#define	kNPscaleTorusTorus	0.5f			///< parent and child torus scale 1/2

#define kNPgridSpacing		30.0f			///< defualt grid spacing, was 10.0
#define kNPgridSegmentsX	12				///< default grid 16x16, was 16
#define kNPgridSegmentsY	6				///< default grid 16x16, was 16

#define kNPpaletteSize		20				///< index mapped color palette

#define kNPmaxName			266				///< name portion of the filePath
#define	kNPmaxPath			4096			///< max file path, msw supports 260
#define	kNPurlMax			4096			///< maximum length of URL accepted

#define kNPdataTypeMax		4096			///< used for CSV header field names
#define kNPbaseItemCount	18
#define kNPnodeItemCount	52
#define kNPcameraItemCount	17
#define kNPgridItemCount	5
#define kNPpinItemCount		12				///< important to keep these up-to-date

#define kNPtagItemCount		5				///< debug db //zzsql
#define kNPChMapItemCount	7				///< debug db //zzsql

#define kNPmenuPerPageMax		20
#define kNPconsoleLineMax		40			///< default is 80x40 character layout
#define kNPconsoleCharPerLine	80
#define kNPconsoleHistoryMax	800			///< max entries to store in RAM
#define kNPconsoleInputMax		4096		///< max user input string
#define kNPmsgQueMax			250			///< max messages stored in que
#define kNPmsgLengthMax			255			///< max message length
#define kNPmsgCmdRate			5			///< message command rate flow control
											
											//! link to journal article LENGTH statistics for title and abstract
											//! http:///<www.plosone.org/article/info%3Adoi%2F10.1371%2Fjournal.pone.0049476
#define kNPtagTitleMax			333			///< tag title max length
#define kNPtagDescMax			8			///< tag description max length 	//zz hpc // 4096
#define	kNPtagMax				2097128 	///< max number of record Tags
#define	kNPtagDrawMax			16383		///< max number tags allowed to draw

#define kNPlinkQueMax			kNPnodeMax	///< max in draw que, not total nodes

#define kNPsingleClickTime		0.45		///<max duration considered to be a click
#define kNPdoubleClickTime		0.45		///<max duration for double click 

#define kNPinputStrMax			4096		///<console max length user input

#define kNPmaxTrackToAttributeMappings 50000 ///<JJ maximum number records in chmap file, of mappings supported (fixed size array)
#define kNPattributeNameOffset	3			///<JJ attribute name column in the ChMap file:  id, ch, track, attribute name, id1, id2, id3
#define kNPchannelOffset		1			///<JJ channel offset column in the ChMap file:   0   1    2         3           4    5    6
#define kNPtrackOffset			2			///<JJ track offset column in the ChMap file:     0   1    2         3           4    5    6

#define kNPchannelMax			128			///< max number of channels
#define kNPtrackBufferSize		65535		///< JJ number of samples possible in the track buffer, NOTE: this is a circular
											///< buffer, the maximum size available to antz at any moment
#define kNPmaxTracks			1000		///< JJ maximum number of tracks supported on a channel
#define kNPmaxFloatProperties	500000		///< JJ maximum number of float properties fed by channel tracks
#define kNPmaxUcharProperties	500000		///< JJ maximum number of unsigned char properties fed by channel tracks
#define kNPmaxLineLength		10000		///< 
#define kNPmaxTokens			1000		///< 
#define kNPmaxPropertiesMapped	500000		///< JJ maximum number of node properties (one set for each data type) fed by channel tracks

#define kNPwindowWidth			800			///< Default window size
#define kNPwindowHeight			510
#define kNPwindowPositionX		40			///< Default window position
#define kNPwindowPositionY		40

#define kNPscreenshotAlpha		false		///< Save screenshot with alpha
#define kNPthumbAlpha			true		///< Save thumbnail with alpha
#define kNPthumbWidth			480			///< thumbnail width
#define kNPthumbHeight			270			///< thumbnail height	

#define kNPhideLevelMax			4			///< Hide nodes upper branch level

#define kNPgitvizTableID		42424242	///< gitviz table_id hack

//------------------------------------------------------------------------------
//! Base Types - designed to be directly compatible with OpenGL

/** Base node type structure that is used for our [Scene Graph](
   http://en.wikipedia.org/wiki/Scene_graph ). <br>
   Global Tree Graph used to store all of the app data structers.
   Multiple instances supports independent user environments and datasets.
   A caveat of this approach is that the app can draw its own structure.
*/

/*! @todo Implement hybrid data structure for all nodes including globals. <br>
*	So that global data structure elements have corresponing (core) graph nodes.
*/
struct NPnode2 {
	long long int	id;			///< node id
	
	int				type;		///< node type defines the data pointer type

	void*			parent;		///< parent node
	void*			next;		///< next sibling, the last sibling is null.
	void*			prev;		///< previous sibling, first sibling null.
	void*			child;		///< first child

	void*			route;		/*!< child branch route last travelled.
									  route is used for user navigation.	*/
	void*			data;		///< node data specific to node type

	int size;		///< size in bytes of this node structure and its data
};
typedef struct NPnode2 NPnode2;
typedef struct NPnode2 *pNPnode2;


struct NPfloatXY {
	float x,y;
};
typedef struct NPfloatXY NPfloatXY;
typedef struct NPfloatXY *pNPfloatXY;

struct NPfloatXYZ {
	float x,y,z;
};
typedef struct NPfloatXYZ NPfloatXYZ;
typedef struct NPfloatXYZ * pNPfloatXYZ;

struct NPfloatXYZA {
	float x,y,z,angle;
};
typedef struct NPfloatXYZA NPfloatXYZA;
typedef struct NPfloatXYZA *pNPfloatXYZA;

struct NPfloatXYZS {
	float x,y,z,scaler;
};
typedef struct NPfloatXYZS NPfloatXYZS;
typedef struct NPfloatXYZS *pNPfloatXYZS;

struct NPfloatRGB {
	float r,g,b;
};
typedef struct NPfloatRGB NPfloatRGB;
typedef struct NPfloatRGB *pNPfloatRGB;

struct NPfloatRGBA {
	float r,g,b,a;
};
typedef struct NPfloatRGBA NPfloatRGBA;
typedef struct NPfloatRGBA *pNPfloatRGBA;

struct NPintXY {
	int x,y;
};
typedef struct NPintXY NPintXY;
typedef struct NPintXY *pNPintXY;

struct NPintXYZ {
	int x,y,z;
};
typedef struct NPintXYZ NPintXYZ;
typedef struct NPintXYZ *pNPintXYZ;

struct NPintXYZS {
	int x,y,z,s;
};
typedef struct NPintXYZS NPintXYZS;
typedef struct NPintXYZS *pNPintXYZS;

typedef unsigned char NPubyte;

struct NPubyteRGBA {
	unsigned char r,g,b,a;
};
typedef struct NPubyteRGBA NPubyteRGBA;
typedef struct NPubyteRGBA *pNPubyteRGBA;

struct NPboolXYZ {
	bool x,y,z;
};
typedef struct NPboolXYZ NPboolXYZ;
typedef struct NPboolXYZ *pNPboolXYZ;

struct NPboolXYZS {
	bool x,y,z,s;
};
typedef struct NPboolXYZS NPboolXYZS;
typedef struct NPboolXYZS *pNPboolXYZS;

typedef void (*NPfuncPtr)(int menuItem, void* dataRef);
///<------------------------------------------------------------------------------
///< Custom Types - multiple instances -------------------------------------------

typedef void (*voidFnPtr)(void*);	//zz-JJ


struct NPmapType
{
	int		id;
	int		type;
	char*	name;
	char*	desc;
};
typedef struct NPmapType NPmapType;
typedef struct NPmapType *pNPmapType;


//zz-JJ
//zz-osc debug, dynamic would be better, but for now fixed size limit
#define kNPnameMax 261
#define kNPelementMax 64

///<approx 3KB record
//zz may allow mapPtr to also be a relative pointer offset... or something like that
///<idea is to support dynamically creating a new C-struct and schema mapping
struct NPmapLink
{
	///< C-struct mapPtr to the data in memory with ID and type mapping
	void*	mapPtr;	 ///<element ptr		///<pointer to an element of the C data map structure	
///<	int		idA;
	int		typeA;
	int		elementID;	///<typeA		///<mapPtr = npGetMapID(mapID); ///<and vice versa

///<	void*	mapPtrB;		///<element ptr	///<add this for channel ptr-pair updates? or separate list...	
///<	int		idListB;		///<id lists with range {22, 23, 44, [55:65]}
///<	int		typeB;			///<element base type
///<	void**	elementListB;	///<element type, could be a pointer offset, but a list is faster to process

///<	int		typeA;			///<base data type ///<mapPtr (c-struct) data type
	/// human-readable Descriptor 

	int		permissions;		///< input output direction +1 = A->B, 0 = A-B, -1 = B->A   ///<

	char*	mapPathA;//[kNPnameMax];		///<db table path or RAM ptr by name
	int		itemA;							///<itemID = np_map_row_id = array[item]
	char*	elementA;//[kNPelementMax];		///<name of the sub-member object
	char*	typeTagA;//[kNPelementMax];

	///< external schema mapping and formatting for OSC, CSV, JSON, MySQL...
	char*	mapPathB;//[kNPnameMax];		///<formatID by name, 'osc_mrmr'
	int		itemB;						///<item ID, array index, osc_id
	char*	elementB;//[kNPelementMax];	///<OSC address, URL, table field name
	char*	typeTagB;//[kNPelementMax];			///<OSC type tag or data type

	char*	name;//[kNPnameMax];
	char*	desc;//[kNPelementMax];

	char*	value;//[kNPelementMax];
	};
typedef struct NPmapLink NPmapLink;
typedef struct NPmapLink *pNPmapLink;

///< approx 3KB human-readable Descriptor Record
///< zz may allow mapPtr to also be a relative pointer offset... or something like that
///< idea is to support dynamically creating a new C-struct and schema mapping
///< auto-convert typeID <-> OSC Type Tag style, and other 3rd party tag types
///< unknown incoming auto-create new type tag record and refer to it
struct NPmapPair
{
	///< C-struct mapPtr to the data in memory with ID and type mapping
	void*	mapPtrA;		///<pointer to internal map element c-struct

	char	mapPathA[kNPelementMax];	///<mapPtr = npGetMapID(mapID); ///<and vice versa
	int		itemA;						///<item ID = np_map_row_id = array[item]
	char	elementA[kNPelementMax];	///<element and type are associated
	int		typeA;						///<mapPtr type <-auto-> OSC typeTag

	///< external schema mapping and formatting for OSC, CSV, JSON, MySQL...
	char	mapPathB[kNPnameMax];		///<db table path or RAM ptr by name  ///<formatID by name, 'osc_mrmr'
	int		itemB;						///<item ID, array index, osc_id
	char	elementB[kNPelementMax];	///<name of the sub-member object ///<OSC address, URL, table field name
	char	typeTagB[kNPnameMax];		///<OSC type tags can be converted to type_id

///<	char*	value;						///<data contents as str or binary bit stream

	///<char	title[kNPnameMax]			///<human readable text displayed in UI
	///<char	desc[kNPnameMax];			///<desc, notes, aux data... no size limit

	///<void** valueList;					///<each base type component
	};
typedef struct NPmapPair NPmapPair;
typedef struct NPmapPair *pNPmapPair;

struct NPrecordSet						///<generic list struct
{
	void**	list;
	int		count;
	int		type;

	int		size;
};
typedef struct NPrecordSet NPrecordSet;
typedef struct NPrecordSet* pNPrecordSet;


/// zz osc
/*
struct NPoscConn {
	int		id;

	char	*txURL;				///< DNS lookkup of IP
	char	*rxURL;

	char	txIP[24];			///<supports ipv4 ipv6 or URL
	char	rxIP[24];

	int		txPort;
	int		rxPort;

	int		size;
};
typedef struct NPoscConn NPoscConn;
typedef struct NPoscConn* pNPoscConn;
*/
struct NPoscMsg {
	int		id;

	char	msg[kNPurlMax];	///<supports ipv4 ipv6 or URL

	int		size;
};
typedef struct NPoscMsg NPoscMsg;
typedef struct NPoscMsg* pNPoscMsg;

#define kNPoscConnectMax 16	///< Max number of OSC connections
struct NPosc {
	int			id;

	NPoscConn	conns[kNPoscConnectMax];	///<  list of OSC connections
	int			count;			///<number of items in list

	int			msgMode;		///< 0 = quiet, 1 = system, 2 = GUI
	int			logMode;		///< 0 = none, 1 = err only, 2 = all

	int			size;			///< active memory used
};
typedef struct NPosc NPosc;
typedef struct NPosc* pNPosc;


struct NPmapColor {
	int				id;

	pNPubyteRGBA*	palette;		///< list of que items or commands
	int				index;			///< current color
	int				altIndex;		///< alternate color (ie: black/white)
	int				count;			///< number of items in list

	int				size;			///< active memory used
};
typedef struct NPmapColor NPmapColor;
typedef struct NPmapColor* pNPmapColor;


//zz-q que and queList could be merged and made a recursive method, perhaps confusing?
enum kNP_FIFO_BUFFER_TYPE
{
	kNPfifoNull = 0,
	kNPfifoCmd,			///<for command buffers that contain NPqueCmd(s)
	kNPfifoMapItem,		///<designates buffer has NPqueItem(s)
	kNPfifoCount
};
/* basic concepts at http:///<en.wikipedia.org/wiki/Queue_(abstract_data_type)
* our high-performance queue uses double-buffered fixed memory arrays
* it does NOT use dynamically linked lists to reduce ptr overhead
* if overflow occurs it either overwrites oldest data or resizes the queue
* this depends on the 'resize' flag, if true then overflow will only occur
* when memory is capped by the apps total allocatable RAM.
* npRealloc() is called to resize, it uses runtime optimized memory pools
* in certain cases there is no data copy cost as the memory pool
* may already be configured with additional free space at end of the array
* A & B buffers are sequentially re-allocated over two program synce cycles
* avoids data copy by syncing the resize to the AB swap when the que is clear
*/
struct NPqueCmd {
	void*	funcPtr;
	void*	data;
	int		type;			///<function ptr type
};
typedef struct NPqueCmd NPqueCmd;
typedef struct NPqueCmd* pNPqueCmda;

											///< may combine these types	//zz streamline
struct NPqueItem {
	void*	mapPtr;
	void*	data;
	int		type;			///<mapPtr type... either data or cmd funcPtr //zz streamline
};
typedef struct NPqueItem NPqueItem;
typedef struct NPqueItem* pNPqueItem;


struct NPque {
	int			id;
	int			type;			///<command or data (NP)type

	NPqueItem**	list;			///<list of que items or commands
	int			enque;			///<back enque write index cannot pass deque
	int			deque;			///<front deque read index chases enque
	int			max;			///<max size of que

	bool		dynamic;		///<flag triggers resize on overflow

	int			size;			///<active memory used
};
typedef struct NPque NPque;
typedef struct NPque* pNPque;

struct NPqueList {
	int			id;
	int			type;			///<command or data (np type)

	NPque**		list;			///<list of que items or commands
	int			count;			///<number of items in list
	int			max;			///<max size of que

	int			size;			///<active memory used
};
typedef struct NPqueList NPqueList;
typedef struct NPqueList* pNPqueList;



struct NPthreadFile
{
	FILE* file;
	char* filePath;
	void* dataRef;
};
typedef struct NPthreadFile NPthreadFile;
typedef struct NPthreadFile *pNPthreadFile;

struct NPrecordTag
{
	int			id;					///<the first column is table id key

	int			recordID;			///<recordID of the node
	int			tableID;			///<tableID of the node, may not need these, debug zz

	char		title[kNPtagTitleMax + 3];	///<size + 3 for possible line endings
	char		desc[kNPtagDescMax + 3];	///<allows for newline in desc
	///<char*		desc;
	int			titleSize;			///<number of characters in title
	int			descSize;			///<number of characters in description

	int			size;
};
typedef struct NPrecordTag NPrecordTag;
typedef struct NPrecordTag *pNPrecordTag;

struct NPtag
{
	int			tableID;			///<tableID of the node, may not need these, debug zz
	int			recordID;			///<recordID of the node

	char		title[kNPtagTitleMax + 1];	///<one line with max width of 63 characters
	char		desc[kNPtagDescMax + 1];	///<allows for newline in desc
	
	int			titleSize;		///<number of characters in title
	int			descSize;		///<number of characters in description

	int			labelHead;		///< start of the link label text			//zz html
	int			labelTail;		///< end of the link label text				//zz html
	int			labelSize;		///< labelSize = labelTail - labelHead + 1	//zz html

	void*		font;

	int			mode;			///<2D, 3D billboard, fixed in model space
	float		lineWidth;		///<1 line title only, 3 lines, max lines

	NPfloatXY	boxSize;

	NPubyteRGBA color;			///<used for the text and box outline
	NPubyteRGBA boxColor;		///<background color of box
	NPubyteRGBA lineColor;		///<box outline

	int			size;
};
typedef struct NPtag NPtag;
typedef struct NPtag *pNPtag;


///< nodes are organized as a branched tree, supports complexity of linked objects
///< ie a recNode of type nodeCamera may have a multiple depth daughter tree that
///< supports several cameras or videoNodes for circle cam rigs etc...
///<-----------------------------------------------------------------------------
typedef struct NPnode NPnode;
typedef struct NPnode *pNPnode;
struct NPnode
{
	int			id;							///<local node ID 
	int			type;						///<node type, point, pin, cam, video

	void*		data;						///<node type specific data
	
	int			selected;					///<true if node currently selected

	pNPnode		parent;						///<parent node, binary tree of nodes
	int			branchLevel;				///<0 is the trunk, 1 is 1st branch
											
	pNPnode		*child;						///<child nodes attached to this one	//zz debug, update to use a circular linked list
	int			childSize;					///<child array slots currently allocated
	pNPnode		next;						///<next sibling node in linked list, N
	pNPnode		prev;						///<previous sibling in ring
	int			childIndex;					///<the currently selected child
	int			childCount;					///<current number of children

	int			chInputID;					///<track to node channel_id map
	int			chOutputID;					///<node to track record channel_id
	int			chLastUpdated;				///<cycle of last channel update

	int			average;					///<averaging type applied to data
	int			interval;					///<the interval to be averaged

	NPfloatXYZ	auxA;	//zz grid			///< topo type specific params
	NPfloatXYZ	auxB;	//zz grid			///< reserved for future use

	float		colorShift;					///<pseudo Hue shift, -180 to 180

	NPfloatXYZA	rotateVec;					///<orientation
	NPfloatXYZ	scale;						///<scale node XYZ
	NPfloatXYZ	translate;					///<location relative to origin
	NPfloatXYZ	tagOffset;					///<origin relative to parent origin

	NPfloatXYZ	rotateRate;					///<rotation rate
	NPfloatXYZ	rotate;						///<rotation angle in polar radians
	NPfloatXYZ	scaleRate;					///<scale rate 
	NPfloatXYZ	translateRate;				///<node velocity
	NPfloatXYZ	translateVec;				///<velocity unit vector

	int			shader;						///<the texture ID or procedural shader type
	int			geometry;					///<cube, sphere, pin, FFT mesh...

	float		lineWidth;					///<draws lines, zero for off
	float		pointSize;					///<draws dots, zero for off
	float		ratio;						///<geometry ratio, torus innerRadius

	int			colorIndex;
	NPubyteRGBA	color;						///<color assigned to new data

	int			colorFade;					///<cycles to fade color alpha, 0=off
	int			textureID;					///<GL texture ID

	bool		hide;						///<hide the node, data stays active
	bool		freeze;						///<freezes data and physics updates

	int			topo;						///<topography, cube, sphere, torus
	int			facet;						///<topo face number

	NPintXYZ	autoZoom;					///<scales node to fit screen, scroll, center to node origin

	NPintXYZ	triggerHi;					///<triggers are multipurpose
	NPintXYZ	triggerLo;
	NPfloatXYZ	setHi;						///<triggers
	NPfloatXYZ	setLo;

	NPfloatXYZ	proximity;					///<collision engine boundary extent
	NPintXYZ	proximityMode;				///<particle, wave and matter interactions
		
	NPintXYZ	segments;					///<geometry segments, facets, grid lines

	int			tagMode;					///<billboard tag
	int			formatID;					///<DB translation to antz field, label
	int			tableID;					///<DB table
	int			recordID;					///<DB recordID

	int			size;

	///<internal references and draw calculations not needed in CSV or DB file
	pNPtag		tag;						///<ptrr to the nodes own text tag
	NPfloatXYZ	screen;						///<MB-LABEL screen pixel coordinates
	NPfloatXYZ	world;						///<child node world coordinates
	float		distFromCamera;				///<MB-Transp					//zz debug
	int			hudType;					///<removethis, use existing param //zz debug
	bool		linkFlag;					///<for establishing link nodes
};

/// pairs an id directly to a scene node pointer, used for mapping datasets, etc.
struct NPmapItemNodeID {
	long long int	id;		///< either scene node id or other (DB) id mapping
	pNPnode			node;	///< pointer to the scene (node) THIS id references.
};							///< can be node_id, record_id, tag_id, etc...
typedef struct NPmapItemNodeID NPmapItemNodeID;
typedef struct NPmapItemNodeID *pNPmapItemNodeID;

// pairs an id directly to a scene node pointer, used for mapping datasets, etc.
struct NPmapNodeID {
	pNPmapItemNodeID*	list;	///< list of items that pair ID to node Ptr
	int					count;	///< number of items in the list
	int					size;	///< allocated size of list
};							
typedef struct NPmapNodeID NPmapNodeID;
typedef struct NPmapNodeID *pNPmapNodeID;


//! ------------------------------------------------------------------------------
//! Global Data Structure -------------------------------------------------------

struct NPkey {
	void*	coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	bool	modAlphaUpper;		///<combined shift and caps lock result

	bool	modCapsLock;		///<true when light on,		//zz debug currently not testing the capslock state

	bool	modShift;			///<combined event state
	bool	modCtrl;
	bool	modAlt;
	bool	modCommand;			///<only on OSX

	bool	modShiftLeft;		///<raw key states
	bool	modShiftRight;
	bool	modCtrlLeft;
	bool	modCtrlRight;
	bool	modAltLeft;
	bool	modAltRight;
	bool	modCommandLeft;
	bool	modCommandRight;
	
	int		skipCount;

	int		map[kNPkeyEventTypeSize][kNPkeyMapSize];	///<key command map

	int		size;
};
typedef struct NPkey NPkey;
typedef struct NPkey * pNPkey;

struct NPtool {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int type;					///<same as mouse->tool
	int mode;					///<orbitXY, fly, translate... set by button state
	int active;					///<targeting, dragging, actively using

	NPfloatXYZ	translate;		///<current position... relative to world local or screen coords?
	NPfloatXYZA	rotate;			///<current angle

	NPfloatXYZ  prevT;			///<previous position
	NPfloatXYZA prevR;			///<previous angle

	NPfloatXYZ  deltaT;			///<current - previous
	NPfloatXYZA deltaR;

	NPfloatXYZ	coordA;			///<start point, mouse down
	NPfloatXYZ	coordB;			///<second point to define region
	
	NPfloatXYZA	rotateA;		///<support for 6DOF devices
	NPfloatXYZA rotateB;

///<	int selectionSetID;
///<	void* NPselect;
};
typedef struct NPtool NPtool;
typedef struct NPtool * pNPtool;

struct NPmouse {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			x;						///<raw coordinate
	int			y;
	int			z;						///<typically the scroll wheel

	int			entry;					///< move this to io.gl

	int			camMode;
	int			pickMode;				///<locks the mouse into cam mode
	int			tool;

	bool		buttonL;				///<true when pressed
	bool		buttonC;
	bool		buttonR;

	bool		singleClick;
	bool		doubleClick;

	NPintXYZ	window;					///<window position in pixels

	NPintXYZ	previous;

	float		cmDX;
	float		cmDY;

	NPfloatXYZ	delta;					///<mouse vector
	NPfloatXYZ	deltaSum;				///<sum of deltas, clears each NPE period

	NPfloatXYZ	target;
	NPfloatXYZ	targetDest;
	float		targetRadius;
	float		targetRadiusDest;
	int			targeting;

	bool		pinSelected;

	bool		createEvent;			///<used to create click, but not drag so cam works

	pNPnode		linkA;					///<used by link tool to store first pick

	int			size;					///< memory used, add/del should modify this, debug zz
};
typedef struct NPmouse NPmouse;
typedef struct NPmouse * pNPmouse;

struct NPmessage
{
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			queIndex;				///< most recent message
	bool		rateExceeded;			///< flow rate err flag

	char		que[kNPmsgQueMax][kNPmsgLengthMax + 1]; ///<+1 for null

	int			size;
};
typedef struct NPmessage NPmessage;
typedef struct NPmessage * pNPmessage;

struct NPnodeList {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int		id;				///<db_id reference

	void**	list;
	int		count;
	int		type;

	int		size;			///<number of items in the list
};
typedef struct NPnodeList NPnodeList;
typedef struct NPnodeList * pNPnodeList;

struct NPmenu {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	char* list[kNPdbMax];		///<list of items by name
	char* name;
	char* details;
	int index;			///<default or currently selected menu item
	int count;			///<number of items in the list
};
typedef struct NPmenu NPmenu;
typedef struct NPmenu* pNPmenu;

struct NPconsole {	
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			selected;					///<true if node currently selected

	NPfloatXYZ	translate;					///<location relative to origin

	float		lineWidth;					///<draws lines, zero for off

	int			colorIndex;
	NPubyteRGBA	color;						///<color assigned to new data

	int			boxColorIndex;
	NPubyteRGBA	boxColor;					///<color assigned to new data

	int			colorFade;					///<color alpha fade with time
	int			textureID;					///<GL texture ID used for box background

	bool		hide;						///<hide the node, data stays active
	bool		freeze;						///<freezes data and physics updates

	NPfloatXYZ	screen;
	NPintXY		position;

	int			level;
	int			mode;						///<1 line, 3 lines, max lines

	NPfloatXY	box;						///<background box size

	void*		font;
	int			justify;

	NPfuncPtr   pMenuCallback;			///<menu item selected callback

	NPmenu		menuStruct;					//zzd r
	pNPmenu		menu;						///<menu list, menubar

	int			page;						///<active page, page = 0 for current
	int			selectIndex;				///<user selected line

	int			selectText;	
	int			insertText;	///<///<selectText

	int			cursorLine;					///<current cursor line number
	int			cursorColumn;				///<current cursor column number

	int			cursorType;
	bool		cursorShow;					///<display a blinking cursor

	int			historyMax;					///<max number of lines stored
	int			lineMax;					///<default is 40 lines
	int			charPerLine;				///<default is 80 characters per line

	int			lineIndex;					///<the currently displayed line
	int			lineCount;

	int			inputIndex;
	char		inputStr[kNPinputStrMax + 1];	///<user input, +1 for null '\0'

	char		line[kNPconsoleLineMax][kNPconsoleCharPerLine + 1];

	///<char*		history[kNPconsoleHistoryMax];	///<debug, zz

	int			size;
};
typedef struct NPconsole NPconsole;
typedef struct NPconsole * pNPconsole;

struct NPtags {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	void**		list;
	int			count;	//zz debug, change to listCount

	
	void**		recordList;		//zz debug, change name?
	int			recordCount;

	void**		sort;
	int			sortCount;

	int			size;
};
typedef struct NPtags NPtags;
typedef struct NPtags * pNPtags;

struct NPhud {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	pNPnode		root;

	bool		drawTags;
	bool		drawCompass;
	bool		drawCoord;
	bool		drawFPS;
	bool		drawConsole;
	bool		drawAxes;
	bool		drawMouseMode;
	bool		drawSelectMode;

	NPtags		tags;
	NPconsole	console;
	NPtag		fps;

	int			size;
};
typedef struct NPhud NPhud;
typedef struct NPhud * pNPhud;


//zz models begin
#define kNPgeoMax 50		///< max number of geometry primitives and models
#define kNPtexMax kNPtexListMax		///< max number of textures

#define kNPgeoListMax kNPgeoMax
#define kNPmodelNameMax 50
#define kNPmodelFileNameMax 75
#define kNPmodelFilePathMax 300

struct NPgeolist {
	int loaded;
	unsigned int modelId;
	int geometryId;
	char name[kNPmodelNameMax];
	char modelFile[kNPmodelFileNameMax];
	char modelPath[kNPmodelFilePathMax];
	char modelTextureFile[kNPmodelFilePathMax]; // new lv
	char modelTexturePath[kNPmodelFilePathMax]; // new lv
	NPfloatXYZ center;
	NPfloatXYZ rotate;
	NPfloatXYZ scale;
	int extTexId;
	unsigned int textureId;
};
typedef struct NPgeolist NPgeolist;
typedef NPgeolist* pNPgeolist;

typedef struct NPgeolist NPgeo;
typedef pNPgeolist pNPgeo;

#define kNPdirPathsMax 256					///< Directory paths maximum
/// Texture list and global parameters.
struct NPtex {
//	pNPtexFile	files[kNPtexMax];			///< Texture files by texmap_id
	int			count;						///< Texture maps count

	char*		dirPaths[kNPdirPathsMax];	///< Texture directories
	int			dirCount;					///< Texture directory count

	int			maxSize;						///< Maximum texture dimension
};
typedef struct NPtex NPtex;
typedef NPtex* pNPtex;

struct NPtexmap {
	int loaded;
	int type;
	int extTexId;
	int intTexId;
	char path[256];
	char filename[256];
	void* image;
	int width;
	int height;
	int channels;
	int reserved;
};
typedef struct NPtexmap NPtexmap;
typedef NPtexmap* pNPtexmap;

struct NPbox
{
	float xH;
	float xL;
	float yH;
	float yL;
	float zH;
	float zL;
};
typedef struct NPbox NPbox;
typedef NPbox* pNPbox;

//zz models end

struct NPgl {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			id;				///<the node ID used to store this GL item 

	int			windowID;		///<multiple GL contexts can share a windowID
	int			glContext;		///<multiple windows can share a GL context
								///<multi-GPU support
	char		name[kNPtagTitleMax];			///<name used for window title

	int			width;			///<current draw size, both window and fullscreen
	int			height;			///<do not set this, is set by actual dimensions

	NPintXY		windowSize;		///<non-fullscreen window size and position
	NPintXY		position;		///<can set these and call npGlobalsUpdate()

	int			fullscreen;

	int			stereo3D;
	int			pixelDepth;
	float		refreshRate;

	int			normal;
	int			shade;
	int			alphaMode;

	int			subsample;		//zzhp

	int			pickPass;
	int			pickID;
	
	int			screenGrab;		///<flag for screenGrab
	char		datasetName[kNPmaxPath];	///<used by screenGrab

	NPhud		hud;			///<one HUD per window


	void**		linkQue;		///< Draw link que
	void**		linkPreQue;		///< Pre-que for clearing un-drawn links
	int			linkQueCount;
	int			linkPreQueCount;

	NPgeolist geolist[kNPgeoMax];	///< models list	 //zz models begin
	pNPgeolist 	geoMap[kNPgeoMax];	///<
	int			geoX;				///<
	int			geoLen;				///<
	bool		geoLock;			///< lock flag, if true: don't access, if false: safe to access 
	int			numPrimitives;		///<
	int			numModels;			///<
	int			modelId;			///<
	unsigned int dl;				///< base Lisplay List

	NPtexmap	texmap[kNPtexMax];
	int			texmapCount;
	int			extMap[100];
	pNPtexmap	extMapMe[100];
	int			tAutoCount;	
	
	NPtex		tex;				///< Texture Maps	//zz models end

	int			size;
};
typedef struct NPgl NPgl;
typedef struct NPgl * pNPgl;


#define kNPmaxSets 4096
struct NPselect {
	pNPnodeList	set[kNPmaxSets];
	int			setCount;
};
typedef struct NPselect NPselect;
typedef struct NPselect * pNPselect;


struct NPmap {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	NPselect	select;					///< 

	void**		node;					///<root node array, uses kNPnodeRootMax
	void**		sort;					///<used for z-sort during GL draw

	int			sortSwap;				///<0 for sortA and 1 for sortB			//zzhp
	int			sortSwapFlag;			///<sort is done and ready for swap
	void**		sortA;					///<double buffered async sort
	void**		sortB;					///<double buffered async sort			//zzhp

	void**		nodeID;					///<maps nodeID to pNPnode, kNPnodeMax
	void**		sortID;					///<maps nodeID for sorting nodes

	int*		parentID;				///<maps node ID to parentID
	int*		orphanList;				///<list of orphans by node ID
	int			orphanCount;
	int			sortCount;														//zzhp
	int			sortCountA;
	int			sortCountB;														//zzhp

	bool		syncNodes;
	bool		syncTagsReady;
										///< 2^24 max for picking algorithm
	int			nodeCount;				///<total for root and children nodes
	int			nodeRootCount;			///<number of root nodes
	int			nodeRootIndex;			///<the active node root
	
	int			tagCount;				///<number of tags
	
	pNPnode		previousNode;			///<used for clicking away...
	pNPnode		currentNode;			///<active node, commands, traversing tree
	pNPnode		currentCam;				///<active camera used for zsort distance

	pNPnode		selectedGrid;			///<selected grid
	pNPnode		selectedPinNode;		///<currently selected node
	int			selectedPinIndex;		///<helpful to know which tree we are on
	pNPnode		selectedHUD;			///<the currently selected HUD item

	NPboolXYZ	selectSet;				///<current selection set to add too
	int			selectAll;				///<true when all nodes selected

	pNPmapColor	color[kNPpaletteMax];	///< color array size of kNPpaletteMax

	pNPmapType	typeMap[kNPdataTypeMax];

	pNPmapType	typeMapBase;
	pNPmapType	typeMapNode;
	pNPmapType	typeMapCamera;
	pNPmapType	typeMapPin;
	pNPmapType	typeMapGrid;
		
	pNPmapType	typeMapTag;				///< debug db //zzssql 
	pNPmapType	typeMapChMap;			///< debug db //zzssql

	pNPmapType	typeMapPalette;				//zz color

	pNPmapType	mapTypeList;
	int			mapTypeCount;

	pNPmapLink	typeMapGlobals;
	pNPmapLink	typeMapOSC;
	int			globalsCount;
	int			oscCount;

	char		startupMsg[kNPurlMax];

	int			size;					///< memory used, add/del should modify this, debug zz
};
typedef struct NPmap NPmap;
typedef struct NPmap * pNPmap;


///<JJ-zz
///<JJ - currently using track data column naming convention for mapping track-to-node-attribute (bottom part of structure will be unnecessary when channel file exists)
struct NPxref {
	int			channelNumber;
	int			trackOffset;
	char*		attributeName;
};
typedef struct NPxref NPxref;
typedef struct NPxref * pNPxref;

///< the pointer logic is same in all cases, processing requires different casting for the "void*" and "void**"
///< for example, when processing float properties, these have to be cast to "float*" and "float**"
struct NPnodePropertyTracks {
	///< for mapping track-to-node-attribute -- set up when a Node subscribes to a channel 
	void*		nodePropertyMemoryLocations[kNPmaxFloatProperties];		///< memory address of each node property fed from track data
	void**		nodePropertyMemoryNextLocation;							///< next available location for a float property memory address
	int			propertyTracks[kNPmaxFloatProperties];					///< offset of the tracks that feed each peroperty
	int			propertyTrackIndex;										///< next available propertyTrack array offset
};
typedef struct NPnodePropertyTracks NPnodePropertyTracks;
typedef struct NPnodePropertyTracks * pNPnodePropertyTracks;

#define kNPchannelMapHeader "id,channel_id,track,attribute,track_table_id,ch_map_table_id,record_id"

///< NPtrackDataSource.dataSourceType constants
enum
{
	kDataSourceUndefined,
	kDataFromFile,
	kDataFromUDP
};

#define kTracksDefaultUDPserverPort 1010		///< default UDP port is JJ abcdefghij, selected when UDP port not specified
#define kSocketTypeUnspecified -1		///< initial value for channel socket trackSocketTypeId

struct NPtrackDataSource {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	///<  kDataFromFile -- load data from Files
	///<  kDataFromUDP -- load data from UDP
	///<
	int			dataSourceType;

	///< when dataSourceType is kDataFromFile
	FILE*		trackFP;		///< for reading *Ch* files, used in a separate thread, and may remain open
	char*		fullFilePath;
	FILE*		channelFP;

	///< when dataSourceType is kDataFromUDP, we use the oscPackListener in NPio as the data source, the message itself 
	///< then routes data to the tracks
};
typedef struct NPtrackDataSource NPtrackDataSource;
typedef struct NPtrackDataSource * pNPtrackDataSource;

struct NPch {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			setNumberToLoad;

	///<
	///<  LOADED FROM CHANNEL FILE
	///<
	NPxref		xref[kNPmaxTrackToAttributeMappings];
	int			xrefIndex;
	char		channelFileTokenBuffer[kNPmaxLineLength];
	char*		channelFileTokens[kNPmaxTokens];			

	///<
	///< LOADED FROM TRACKS FILES
	///<
	///< data source for the tracks
	NPtrackDataSource trackDataSource;
	char		trackFileTokenBuffer[kNPmaxLineLength];
	char*		trackFileTokens[kNPmaxTokens];			

	///< properties apply to all tracks on the channel
	bool		updateData;				///< true when we need to update data (either read from track to update node, or vice versa)
	int			channelReadIndex;		///< offset in all tracks, next data to read
	int			channelWriteIndex;		///< offset in all tracks, next data to read
	int			channelDataSize;		///< amount of data in each track, any value between 0 and kNPtrackBufferSize
	int			channelSampleRate;		///< sample rate for this channel, # of milliseconds to keep same channel data, e.g. 50 === 20 changes per second (1000 ms/50 ms)
	long long	channelNextDataChangeTime;	///< next time to change data (by updating channelReadIndex) on this channel, used with sample rate to determine when to change data
	
	///< track specific data, data loaded from the track file
	int			numberTracks;			///< number of tracks in this channel
	char**		trackNames;				///< each track has a name, name indicates node attribute (multiple allowed, separated by anything except ",")

	///< all data types created for each track (until we have way of knowing which is needed)
	float**		trackFloatData;			///< list of pointers to "float" values, track data as "float"
	unsigned char** trackUcharData;		///< list of pointers to "unsigned char" values, track data as "unsigned char"
	int**		trackIntData;			///< list of pointers to "int" values, track data as "int"

	NPnodePropertyTracks	floatTracks;///< tracks associated with "float" Node properties
	NPnodePropertyTracks	ucharTracks;///< tracks associated with "unsigned char" Node properties
	NPnodePropertyTracks	intTracks;	///< tracks associated with "int" Node properties

	bool		pause;

	int			size;
};
typedef struct NPch NPch;
typedef struct NPch* pNPch;
///<JJ-zz end


struct NPfile
{
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			loading;				///<used to signal processes to pause
	bool		saveSelect;

	char		appPath[kNPmaxPath];
	char		csvPath[kNPmaxPath];
	char		mapPath[kNPmaxPath];
	char		modelPath[kNPmaxPath];  ///< 3D models //zz models change to model(s)Path[]
	char		currentOpenPath[kNPmaxPath];
///<	char		cwdPath[4096];			//zz debug, maybe better to use GetCWD
	char		userSelectedPath[kNPmaxPath];

	int			size;
};
typedef struct NPfile NPfile;
typedef struct NPfile* pNPfile;

/*
struct NPconnect
{
	NPoscPackConn oscListener;	//zz osc

	int			size;
};
typedef struct NPconnect NPconnect;
typedef struct NPconnect* pNPconnect;
*/
#define kNPqueMax 256
#define kNPmaxConnect 256


/*! lde @todo
* Create NPos
*  - containing structure with function pointers to os functions
*/

struct NPosFuncSet
{
	void	(*getAppPath)();
	void	(*getCWD)();
	void	(*setCWD)();
	void	(*getOpenFilePath)();
	int		(*fileDialog)();
	int		(*showCursor)();
	int		(*setCursorPos)();
	double	(*getTime)();
	void	(*updateTime)();
	void	(*sleep)();
	int		(*getKey)();
	void	(*timeStampName)();
	void	(*beginThread)();
	void	(*endThread)();
	bool	(*supportsAntzThreads)();
	void*	(*loadLibrary)(char* filePath);
	void*	(*getLibSymbol)();
};
typedef struct NPosFuncSet NPosFuncSet;
typedef struct NPosFuncSet* pNPosFuncSet;

struct NPos {
	pNPosFuncSet (*newFuncSet)(); // removed void
	void (*hook)();
	int (*deleteFuncSet)(); // Specify later, lde @todo
	pNPosFuncSet funcSet;
};
typedef struct NPos NPos;
typedef struct NPos* pNPos;


struct NPio {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int			argc;
	char**		argv;

	NPos		os;					///< Operating System specific	//zzd
	
	NPmessage	message;
	NPkey		key;
	NPmouse		mouse;
	NPgl		gl;
	NPch		ch;
	NPfile		file;

///<	struct	dbNewConnect *connect;	//zzsql							//zz debug	//zz dbz
//	struct databases *dbs;			//zz dbz
	NPdbs		db;
	
///<	NPoscPackConn oscListener;		///<JJ-zz
//	pNPconnect	connect[kNPmaxConnect];	//zz osc

	void*		assimp;				//zz models
	NPgithub	github;
	NPcurl		curl;
	NPjson		json;

	int			connectCount;
	NPosc		osc;				///<OSC stuff uses io que for thread safety

	NPqueList	fifo;				///<io buffering for data and command sync
	//zz should que be part of npmap? ...its a 'io' (data) buffer

	/// @todo move time params to data->io.time
	double		time;				///<system time in seconds
	double		timeStart;			///<time at app start
	double		cycleDelta;			///<the actual duration of the last cycle
	double		cycleRateHz;		///<target rate, not necessarily the actual
	double		cyclePeriod;		///<period calculated from rate
	int			cycleCount;			///<number of cycles since app startup

	bool		blinkState;			///<blinking cursor state

	bool		clearFrameBuffer;	///<1 to clear screen each draw, 0 accumulates
	int			clearIndex;			///<background color index, black, white...
	NPfloatRGBA clear;				///<background color for clearing the screen ///<move to gl

	int			drawMenu;

	NPboolXYZ	axes;					///<selects active x,y,z, axes			debug, zz

	char*		write;

	char		url[kNPurlMax];
	char		gitvizURL[kNPurlMax];

///<	databases	dbs;

	int			hideLevel;

	int			refCount;			// Reference Counter, lde @todo
	
	int			msgFlowFile;		///< File load message rate control
	int			msgFlowCmd;			///< User command message rate control

	int			size;
};
typedef struct NPio NPio;
typedef struct NPio* pNPio;

struct NPcpu {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int multiPartition;		///<multi-partition NUMA HPC systems, 16384+ sockets
	int systemNodes;		///<number of system nodes in this partition
	int sockets;			///<number of CPU sockets
	int cores;				///<NUMA systems have up to 2048+ cores per partition
	int threads;			///<total threads, hyper-threads

	int socketsPerNode;		///<the number CPUs on a single board, shared RAM
	int coresPerCPU;		///<1,2,4,6,8,12,16...
	int threadsPerCore;		///<typically 1 or 2
	int threadsPerCPU;		///<typically either twice the core count or same as

	///<memory
	long long cacheL1;		///<L1 cache per thread
	long long cacheL2;		///<L2 cache per core
	long long cacheL3;		///<L3 cache per CPU

	long long multiRAM;		///<total RAM across all NUMA partitions
	long long systemRAM;	///<total RAM in this NUMA partition

	long long RAM;			///<RAM per motherboard or compute blade
	long long appRAM;		///<per CPU core

	int		size;
};
typedef struct NPcpu NPcpu;
typedef struct NPcpu* pNPcpu;

///< List of users, stores account and other session info.
#define kNPuserMax 256					//zz select
struct NPuserEnvironment{							//zz select
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	int activeContext;
	int activeWindow;
	int activeNode;

	pNPnode	tool;				///<

	int size;
};
typedef struct NPuserEnvironment NPuserEnvironment;
typedef struct NPuserEnvironment* pNPuserEnvironment;


/// List of users, stores account and other session info.
#define kNPuserMax 256
struct NPuser{	
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.
						//zz select
	char name[256];
	char pwd[256];

	pNPuserEnvironment environment;		//zz perhaps move to map->user[] question is public or private
	pNPnode	tool;				///<
};
typedef struct NPuser NPuser;
typedef struct NPuser* pNPuser;			///< @todo move 'tool' element to user->environment->tool

/*!
*  Control functions, environment variables and user specific data
*/
struct NPctrl {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///< each global struct has a corresponding base node.

	pNPuser user[kNPuserMax];			///< List of users, stores session info.

	int userCount;
	
	float	slow;						///< slow velocity, shift not pressed //zz move this to io or npengine
	float	fast;						///< fast velocity, shift key pressed 

	NPcpu	cpu;

	bool	startup;

	void**	cmdFunc;					//zz maybe move to data->map->functions[]

	int		size;
};
typedef struct NPctrl NPctrl;
typedef struct NPctrl* pNPctrl;


/*!
*  Global Context reference using MVC architecture
*/
struct Data {
	void* coreNode; ///< core nodes tie global structures to the scene graph
						///<  each global struct has a corresponding base node.
	NPmap	map;		///< Model   - map
	NPio	io;			///< View    - io
	NPctrl	ctrl;		///< Control - ctrl

	int		size;
};
typedef struct Data Data;
typedef struct Data* pData;


///< for displaying labels related to node parameters
///< displays the name at the top, followed by X,Y,Z values in a column
///< unitType determines display format and appends the unit label, ie 9.8 m/s^2
///< the nodeField (ie, translate.x) is first multiplied by the factor then offset
///< displayValue = nodeField * factor + offset
///<------------------------------------------------------------------------------
struct NPlabel
{
	char		name[64];			///<author, city, population...

	NPfloatXYZS	factor;				///<multiplies the value for scaling range
	NPfloatXYZS	offset;				///<constant to offset value by

	NPintXYZS	unitType;			///<ft, m, F, C, kg/m^3, m/s^2, R,G,B...

///<	NPintXYZS	formatType;			///<specifies conversion math, y = mx + c

	NPintXYZS	nodeField;			///<which node parameter to display
};
typedef struct NPlabel NPlabel;
typedef struct NPlabel *NPlabelPtr;


///< cameras can be GL scene cameras or real-world cameras
///< video is attached as a daughter of the recNode
///<-----------------------------------------------------------------------------
struct NPcamera
{
	int			id;							///<node ID
	int			type;

	int			format;						///<320p, 480i, 720p, 4K full app
	int			interlaced;					///<interlaced field order
	int			stereo3D;						///<stereoscopic 3D
	float		aspectRatio;				///<1.0, 1.333, 1.777, 1.85, 2.25... 
	float		fps;						///< 15, 24, 29.97, 30, 59.94, 120...
	int			colorSpace;					///<8, 12bit, YUV, RGBA, XYZ, CMYK...
	int			width;						///<res in pixels
	int			height;

	float		fov;						///<FOV 35mm, 70mm...
	float		clipNear;
	float		clipFar;
	
	///< real world camera parameters
	float		aperture;					///<F stop
	float		exposure;					///<in seconds
	int			sensorType;					///<3CCD, Debayer pattern...

	float		matrix[16];					///<for local to world coord convert
	float		inverseMatrix[16];			///<for local to world coord convert

	int			size;
};
typedef struct NPcamera NPcamera;
typedef struct NPcamera *NPcameraPtr;


///<-----------------------------------------------------------------------------
struct NPvideo
{
	int			id;							///<node ID
	int			type;

	int			format;						///<320p, 480i, 720p, 4K full app
	int			interlaced;					///<interlaced field order
	int			stereo3D;						///<stereoscopic 3D
	float		aspectRatio;				///<1.0, 1.333, 1.777, 1.85, 2.25... 
	float		fps;						///< 15, 24, 29.97, 30, 59.94, 120...
	int			colorSpace;					///<8, 12bit, YUV, RGBA, XYZ, CMYK...
	int			width;						///<res in pixels
	int			height;

	float		brightness;
	float		contrast;
	float		hue;
	float		saturation;
	float		blackLevel;

	NPfloatRGBA	bias;						///<color shift
	NPfloatRGBA	gain;						///<color multiplier

	int			size;
};
typedef struct NPvideo NPvideo;
typedef struct NPvideo *NPvideoPtr;

///<-----------------------------------------------------------------------------
struct NPgis
{
	int			id;							///<node ID
	int			type;

	float		*vertexArray;				///<the array of vertices
	float		*normalArray;				///<the array of normals
	unsigned char	*colorArray;				///<each point gets color assigned
	int			vertexCount;				///<
	int			normalCount;				///<
	int			arrayIndex;					///<current vertex index in the array
	///<			polyLine
	///<			polyStrip

	int			size;
};
typedef struct NPgis NPgis;
typedef struct NPgis *NPgisPtr;



///<-----------------------------------------------------------------------------
struct NPsurface
{
	int			id;							///<node ID
	int			type;

	float		*vertexArray;				///<the array of vertices
	float		*normalArray;				///<the array of normals
	unsigned char	*colorArray;				///<each point gets color assigned
	int			vertexCount;				///<
	int			normalCount;				///<
	int			arrayIndex;					///<current vertex index in the array

	int			size;
};
typedef struct NPsurface NPsurface;
typedef struct NPsurface *NPsurfacePtr;


///<-----------------------------------------------------------------------------
struct NPpoints
{
	int			id;							///<node ID
	int			type;
	float**			vertexArray;				///<the array of data points
	unsigned char**	colorArray;				///<each point gets color assigned
	int			vertexCount;				///<number of points in the node
	int			arrayIndex;					///<current vertex index in the array

	int			size;
};
typedef struct NPpoints NPpoints;
typedef struct NPpoints *NPpointsPtr;


///<-----------------------------------------------------------------------------
struct NPpin
{
	int			id;							///<node ID
	int			type;

	float		innerRadius;				///<inner radius of the toroid
	float		outerRadius;				///<outer radius of the toroid

	int			radiusRatioIndex;			///<sorts pre-loaded GPU toriods
	int			screenSizeIndex;			///<same as above for detail level
	
	int			slices;						///<calculated from radiusRatioIndex
	int			stacks;						///<...and from the screenSizeIndex

	NPfloatXYZ	translateTex;
	NPfloatXYZ	scaleTex;
	NPfloatXYZA	rotateTex;					///<orientation of the texture map

	int			size;
};
typedef struct NPpin NPpin;
typedef struct NPpin *NPpinPtr;


///<-----------------------------------------------------------------------------
struct NPgrid
{
	int			id;							///<node ID
	int			type;

	NPfloatXYZ	spacing;
	bool		overlay;					///<overlays relative to screen coord

	int			size;
};
typedef struct NPgrid NPgrid;
typedef struct NPgrid * NPgridPtr;


///<-----------------------------------------------------------------------------
struct NPnodeData
{
	int			id;							///<node ID
	int			type;

	int			index;
	void*		a;			///<points to an array of data pointers
	int*		dataType;		///<points to an int of the data type,

	int			size;
};
typedef struct NPnodeData NPnodeData;
typedef struct NPnodeData * pNPnodeData;


///<-----------------------------------------------------------------------------
struct NPdataItem
{
	int			type;
	void*		item;
};
typedef struct NPdataItem NPdataItem;
typedef struct NPdataItem * NPdataItemPtr;


///<-----------------------------------------------------------------------------
///<-----------------------------------------------------------------------------
///<-----------------------------------------------------------------------------
///< constants ------------------------------------------------------------------

enum kNP_NODE_TYPES
{
	kNodeDefault = 0,

	kNodeCamera,			///<for both real-world and virtual GL cameras
	kNodeVideo,				///<video from live/stream, internal GL, or file
	kNodeSurface,			///<ground, sky... background objects drawn first
	kNodePoints,			///<for X/XY/XYZ line, eeg/ecg, temp, movement...
	kNodePin,				///<combo of cones, spheres, toroids, etc...
	kNodeGrid,				///<grid in 1D, 2D or 3D
	kNodeLink,				///<links any two nodes
	kNodeHUD,				///<HUD elements and tools
	kNodeCount
};

/*
enum
{
	kSurfaceNull = 0,
	kSurfacePlane,
	kSurfaceCylinder,
	kSurfaceCone,
	kSurfaceSphere,
	kSurfaceTorus,
	kSurfacePin,							///< ice-cream cone shape
	kSurfaceMesh,							///< terrain...
	kSurfaceLayers,							///< ie stack of FFT images, MRI...
	kSurfaceCamera,
	kSurfaceTypeCount
};
*/

enum kNP_SELECT_COMMANDS
{
	kSelectNULL = 0,
	kSelectAll,
	kSelectNone,
	kSelectAdd,
	kSelectRemove,
	kSelectColor,
	kSelectParent,
	kSelectSiblings,
	kSelectChildren,
	kSelectProperty
};

enum kNP_MAP_TYPE
{
	kNPmapNull = 0,			///<default native handler
		
	kNPmapNP,				///<map descriptor to build maps

	kNPmapRoot,				///<1 root to rule them all
	kNPmapMultiverse,		///<collection of universes		
	kNPmapUniverse,
	kNPmapGalaxyCluster,
	kNPmapGalaxy,
	kNPmapSolar,			///<single star or binary/trinary... system

	kNPmapWorld,			///< Earth is 100ms typical, 10ms theoretical
	kNPmapRegion,			///< < 30 ms latency, < 4000km  kNPmapLocation,			///< < 3 ms latency, < 400km   kNPmapCluster,			///< < 1 ms latency, < 40km
	kNPmapSpot,				///< < 1 ms latency, ///< < 4m, CPU node, not scene node

	kNPmapSystem, 
	kNPmapIO,				///< typically a bus device if a node
						
	kNPmapBUS,				///< Node maps
	kNPmapCPU,
	kNPmapRAM,
	kNPmapGPU,

	kNPmapDrive,
	kNPmapNetwork,
	kNPmapVideo,
	kNPmapAudio,

	kNPmapHaptic,
	kNPmapKey,
	kNPmapMouse,
	kNPmapJoystick,
	kNPmapTablet,
	kNPmapTouchScreen,
	kNPmapHID,				///<end physical locality start virtual structures

	//zz  above is physical context, perhaps below should be separate list

	kNPmapNPE,
	kNPmapDraw,				///<a list of objects to draw in a GL context 
	kNPmapQuadsort,			///<break scene into quadrants, cluster and NPE
	kNPmapZsort,				///<GL draw order, for correct alpha rendering

	kNPmapTree,
	kNPmapList,
	kNPmapArray,
	
///<	kNPmapNodeNull,

	kNPmapGlobals,
	kNPmapNode,
	kNPmapTag,
	kNPmapChannel,
	kNPmapTrack,
	kNPmapGL,

	kNPmapCSV,			///<generic table/row/field/type
	kNPmapJSON,			///<likely the most descriptive
	kNPmapXML,			///<for 3rd party data support
	kNPmapKML,			///<google KML, related to COLLADA

	kNPmapOSC,			///<default 3rd party OSC mapping
	kNPmapSNMP,			///<can bridge SNMP-JSON-OSC

	kNPmapDB,			///<native DB format is MySQL

	kNPmapNodeCSVvOne,	//zz debug... move this elsewhere
	kNPmapNodeCSV,		//zz

	kNPmapCount
};


enum kNP_SHADER_TYPE
{
	kShadingNull = 0,
	kShadingWire,
	kShadingFlat,
	kShadingGouraud,
	kShadingPhong,
	kShadingReflection,
	kShadingRaytrace,
	kShadingCount
};

enum kNP_CAMERA_SENSOR_TYPE
{
	kCameraSensorNull = 0,					///<don't know the sensor type
	kCameraSensorMonoCCD,					///<monochrome CCD such as BW, IR
	kCameraSensorDebayerCCD,				///<single color CCD debayer pattern
	kCameraSensorThreeCCD,					///<3CCD color sensor
	kCameraSensorDepthCCD					///<BW or color with Z depth data
};

/// We are defining a subset of the large number of video and film resolutions.
/// Film uses many others http://www.celco.com/formatresolutiontable4k.asp
/// This table does not define the FPS (vertical frequency).
enum kNP_VIDEO_FORMAT										///<add DPX format support, zz
{
	kVideoFormatNull = 0,
	kVideoFormatNTSC,
	kVideoFormatPAL,
	kVideoFormat720p,
	kVideoFormat1080i,
	kVideoFormat1080p,
	kVideoFormat2K,				///< 2048x1536 is 35MM FULL SCREEN
	kVideoFormat4K_UHD,			///< UHD is 3840x2160
	kVideoFormat4K_DCI,			///< 4K DCI is 4096x2160
	kVideoFormat8K_UHD,			///< 8K UHD is 7680x4320
	kVideoFormat8K_DCI,			///< 8K DCI 8192x4320
	kVideoFormat8K_fulldome,	///< 8192x8192 used for dome projection
	kVideoFormat16K,
	kVideoFormat32K,
	kVideoFormat64K,
};

enum kNP_VIDEO_HZ									///<add DPX format support, zz
{
	kVideoFPS_Null = 0,
	kVideoFPS_23_976HZ,		///<Film on Video
	kVideoFPS_24HZ,			///<Film
	kVideoFPS_25HZ,			///<PAL
	kVideoFPS_29_97HZ,		///<NTSC
	kVideoFPS_30HZ,			
	kVideoFPS_59_94HZ,		///<1080i, 60i, 60
	kVideoFPS_60HZ,
	kVideoFPS_72HZ,			///<experimental format
	kVideoFPS_119_88HZ,
	kVideoFPS_120HZ
};

enum kNP_VIDEO_INTERLACED
{
	kVideoFieldNull = 0,
	kVideoFieldOdd,
	kVideoFieldEven
};

enum kNP_VIDEO_STEREO_3D
{
	kVideoStereoNull,
	kVideoStereoLeft,
	kVideoStereoRight,
	kVideoStereoSequential
};

/// Color Space bit depth
/// @todo add XYZ color space, such as used by DCI JPEG2000
enum kNP_COLOR_SPACE
{
	kColorSpaceNull = 0,
	kColorSpaceRGB_8,			///<bits per channel
	kColorSpaceRGB_10,
	kColorSpaceRGB_12,
	kColorSpaceRGB_16,
	kColorSpaceRGB_24,
	kColorSpaceRGB_32,	
	kColorSpaceRGBA_8,			///<bits per channel
	kColorSpaceRGBA_10,
	kColorSpaceRGBA_12,
	kColorSpaceRGBA_16,
	kColorSpaceRGBA_24,
	kColorSpaceRGBA_32,
	kColorSpaceYUV_422_8,		///<bits per channel
	kColorSpaceYUV_422_10,
	kColorSpaceYUV_422_12,
	kColorSpaceYUVA_422_8,		///<bits per channel
	kColorSpaceYUVA_422_10,
	kColorSpaceYUVA_422_12,
	kColorSpaceYUV_444_8,		///<bits per channel
	kColorSpaceYUV_444_10,
	kColorSpaceYUV_444_12,
	kColorSpaceYUV_444_16,
	kColorSpaceYUVA_444_8,		///<bits per channel
	kColorSpaceYUVA_444_10,
	kColorSpaceYUVA_444_12,
	kColorSpaceYUVA_444_16,
	kColorSpaceCMYK_8,			///<bits per channel
	kColorSpaceCMYK_16,
};

enum kNP_TRANSPARENCY_ALPHA_MODE
{
	kNPalphaModeNull = 0,
	kNPalphaModeSubtractive,	///<default alphaMode
	kNPalphaModeAdditive,
	kNPalphaModeDark,
	kNPalphaModeCount
};

enum kNP_AVERAGE_TYPE
{
	kAverageNull = 0,
	kAverage,
	kAverageRootMeanSquare,
	kAverageSmoothed,
	kAverageRunning
};

enum kNP_CHANNEL_MISC
{
	kChannelNull = 0,
	kChannelCycleCount,
	kChannelOscilloscope
};

enum kNP_FILE_DIALOG_TYPE
{
	kNPfileDialogNULL = 0,

	kNPfileDialogNew,
	kNPfileDialogOpen,

	kNPfileDialogSave,
	kNPfileDialogSaveAs,

	kNPfileDialogImport,
	kNPfileDialogExport,

	kNPfileDialogClose
};


///< 120Hz timer, 60, 30, 24
///< 119.88Hz timer, 59.94, 29.97, 23.976


///<---- Ctrl Command Constants ----

/*!
*  Control Command constants used by npCtrlCommand function.
*/
enum kNP_COMMAND_TRIGGER {
	///<global ctrl commands
	kNPcmdNull = 0,

	kNPcmdFileNew,
	kNPcmdFileOpen,
	kNPcmdFileClose,
	kNPcmdFileSave,
	kNPcmdFileSaveAs,
	kNPcmdFileImport,
	kNPcmdFileExport,

	kNPcmdScreenGrab,

	kNPcmdFileMapOne,		///<used for quick loading and saving of state files
	kNPcmdFileMapTwo,
	kNPcmdFileMapThree,

	kNPcmdVizCode,			//zz vizsrc.com
	kNPcmdFileViz,
	kNPcmdGitViz,
	kNPcmdWebViz,

	///< launch external app
	kNPcmdOpenURL,
	kNPcmdOpenNodeFile,
	kNPcmdOpenApp,
	kNPcmdOpenAntz,

	kNPcmdConsole,
	kNPcmdViewer,
	kNPcmdViewer2, //lde @todo

	///<global graph commands
	kNPcmdMenu = 4242,
	kNPcmdTagMode,
	
	kNPcmdNew,
	kNPcmdDelete,

	kNPcmdNext,
	kNPcmdNextOff,
	kNPcmdPrev,
	kNPcmdPrevOff,
	kNPcmdNextBranch,
	kNPcmdNextBranchOff,
	kNPcmdPrevBranch,
	kNPcmdPrevBranchOff,

	kNPcmdCamera,
	kNPcmdGrid,
	kNPcmdLight,
	kNPcmdPin,
	kNPcmdHUD,

	kNPcmdSelect,
	kNPcmdSelectOff,
	kNPcmdSelectToggle,

	kNPcmdSelectAll,
//	kNPcmdSelectAllOn,
//	kNPcmdSelectAllOff,
	kNPcmdSelectNone,
	kNPcmdSelectAllInvert,

	kNPcmdSelectOne,
	kNPcmdSelectTwo,
	kNPcmdSelectThree,
	kNPcmdSelectSetNone,

	kNPcmdPresetOne,
	kNPcmdPresetTwo,
	kNPcmdPresetThree,
	kNPcmdPresetFour,
	kNPcmdPresetFive,
	kNPcmdPresetSix,
	kNPcmdPresetSeven,
	kNPcmdPresetEight,

	///<translate, rotate and scale selected objects
	kNPcmdSelectAxes,
	
	kNPcmdXincrease,
	kNPcmdXincreaseOff,
	kNPcmdXdecrease,
	kNPcmdXdecreaseOff,
	
	kNPcmdYincrease,
	kNPcmdYincreaseOff,
	kNPcmdYdecrease,
	kNPcmdYdecreaseOff,
	
	kNPcmdZincrease,
	kNPcmdZincreaseOff,
	kNPcmdZdecrease,
	kNPcmdZdecreaseOff,
	
	kNPcmdRotateLeft,
	kNPcmdRotateLeftOff,
	kNPcmdRotateRight,
	kNPcmdRotateRightOff,

	kNPcmdRotateUp,
	kNPcmdRotateUpOff,
	kNPcmdRotateDown,
	kNPcmdRotateDownOff,

	kNPcmdRotateCCW,
	kNPcmdRotateCCWOff,
	kNPcmdRotateCW,
	kNPcmdRotateCWOff,

	kNPcmdZoomOn,
	kNPcmdZoomOff,
	
	kNPcmdClearFrameBuffer,
	kNPcmdBackground,
	
	///<node specific graph commands
	kNPcmdSampleInterval,
	kNPcmdChannelDown,
	kNPcmdChannelUp,
	kNPcmdPause,

	kNPcmdNormalMode,
	kNPcmdShaderMode,
	kNPcmdAlphaMode,

	kNPcmdColorUp,
	kNPcmdColorDown,
	kNPcmdColorFade,
	kNPcmdAltColor,
	kNPcmdColorPalette,
	
	kNPcmdAlphaUp,
	kNPcmdAlphaDown,
	kNPcmdBiasUp,
	kNPcmdBiasDown,
	kNPcmdGainUp,
	kNPcmdGainDown,

	kNPcmdTexture,
	kNPcmdTextureDown,

	kNPcmdFreeze,
	kNPcmdHide,
	kNPcmdClear,
	
	kNPcmdCenter,
	kNPcmdScroll,

	kNPcmdPoints,	
	kNPcmdLines,
	kNPcmdSegments,
	kNPcmdShader,
	kNPcmdGeometry,
	kNPcmdPrimitiveDown,

	kNPcmdTopo,
	kNPcmdTopoDown,
	kNPcmdMesh,

	kNPcmdSetpointLo,											///< add kNPcmd..,  debug zz
	kNPcmdSetpointLoOff,
	kNPcmdSetpointHi,
	kNPcmdSetpointHiOff,

	kNPcmdFullscreen,

	kNPcmdRatio,

	kNPcmdTool,
	kNPcmdToolDown,

	kNPcmdSubsample,			//zzhp

	kNPcmdTrigger,

	kNPcmdContext,			///<next active (multi-user) ctrl context
	kNPcmdContextID,		///<set context by ID, console = 0, main window = 1
	kNPcmdUserFocus,		///<set user focus, will bring window to front

	kNPcmdCount
};

//zz  debug move key codes npkey.h
enum kNP_KEY_EVENT_TYPE
{
	kKeyDown = 1,
	kKeyRepeat,
	kKeyUp,
	kKeyModifier,
	kKeyDownASCII,
	kKeyUpASCII,
	kKeyDownSpecial,
	kKeyUpSpecial
};

enum kNP_KEY_EVENT_GLUT_SPECIAL
{
	kGlutKeyDown = 1,
	kGlutKeyUp,
	kGlutKeyDownSpecial,
	kGlutKeyUpSpecial
};

///<OSX keycodes, are non-ASCII
enum kNP_KEY_CODES_OSX
{
	kKeyCodeA = 0x00,
				kKeyCodeS,
				kKeyCodeD,
				kKeyCodeF,
				kKeyCodeH,
				kKeyCodeG,
				kKeyCodeZ,
				kKeyCodeX,
				kKeyCodeC,
				kKeyCodeV = 0x09,
				kKeyCodeB = 0x0b,
				kKeyCodeQ,
				kKeyCodeW,
				kKeyCodeE,
				kKeyCodeR,
				kKeyCodeY,
				kKeyCodeT,
				kKeyCode1,
				kKeyCode2,
				kKeyCode3,
				kKeyCode4,
				kKeyCode6,
				kKeyCode5,
				kKeyCodeEqual,
				kKeyCode9,
				kKeyCode7,
				kKeyCodeMinus,
				kKeyCode8,
				kKeyCodeZero,
				kKeyCodeRightBracket,
				kKeyCodeO,
				kKeyCodeU,
				kKeyCodeLeftBracket,
				kKeyCodeI,
				kKeyCodeP,
				kKeyCodeReturn,
				kKeyCodeL,
				kKeyCodeJ,
				kKeyCodeRightQuote,
				kKeyCodeK,
				kKeyCodeSemiColon,
				kKeyCodeBackSlash,
				kKeyCodeComma,
				kKeyCodeSlash,
				kKeyCodeN,
				kKeyCodeM,
				kKeyCodePeriod,
				kKeyCodeTab,
				kKeyCodeSpace,
				kKeyCodeTilda,
				kKeyCodeDelete = 0x33,
				kKeyCodeBackSpace,
				kKeyCodeESC = 0x35,
				
				kKeyCodeF1 = 0x7A,
				kKeyCodeF2 = 0x78,
				kKeyCodeF3 = 0x63,
				kKeyCodeF4 = 0x76,
				kKeyCodeF5 = 0x60,
				kKeyCodeF6 = 0x61,
				kKeyCodeF7 = 0x62,
				kKeyCodeF8 = 0x64,
				kKeyCodeF13 = 0x69,
				kKeyCodeF16 = 0x6A,
				
				kKeyCodeHelp = 0x72,
				kKeyCodeHome = 0x73,
				kKeyCodePageUp = 0x74,
				kKeyCodeDeleteRight = 0x75,
				kKeyCodeEnd = 0x77,
				kKeyCodePageDown = 0x79,
				
				kKeyCodeLeft = 0x7B,
				kKeyCodeRight = 0x7C, 
				kKeyCodeDown = 0x7D,
				kKeyCodeUp = 0x7E,
				
				kKeyCodeNumPadPeriod = 0x41,
				kKeyCodeNumPadStar = 0x43,
				kKeyCodeNumPadPlus = 0x45,
				kKeyCodeNumPadClear = 0x47, 
				kKeyCodeNumPadEnter = 0x4C,
				kKeyCodeNumPadSlash = 0x4B,
				kKeyCodeNumPadMinus = 0x4E,
				kKeyCodeNumPadEqual = 0x51,
				kKeyCodeNumPadZero = 0x52,
				kKeyCodeNumPad1 = 0x53,
				kKeyCodeNumPad2 = 0x54,
				kKeyCodeNumPad3 = 0x55,
				kKeyCodeNumPad4 = 0x56,
				kKeyCodeNumPad5 = 0x57,
				kKeyCodeNumPad6 = 0x58,
				kKeyCodeNumPad7 = 0x59,
				kKeyCodeNumPad8 = 0x5B,
				kKeyCodeNumPad9 = 0x5C
};


///< OpenGL types as standard  ---------------------------------------------------------

enum kNP_NATIVE_DATA_TYPES
{
	kNPnull = 0,

	///<function Pointers
	kNPcmdFuncPtr,
	kNPthreadFuncPtr,

	///< data structures
	kNPvoidPtr,
	kNPdataRef, ///<kNPdata,
	kNPbase,
	kNPmap,
	kNPio,
	KNPctrl,
	kNPglobal,
	kNPnode,
	kNPtag,
	kNPfileBlock,
	kNPlist,

	kNPtagID,
	kNPtagName,
	kNPtagDesc,

	kNPmapItem,

	kNPque,
	kNPqueItem,
	kNPqueCmd,

	kNPcmd,
	kNPnodeData,
	kNPcamera,
	kNPgrid,
	kNPpin,			///< @todo zz remove this, too easily confused with kNodePin
	kNPchMap,		///< @todo remove this, should not be here //zzsql 

	/// fundamental C types
	kNPfloat,
	kNPint,
	kNPbyte,
	kNPbool,
	kNPuint,
	kNPubyte,

//	kNPstring,		///< char* ASCII null terminated cstr


	kNPcharArray, //zzsql ok //zz debug redundant to kNPcstrPtr?

	/// C structures
	kNPboolXYZ,
	kNPboolXYZS,
	kNPintXY,
	kNPintXYZ,
	kNPintXYZS,

	kNPfloatXY,
	kNPfloatXYZ,
	kNPfloatXYZA,
	kNPfloatXYZS,

	kNPubyteRGB,
	kNPubyteRGBA,

	kNPfloatRGB,
	kNPfloatRGBA,

	kNPcstrPtr,
	
	/// GL types
	kNPGLboolean,
	kNPGLubyte,
	kNPGLint,
	kNPGLfloat,

	/// node element types
	kNPid,
	kNPtype,
	kNPdata,

	kNPselected,
	kNPparent,
	kNPbranchLevel,
	kNPchild,

	kNPchildIndex,
	kNPchildCount,

	kNPchInputID,
	kNPchOutputID,
	kNPchLastUpdated,

	kNPaverage,
	kNPinterval,

	kNPauxA,
	kNPauxB,

	kNPcolorShift,

	kNProtateVec,
	kNPscale,
	kNPtranslate,
	kNPtagOffset,

	kNProtateRate,
	kNProtate,
	kNPscaleRate,
	kNPtranslateRate,
	kNPtranslateVec,

	kNPshader,
	kNPgeometry,
	
	kNPlineWidth,
	kNPpointSize,
	kNPratio,
	
	kNPcolorIndex,
	kNPcolor,
	kNPcolorFade,
	kNPtextureID,
	
	kNPhide,
	kNPfreeze,
	
	kNPtopo,
	kNPfacet,

	kNPautoZoom,

	kNPtriggerHi,
	kNPtriggerLo,
	kNPsetHi,
	kNPsetLo,
	
	kNPproximity,
	kNPproximityMode,

	kNPsegments,

	kNPtagMode,
	kNPformatID,
	kNPmapID,
	kNPrecordID,

	///< camera
	kNPformat,
	kNPinterlaced,
	kNPstereo,
	kNPaspectRatio,
	kNPfps,
	kNPcolorSpace,
	kNPwidth,
	kNPheight,
	kNPposition,
	kNPfov,
	kNPclipNear,
	kNPclipFar,
	kNPaperture,
	kNPexposure,
	kNPsensorType,
	kNPoverlay,

	///< pin
	kNPinnerRadius,
	kNPouterRadius,
	kNPradiusRatio,
	kNPscreenSize,
	kNPslices,
	kNPstacks,
	kNPtranslateTex,
	kNPscaleTex,
	kNProtateTex,

	///< grid

	///<tags
	kNPtitle,
	kNPdesc,

	///<chmap
	kNPchannelID,
	kNPtrackID,
	kNPattribute,
	kNPtrackTableID,
	kNPchMapTableID,	

	///< map file
	kNPversion,
	kNPmapTypeCount,
	kNPrecordCount,
	kNPnodeRootCount,

	///< global types
	kNPgNull,
	kNPgAlphaMode,
	kNPfullscreen,
	kNPgFullscreen,		//zz-globals redundant... 

	kNPgBackground,
	kNPgBackgroundR,
	kNPgBackgroundG,
	kNPgBackgroundB,
	kNPgBackgroundA,

	kNPsubsample,

	NPposition,			//zz-osc
	kNPpositionXY,		//zz debug or should this just be kNPposition?
	kNPpositionXYZ,
	kNPwindowSize,
	kNPwindowSizeXY,
	kNPwindowSizeXYZ,

	kNPhudLevel,

	kNPloginNull,
	kNPloginHostIP,
	kNPloginUser,
	kNPloginPassword,
	kNPloginHostType,
	kNPtxIP,
	kNPrxIP,
	kNPtxPort,
	kNPrxPort,
	kNPgBrowserURL,
	kNPgitvizURL,

	kNPmousePickMode,
	kNPmouseCamMode,
	kNPmouseTool,

	kNPitemCount,	///<used at end of lists to determine number of items in list

	kNPsize
};

enum kNP_GEOMETRY_TYPES
{
	///< GL primitives using GLUT and GLU

	///< change to all solid with shader specifying wireframes or solid		//zz debug
	///< kNPgeoPoint = 0,
	///< kNPgeoLine,			///<length 2
	///< kNPgeoTriangle,		///<length 2
	///< kNPgeoQuad,			///<length 2
	///< kNPgeoPentagon,		///<
	///< kNPgeoCircle,		///<radius 1
	kNPgeoCubeWire = 0,
	kNPgeoCube,				///<length 2, currently is something else, what? //zz debug
	kNPgeoSphereWire,
	kNPgeoSphere,			///<radius 1
	kNPgeoConeWire,
	kNPgeoCone,				///<radius 1, height 2

	kNPgeoTorusWire,
	kNPgeoTorus,			///<ratio 0.1, radius 1.5 should we change to 1.0 ?

	kNPgeoDodecahedronWire,
	kNPgeoDodecahedron,		///<size ?
	kNPgeoOctahedronWire,
	kNPgeoOctahedron,		///<size ?
	kNPgeoTetrahedronWire,
	kNPgeoTetrahedron,		///<should have edge length of 2, actual size ?
	kNPgeoIcosahedronWire,
	kNPgeoIcosahedron,		///<should have edge length of 2, actual size ?

	kNPgeoPin,				///<height 5.5, 5 from tip to center of sphere
	kNPgeoPinWire,								///< solid and wire reversed, debug zz

	kNPgeoCylinderWire,
	kNPgeoCylinder,			///<radius 1, height 2

///<	kNPglutWireTeapot,
///<	kNPglutSolidTeapot,

	kNPgeoCount,	///<add primitives, triangle, quad, circle, cylinder

	kNPgeoPlot,
	kNPgeoSurface,
	kNPgeoMesh,
	kNPgeoGrid
};

enum kNP_GL_NORMALS
{	
	kNPglNormalNull = 0,
	kNPglRescaleNormal,
	kNPglNormalize,

	kNPglNormalSize
};

enum kNP_GL_SHADER_MODE
{	
	kNPglShadeNull = 0,
	kNPglShadeSmooth,
	kNPglShadeFlat,

	kNPglShadeSize
};

enum kNP_MOUSE_MODE
{
	kNPmouseModeNull = 0,

	kNPmouseModeCamLook,

	kNPmouseModeCamFlyA,
	kNPmouseModeCamFlyB,

	kNPmouseModeCamExamXZ,
	kNPmouseModeCamExamXY,
	
	kNPmouseModeDragXZ,
	kNPmouseModeDragXY,
	kNPmouseModeDragWorld,

	kNPdragModeSize
};

enum kNP_CENTER_PLOT
{
	kNPcenterNull = 0,
	kNPcenterPerCycle,		///<center on the current data point
	kNPcenterPerGrid		///<center when current point exceeds 1 grid space
};

enum kNP_NODE_ROOT_GROUP
{
	kNPnodeRootNull = 0,
	kNPnodeRootCamera,			///< root node index for all cameras
	kNPnodeRootGrid,			///< root node index for all grids
///<	kNPnodeRootLight,
	kNPnodeRootHUD,
	kNPnodeRootPin				///< first root node for all pins
};

enum kNP_SELECT_FILTER
{
	kNPmodeNull = 0,
///<	kNPmodeCombo,			///< choose a combo of modes
	kNPmodeCamera,				///< pick cameras
	kNPmodeGrid,				///< pick grids
	kNPmodePin,					///< pick pins
///<	kNPmodeHUD,				///< modify HUD element
///<	kNPmodeAll,				///< pick anything
	kNPmodeCount
};

/*! Mouse Tools */
enum kNP_TOOL_TYPE
{
	kNPtoolNull = 0,		///< default behavior based on pickMode
	kNPtoolSelect,			///< L-click picks object, R-click group, drag region
	kNPtoolCreate,			///< L-click creates new node, R-click to delete
	kNPtoolLink,			///< L-click to Select A then Select B, R-click to Cancel (re-select A)

	kNPtoolCombo,			///< Camera move or Pin select, move, rotate, scale and ratio
							///< choose a combo of tools, current default tool
	kNPtoolMove,			///< L-drag Move in XY, R-drag for XZ
	kNPtoolRotate,			///< L-drag Rotate XY, R-drag for Z
	kNPtoolSize,			///< L-drag Scale XYZ axes or R-drag for torus Ratio

//	kNPtoolPick,			///< L-click to Pick an object, R-click for multiple

	kNPtoolTopo,			///< L-click next Topo type, R-click reverses
	kNPtoolGeometry,		///< L-click next Primitive type, R-click reverses
							///< Geometry does not effect Topology
//	kNPtoolSegments,		///< L-Click add or R-Click remove segments

	kNPtoolColor,			///< L-click next Color index, R-click reverses
	kNPtoolTexture,			///< L-click next Topo type, R-click reverses
	kNPtoolHide,			///< L-click Hide branches, R-click Show branches
							///< '~' key to Show ALL, '4' Hide ALL at branch level > 1
	kNPtoolTag,				///< L-click toggles Text Tag, R-click hides Tag

	kNPtoolAlpha,			///< L-Click reduces Alpha color, R-Click increases

	kNPtoolChannel,			///< L-click next Channel, R-click reverses
	kNPtoolFreeze,			///< L-click Freezes node, R-click un-Freezes

	kNPtoolSetpointHi,		///< L-click to Set Upper Limit, R-click clears Setpoint
	kNPtoolSetpointLo,		///< L-click to Set Lower Limit, R-click clears Setpoint

	kNPtoolCount			///< enumeration item count
};

enum kNP_COORD_TYPE
{
	kNPcoord,
	kNPcoordLatLongX,
	kNPcoordScale,
	kNPcoordCount
};

enum kNP_COMPASS_ELEMENT
{
	kNPcompassHeading = 0,
	kNPcompassTilt,
	kNPcompassRoll,
	kNPcompassCoordX,
	kNPcompassCoordY,
	kNPcompassCoordZ,
	kNPcompassCount
};

enum kNP_HUD_ELEMENT
{
	kNPhudNull = 0,
	kNPhudTags,
	kNPhudSelection,
	kNPhudConsole,
	kNPhudFPS,
	kNPhudCompass,
		kNPhudAngle,
	// 	kNPhudTilt,
	// 	kNPhudRoll,
			kNPhudCoordX,	// perhaps just call kNPhudX,			 //zz debug
	// 	kNPhudTranslate
	// 	kNPhudScale
	// 	kNPhudRotate
		kNPhudCoordY,
		kNPhudCoordZ,
		
	kNPhudMode,
	kNPhudTool,
	kNPhudSave,			//zz select
	kNPhudSelect,		//zzf
	kNPhudCreate,
	kNPhudLink,
	kNPhudCombo,
	kNPhudMove,
	kNPhudRotate,
	kNPhudSize,
	kNPhudTopo,
	kNPhudGeo,
	kNPhudColor,
	kNPhudTexture,
	kNPhudHide,
	kNPhudTag,
///<	kNPhudCursor,		//zzf
	kNPhudCount
};


///<concept of default topo is in flux, think in terms of //zz debug
///<cut and paste various bramches, including making a 
///<sub-branch a root pin determined by parent node type
///<may not be worth it and do away with topoNull pins, CSVverOne
enum kNP_TOPOLOGY_TYPE
{
	kNPtopoNull = 0,		///< linear 3D euclidean space, same as grid? //zz

	kNPtopoCube,			///< six facet coord system for each side of cube
	kNPtopoSphere,			///< spherical coords compatible with KML
	kNPtopoTorus,			///< default branchLevel = 1 attached to pin
	kNPtopoCylinder,		///< cylindrical coords

	kNPtopoPin,				///< default root pin shaped as icecream cone
	kNPtopoRod,				///< similar to cylinder and pin
	kNPtopoPoint,			///< spherical coords with no offset or scale
	kNPtopoGrid,			///< no offset, no scale, cartesian coords

	kNPtopoZcube,			///< six facet coord system for each side of cube
	kNPtopoZsphere,			///< spherical coords with z scaling
	kNPtopoZtorus,			///< default branchLevel = 1 attached to pin
	kNPtopoZcylinder,		///< cylindrical coords
	
	kNPtopoCount,

	kNPtopoCone,			///< cylindrical coords
	kNPtopoPlot,			///< 1D, 2D or 3D line plot, GPS, Oscillscope
	kNPtopoSurface,			///< deformable grid, FFT, LIDAR, sound sphere
	kNPtopoMesh				///< 3D mesh model mapped as surface terrain
//	kNPtopoCount
};

/// Flag IO update to grab a screenshot
enum kNP_SCREENSHOT_FLAG
{
	kNPscreenshotOFF = 0,	///< Disabled
	kNPscreenshotFull,		///< Full window
	kNPscreenshotThumb		///< Thumbnail
};

///  Message logs and user feedback are filtered by category
enum kNP_MESSAGE_CAT
{
	kNPmsgNull = 0,
	kNPmsgErr,
	kNPmsgWarn,
	kNPmsgDebug,
	kNPmsgLog,

	kNPmsgMap,
	kNPmsgIO,
	kNPmsgCtrl,

	kNPmsgView,	// kNPmsgConsole
	kNPmsgHint,	// help

	kNPmsgNPE,
	kNPmsgFile,
	kNPmsgGL,
	kNPmsgDB,
	kNPmsgOSC,
	kNPmsgNet,
	kNPmsgCount
};


//zz debug, don't really need this, remove this and its dependencies
///< simplify the funcitonality elsewhere
enum kNP_CONSOLE_LEVEL
{
	kNPconsoleLevelNull = 0,	///< hide console
	kNPconsoleLevelOne,			///< display 1 line
	kNPconsoleLevelThree,		///< display 3 lines
	kNPconsoleLevelMax,			///< display maximum number of lines possible
	kNPconsoleLevelCount
};

enum kNP_CONSOLE_MODE
{
	kNPconsoleMsg = 0,		///< default message mode
	kNPconsoleCmd,
	kNPconsoleTag,				///< set the tag names
	kNPconsoleEdit,				///< code editor
	kNPconsoleMenu,				///< generic menu list	
	kNPconsoleMySQL				///< DB Browser
};

enum kNP_CONSOLE_COMMAND_LINE_FLAG
{
	kNPflagNull = 0,
	kNPflagN,					///< any native parameter, mapItem desc format
	kNPflagCmd,					///< commands
	kNPflagURL,
	kNPflagFile,
	kNPflagUDP,
	kNPflagOSC,
	kNPflagHelp,
	kNPflagVer
};

enum kNP_GLUT_FONT_TYPE
{
	kNP_GLUT_STROKE_ROMAN = 0,
	kNP_GLUT_STROKE_MONO_ROMAN,
//	kNP_GLUT_BITMAP_9_BY_15, ///<debug zz GLUT_BITMAP_9_BY_15 = ((void *)0x0002)
	kNP_GLUT_BITMAP_8_BY_13,
	kNP_GLUT_BITMAP_TIMES_ROMAN_10,
	kNP_GLUT_BITMAP_TIMES_ROMAN_24,
	kNP_GLUT_BITMAP_HELVETICA_10,
	kNP_GLUT_BITMAP_HELVETICA_12,
	kNP_GLUT_BITMAP_HELVETICA_18,
	kNPfontCount
};
# define  kNP_GLUT_BITMAP_9_BY_15      ((void *)0x0002)		//zz debug, workaround

enum kNP_FONT_JUSTIFY
{
	kNPjustifyLeft = 0,
	kNPjustifyRight,
	kNPjustifyCenter
};

enum kNP_FONT_SIZE
{
	kNPfontSizeSmall = 0,
	kNPfontSizeMedium,
	kNPfontSizeLarge
};

enum kNP_QUADRANT
{
	kNPpositionNull = 0,
	kNPpositionLeft,
	kNPpositionRight,
	kNPpositionCenter,			///<refers to either vertical or horizontal middle
	kNPpositionTop,
								///<kNPmiddle,
	kNPpositionBottom,
	
	kNPpositionCount
};

enum kNP_TAG_MODE
{
	kNPtagModeNull = 0,			///< fixed to object in 3D scene, not oriented
	kNPtagModeBoxOutlineHUD,	///< white text on grey box, node colored outline
	kNPtagModeBoxHUD,			///< node colored text on grey box, no outline
///<	kNPtagModeHUD,				///< fixed size font displayed on the HUD, removed as is typically too hard to read, debug zz
///<	kNPtagModeBillboard,		///< text in 3D scene oriented towards cam
	kNPtagModeCount
};


/// File category is useful for choosing which procedure to read data with.
enum kNP_FILE_CATEGORY
{
		kNPfileCatNull = 0,
		kNPfileCatDir,			///< If directory will be of type kNPfileNull
		kNPfileCatBin,
		kNPfileCatCode,
		kNPfileCatImage,
		kNPfileCatTable,
		kNPfileCatText,
		kNPfileCatWeb,
		kNPfileCatModels,
		kNPfileCatAudio,
		kNPfileCatVideo,
		kNPfileCatAV,
		kNPfileCatCount
};

/// File type identification used to assign specific file handlers.
enum kNP_FILE_TYPE
{
	kNPfileNull = 0, ///< default is unknown, so try it

	/// Binary Executable Files
	kNPfileEXE,		///< MSW exececutable
	kNPfileDLL,		///< MSW exececutable
	kNPfileAPP,		///< Posix executable
	kNPfileBAT,		///< MSW BATCH
	kNPfileCOM,		///< MSW COM
	kNPfileDYLIB,	///< Posix library

	/// Code Files
	kNPfileH,		///< Header			
	kNPfileC,		///< C
	kNPfileCPP,		///< C++ ( .cp, .cpp )
	kNPfilePY,		///< Python source code
	kNPfilePYC,		///< Python compiled file ( .pyc .pyo, .pyd ) 

	kNPfileCSV,		///< CSV table formats
	kNPfileTXT,
	kNPfileXML,
	kNPfileJSON,	///< JSON data supports a tree schema

	/// 3D model formats supported by Assimp library.
	 /// Assimp import/export formats.
	kNPfileDAE,		///< Collada 3D model inter-change
	kNPfileOBJ,		///< Wavefront Object
	kNPfileSTL,		///< Stereolithography
	kNPfilePLY,		///< Stanford Polygon Library 
	 /// Assimp common interchange formats, import only.
	kNPfileFBX,		///< Autodesk
	kNPfileGLTF,	///< glTF ( .gltf, .glb )
	kNPfileBLEND,	///< Blender 3D
	kNPfile3DS,		///< 3D Studio (Max)
	kNPfileASE,		///< 3ds Max ASE
	kNPfileIFC,		///< Industry Foundation Classes (IFC/Step)
	kNPfileXGL,		///< XGL ( .xgl,.zgl )
	kNPfileDXF,		///< AutoCAD DXF
	kNPfileLWO,		///< LightWave
	kNPfileLWS,		///< LightWave Scene
	kNPfileLXO,		///< Modo
	kNPfileX,		///< DirectX X
	kNPfileAC,		///< AC3D
	kNPfileMS3D,	///< Milkshape 3D
	kNPfileCOB,		///< TrueSpace
	 /// Assimp MOTION CAPTURE FORMATS
	kNPfileBVH,		///< Biovision
	kNPfileCSM,		///< CharacterStudio Motion
	 /// Assimp GRAPHICS ENGINE FORMATS
	kNPfileOXML,	///< Ogre XML ( .xml )	/// @todo how to identify Ogre XML.?
	kNPfileIRRMESH,	///< Irrlicht Mesh
	kNPfileIRR,		///< Irrlicht Scene
	 /// Assimp - GAME FILE FORMATS
	kNPfileMDL,		///< Quake I
	kNPfileMD2,		///< Quake II
	kNPfileMD3,		///< Quake III Mesh
	kNPfilePK3,		///< Quake III Map/BSP
	kNPfileMDC,		///< Return to Castle Wolfenstein
	kNPfileMD5,		///< Doom 3
	kNPfileVTA,		///< Valve Model ( .smd, .vta )
	kNPfileOGEX,	///< Open Game Engine Exchange
	kNPfile3D,		///< Unreal
	 /// Assimp - Other 3D model formats.
	kNPfileB3D,		///< BlitzBasic 3D
	kNPfileQ3D,		///< Quick3D ( .q3s, q3d )
	kNPfileNFF,		///< Neutral File Format
	kNPfileS8NFF,	///< Sense8 WorldToolKit	/// @todo nff vs nff
	kNPfileOFF,		///< Object File Format
	kNPfilePRAW,	///< PovRAY Raw
	kNPfileTER,		///< Terragen Terrain
	kNPfile3MDL,	///< 3D GameStudio (3DGS) ( .mdl )
	kNPfileHMP,		///< 3D GameStudio (3DGS) Terrain
	kNPfileNDO,		///< Izware Nendo

	/// Other 3D model formats (non-Assimp).
	kNPfileKML,		///< GIS 3D standard

	/// Audio formats.
	kNPfileAIFF,	///< UNIX audio
	kNPfileWAV,		///< MSW audio
	kNPfileMP3,		///< MPEG3 audio
	kNPfileM4A,		///< MPEG4 audio

	/// Image formats (supported by FreeImage).
	kNPfileBMP,		///< 
	kNPfileCUT,		///< Dr. Halo CUT
	kNPfileDDS,		///< Native GPU compressed image format 
	kNPfileEXR,		///< 
	kNPfileFAX,		///< FAXG3
	kNPfileGIF, 	///< 
	kNPfileHDR,		///< 
	kNPfileICO,		///< 
	kNPfileIFF,		///< 
	kNPfileJ2K,		///< JPEG 2000
	kNPfileJP2,		///< 
	kNPfileJPG,		///< JPEG / JIF ( .jpg, .jpeg )
	kNPfileJNG,		///<
	kNPfileJXR,		///< JPEG-XR ( .ms-photo )
	kNPfileKOA,		///< KOALA
	kNPfileLBM,		///<
	kNPfileMNG,		///< 
	kNPfilePBM,		///< ( .pbm, .pgm, .ppm )
	kNPfilePBMRAW,	///<  RAW
	kNPfilePCD,		///< 
	kNPfilePCX,		///< 
	kNPfilePFM,		///< 
	kNPfilePICT,	///< Macintosh PICT
	kNPfilePGM,		///< 
	kNPfilePGMRAW,	///<
	kNPfilePNG,		///<
	kNPfilePPM,		///<
	kNPfilePPMRAW,	///<	
	kNPfilePSD,		///< Photoshop PSD
	kNPfileRAS,		///< Sun RAS
	kNPfileRAW, 	///< RAW camera
	kNPfileSGI,		///<
	kNPfileTGA,		///< Targa
	kNPfileTIFF,	///< 
	kNPfileWBMP,	///< 
	kNPfileWEBP,	///< 
	kNPfileXBM,		///< 
	kNPfileXPM,		///<

	/// AV formats.
	kNPfileMJ2,		///< Motion JPEG 2000
	kNPfileDCP,		///< Digital Cinema Package (DCI)
	kNPfileMPG,		///< MPEG 1 and 2 video
	kNPfileMP4,		///< MPEG 4
	kNPfileMXF,		///< Both open standard and proprietary
	kNPfileDNX,		///< Avid
	kNPfileAVI,		///< Windows movie
	kNPfileMOV		///< Quicktime movie
};

/// Data format is the in memory format, not the file type
///< kNPmapOSC vs kNPformatOSC ...formatJSON ...DB ..SNMP
///< kNPmapOSC specifies an OSC map, A-B map pairs for mapping data and commands
///< kNPformatOSC specifies an OSC formatted bitstream
enum kNP_DATA_FORMAT
{
	kNPformatNull = 0,
	kNPformatCSV,			///< primary native file format
	kNPformatDB,			///< native DB format is MySQL
	kNPformatOSC,			///< primary network protocol
	kNPformatJSON,			///< likely the most descriptive
	kNPformatSNMP,			///< can bridge SNMP-JSON-OSC
	kNPformatXML,			///< for 3rd party data support
	kNPformatKML,			///< google KML, related to COLLADA
/*
	kNPformatBMP,
	kNPformatTGA,
	kNPformatDDS,
	kNPformatJPG,			///< perhaps should be in separate list... //zz debug
	kNPformatJ2K,			///<
	kNPformatPNG,			///<
	kNPformatGIF,			///<

	kNPformatAIFF,			///<
	kNPformatWAV,
*/
	kNPformatCount
};

//-----------------------------------------------------------------------------
void npMapTypesInit (void* dataRef);		///<
void npInitChMapTypes (void* dataRef);

#endif

