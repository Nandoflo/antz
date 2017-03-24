/* -----------------------------------------------------------------------------
*
*  npgldraw.c
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

#include "npgldraw.h"

#include "../../npdata.h"
#include "../../npctrl.h"
#include "../../os/npos.h"
#include "../npglut.h"

#include "npgeometry.h"
#include "nptags.h"

/// @todo add Legend as a hybrid 2D/3D HUD element


NPfloatXYZ npProjectWorldToScreen (const pNPfloatXYZ offset);

void DrawDefault (pNPnode node);
void DrawCamera (pNPnode node);
void DrawSurface (pNPnode node);
void DrawPoints (pNPnode node);
void DrawVideo (pNPnode node);

void DrawGrid (pNPnode node, void* dataRef);
void DrawPin (int selected, pNPnode node, void* dataRef);
void DrawPinChild (pNPnode node, void* dataRef);

void npDrawLink (pNPnode node, void* dataRef);
void npDrawLinkQue (void* dataRef);
void npClearLinkQue (void* dataRef);
void npAddLinkQue (pNPnode node, void* dataRef);

void npDrawTags (void* dataRef);

void npDrawSort(void** sort, int count, void* dataRef);
void npDrawSortThread(void* dataRef);

//------------------------------------------------------------------------------
void npInitGLDraw(void* dataRef)
{
	pData data = (pData) dataRef;

//	InitTorus (dataRef);

	nposBeginThread (npDrawSortThread, data);

	data->io.gl.pickPass = false;			//zz global init
}


//------------------------------------------------------------------------------
void DrawDefault (pNPnode node)
{
	return;
}


// draw an icon at the location of the camera in the scene
//------------------------------------------------------------------------------
void DrawCamera (pNPnode node)
{
	// draw icon of camera in the scene, but not the current viewport camera
	return;
}


//------------------------------------------------------------------------------
void DrawSurface (pNPnode node)
{
	// draw ground
	glPushMatrix();

	// position the node
	glTranslatef (node->translate.x, node->translate.y, node->translate.z);
	glRotatef (node->rotate.y, 0.0f, 0.0f, 1.0f);
	glRotatef (node->rotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef (node->rotate.z, 0.0f, 0.0f, 1.0f);

	glScalef (node->scale.x, node->scale.y, node->scale.z);

	glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

	glBegin(GL_QUADS);
		glVertex3f (-100.0f, -100.0f, 0.0f);
		glVertex3f (-100.0f,  100.0f, 0.0f);
		glVertex3f ( 100.0f,  100.0f, 0.0f);
		glVertex3f ( 100.0f, -100.0f, 0.0f);
	glEnd();

	glPopMatrix();
}


//------------------------------------------------------------------------------
void DrawPoints (pNPnode node)
{
	return;
}


//MB//
// converts the ID value to an RGB value for picking objects by color
//------------------------------------------------------------------------------
void npIDtoRGB( int id, int* r, int* g, int* b )
{
	int remain = id;
	
	*r = remain / (256*256);	remain = remain - *r * (256*256);
	*g = remain / 256;			remain = remain - *g * 256;
	*b = remain;
}

//convert RGB pixel value back to the encoded node ID
int npRGBtoID( int r, int g, int b )
{
	int id;
	id = (256*256)*r + 256*g + b;

	return id;
}
//MB-End//


//calculate screen position and add to tags draw list if on-screen
//------------------------------------------------------------------------------
void npAddTagToDraw (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtags tags = &data->io.gl.hud.tags;

	//get screen coordinates
//	node->screen = npProjectWorldToScreen (&node->tagOffset);

	//if true then exit, the node is behind the camera
	if (node->screen.z < 0.0f || node->screen.z > 1.0f)
		return;

	//exit if tag is not on-screen
	if ( node->screen.x < 0 || node->screen.x > data->io.gl.width
		|| node->screen.y < 0 || node->screen.y > data->io.gl.height )
		return;

	//add to tags draw list
	if( tags->count < kNPtagDrawMax )
		tags->list[tags->count++] = node;
}


//------------------------------------------------------------------------------
void DrawPin (int selectedRootNode, pNPnode node, void* dataRef)
{
	int i = 0;
	int idRed = 0, idGrn = 0, idBlu = 0;
	NPfloatXYZ offset = {0.0f,0.0f,0.0f};
	GLfloat modelView[16];	//zz-link  //zzhp can comment out if links not used

	pData data = (pData) dataRef;
	pNPnode nodeChild = data->map.currentNode;
	pNPnode rootGrid = data->map.node[kNPnodeRootGrid];
	NPcameraPtr camData = data->map.currentCam->data;  // for world coordinates

	//if pickPass render scene with node ID encoded as color	//MB//
	if( data->io.gl.pickPass )
	{							// Convert nodeID to RGB triplet range is 2^24
		npIDtoRGB( node->id, &idRed, &idGrn, &idBlu );
		glColor4ub( idRed, idGrn, idBlu, 255 );
	}
	else
		glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);


	glPushMatrix();

	// position the node and scale the z position based on root grid scale
	glTranslatef(	node->translate.x * rootGrid->scale.x,
					node->translate.y * rootGrid->scale.y, 
					node->translate.z * rootGrid->scale.z);	//global grid height

	// rotate to node orientation
	glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);		//tlit
	glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);		//roll
	glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);		//heading

	glLineWidth (node->lineWidth);	//zz move this out to npglDrawGeo

	// rod topo has unique scaling and offset
	if (!node->hide)
	{
		if ( node->topo == kNPtopoRod )
		{					//width uses ratio, length uses scale 2 * 5 = 10
			glPushMatrix();
				glScalef (	node->ratio * 2.0f,
							node->ratio * 2.0f,
							node->scale.z * kNPoffsetRod * 0.5f );
				glTranslatef (0.0f, 0.0f, kNPoffsetUnit);
				npGLSurface (true, node, data);
			glPopMatrix();
		}
		else
		{
			glScalef (node->scale.x, node->scale.y, node->scale.z);
			npGLSurface (true, node, data);
		}
	}

	/// calculate screen coords for drawing tags and depth z-sort
	node->screen = npProjectWorldToScreen( &offset );

	//if tagMode then calculate screen position and add to tags draw list
	if (node->tagMode)
		npAddTagToDraw (node, data);

	// topo ground level offsets removed via user request, ie: kNPoffsetPin
	// check pre 2015 ver for methods (commented out)

	//perhaps add logic to only calculate origin coordinates when needed, zz debug
	//typically only need if cam target or connected by a link topo

	//zzhp root local translate coordinates = world coordinates
	//does not properly compute root nodes rotated about x axis,	//zz debug

	//update the world coordinates of the node
	//perhaps add logic to only calculate origin coordinates when needed,	 zz debug
	//typically only need if cam target or connected by a link topo
	if (node->topo == kNPtopoPin)
	{
		glPushMatrix();
			glTranslatef (0.0f, 0.0f, kNPoffsetPin);// * node->scale.z);	//inverted position with negative scale, zz debug
			glGetFloatv (GL_MODELVIEW_MATRIX, modelView);				//for some reason the rod does not have same issue
			npLocalToWorld (&node->world, camData->inverseMatrix, modelView);
		glPopMatrix();
	}
	else if (node->topo == kNPtopoRod)
	{
		glPushMatrix();
			glTranslatef (0.0f, 0.0f, kNPoffsetRod);// * node->scale.z);	//zz debug bug 115
			glGetFloatv (GL_MODELVIEW_MATRIX, modelView);
			npLocalToWorld (&node->world, camData->inverseMatrix,  modelView);
		glPopMatrix();
	}
	else
	{
		//node->world = node->translate;
		node->world.x = node->translate.x * rootGrid->scale.x;
		node->world.y = node->translate.y * rootGrid->scale.y;
		node->world.z = node->translate.z * rootGrid->scale.z;
	}

	//draw selection outline as wireframes
	if (data->io.mouse.pickMode == kNPmodePin
		&& (node->selected || selectedRootNode || data->map.selectedPinIndex ))
	{
		glLineWidth(1.0f);

		glPushMatrix();

		if (selectedRootNode)
		{
			// slide down a little extra for scaling up to surround the pin
			if (node->topo == kNPtopoRod)	
			{
				glScalef (1.15f, 1.15f, (kNPoffsetRod/kNPoffsetPin) * node->scale.z); //rod is twice pin height	
			}
			else
			{
				glTranslatef (0.0f, 0.0f, -0.1f); //- kNPoffsetPin );	//zzoff	
				glScalef (1.15f, 1.15f, 1.15f);
			}

			if ( !data->io.gl.pickPass )
			{
				if (node->selected)
					glColor4f (1.0f, 1.0f, 0.0f, 0.8f);	// draw selected yellow
				else if (node == data->map.currentNode)
						glColor4f ( 1.0f, 0.0f, 0.0f, 1.0f);	// current node red
				else
					glColor4f ( 0.5f, 0.5f, 0.5f, 0.7f);	// grey
			}
			npGLPrimitive (kNPgeoPinWire, 0.0f);	//draw surrounding outline

			if (!data->io.gl.pickPass)
			{
				if (nodeChild->selected)									//zz debug
					glColor4f (1.0f, 1.0f, 0.0f, 0.8f);	// draw selected yellow
				else
					glColor4f (1.0f, 0.0f, 0.0f, 1.0f);	// current node red
			}

			if (node->topo == kNPtopoRod)	
			{
				glTranslatef (0.0f, 0.0f, kNPoffsetPin);
				glScalef (1.0f, 1.0f, 0.66666667f / (2.0f * node->scale.z));	// squish z height
			}
			else
				glScalef (1.0f, 1.0f, 0.66666667f);			// squish z height

			npGLPrimitive (kNPgeoPinWire, 0.0f);		// 2nd one is above node
		}
		else if (node->selected)
		{
			if( !data->io.gl.pickPass )
				glColor4f (1.0f, 1.0f, 0.0f, 0.8f);		// draw selected as yellow

			if (node->topo != kNPtopoRod)	
				glTranslatef (0.0f, 0.0f, -0.1f);//- kNPoffsetPin );	//zzoff	
			glScalef (1.05f, 1.05f, 1.05f);
			npGLPrimitive (kNPgeoPinWire, 0.0f);
		}

		glPopMatrix();
	}

	//should call DrawNode, that calls draw pin, move this to drawnodes...   debug, zz
	// recursively
	for (i=0; i < node->childCount; i++)
		DrawPinChild (node->child[i], dataRef);

	glPopMatrix();
}

// first draw child here, then break it up, draw nodes should iterate down, debug, zz
// the tree branches and take care of selected glows, etc...
// this routine should be generic regardless of branch level
//------------------------------------------------------------------------------
#define kNPinvertDeg 0.005555555555f	//equals 1/180

void DrawPinChild (pNPnode node, void* dataRef)
{
	int i = 0;
	int idRed = 0, idGrn = 0, idBlu = 0;

	//zzhp can comment out the modelview if links not used
	GLfloat modelView[16];													//zz-link

	pData data = (pData) dataRef;

	NPcameraPtr camData = data->map.currentCam->data;						//zz-link
	pNPnode parent = node->parent;
	NPpinPtr nodeData = (NPpinPtr) node->data;


	if (parent == NULL)									//take this out		debug zz	
		return;

	//link nodes drawn separately from pins after both ends rendered
	if (node->type == kNodeLink)
	{
		npAddLinkQue (node, dataRef);			//drawn after both endpoint nodes
		return;
	}

	glPushMatrix();

	//node ID is encoded on the Pixel! In RGB Triplet (Range: 0-16.7 Million)
	if( data->io.gl.pickPass )
	{
		npIDtoRGB( node->id, &idRed, &idGrn, &idBlu );
		glColor4ub( idRed, idGrn, idBlu, 255 );
	}
	else
		glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

	// position based on parent topo type
	if (parent->topo == kNPtopoGrid)
	{
		// topo grid type has cartesian coordinates, does not effect child
		glTranslatef (node->translate.x, node->translate.y, node->translate.z);

		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

		if (node->topo != kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (node->scale.x, node->scale.y, node->scale.z);	//node scale
	}
	else if (parent->topo == kNPtopoCube || parent->topo == kNPtopoZcube)
	{
		//update to use parent->childIndex, debug zz
		/// if facet = 0 then calculate using id/6 + 1, fixes issue #227
		if (node->facet == 0)
			node->facet = node->id - 6 * (node->id / 6) + 1;	// +1 to start at 1

		// rotate orthogonal to face based on facet then translate
		switch (node->facet)
		{
			case 1 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//pos x	front
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 2 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//neg x	back
				glRotatef (180.0f, 1.0f, 0.0f, 0.0f);
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 3 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//pos y	right
				glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 4 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//neg y	left
				glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 5 : 									//pos z top, no rotation
			//	glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 6 :
			//	glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);
				glRotatef (180.0f, 0.0f, 1.0f, 0.0f);	//neg z	bottom
				break;
			default : break;
		}

		/// Do not offset to surface if kNPtopoZ... type
		if( parent->topo != kNPtopoZcube )
			glTranslatef (0.0f, 0.0f, kNPoffsetCube);
		
		glScalef (kNPinvertDeg * kNPoffsetCube, kNPinvertDeg * kNPoffsetCube, 
					kNPinvertDeg * kNPoffsetCube); //scale face +/- 180 deg
		
		//now position node specific coordinates
		glTranslatef (node->translate.x, node->translate.y, node->translate.z);

		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
	//	glRotatef (node->rotate.z * kRADtoDEG, 0.0f, 0.0f, 1.0f);

		//scale node up to half parent size
		glScalef (90.0f / kNPoffsetCube, 90.0f / kNPoffsetCube, 90.0f / kNPoffsetCube);
		
		if (node->topo != kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (node->scale.x, node->scale.y, node->scale.z);	//node scale
	}
	else if( parent->topo == kNPtopoSphere 
			|| parent->topo == kNPtopoZsphere
			|| parent->topo == kNPtopoPoint )
	{
	//	glRotatef (node->rotate.y * kRADtoDEG, 0.0f, 0.0f, 1.0f);	//CSV ver 1 debug zz
		//position
		glRotatef (node->translate.x + 180.0f, 0.0f, 0.0f, 1.0f);	//longitude, +180 so N = 0
		glRotatef (node->translate.y - 90.f, -1.0f, 0.0f, 0.0f);	//latitude

		//translate 1 unit to surface for sphere, convert node z local units
		if( parent->topo == kNPtopoPoint )
			glTranslatef ( 0.0f, 0.0f, node->translate.z); //point has no offset
		else if( parent->topo == kNPtopoSphere )
			glTranslatef ( 0.0f, 0.0f, 1.0f + node->translate.z / (4.0f*kRADtoDEG));
		else //kNPtopoZsphere
			glTranslatef ( 0.0f, 0.0f, node->translate.z / (4.0f*kRADtoDEG));

		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

		//scale node * ratio based on topo type
		if (parent->topo == kNPtopoPoint)
			glScalef (node->scale.x, node->scale.y, node->scale.z);
		else if (node->topo == kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (0.25f, 0.25f, 0.25f);	//scale 1/4 size of parent
		else
			glScalef (node->scale.x * 0.25f, node->scale.y * 0.25f, 
							node->scale.z * 0.25f);	//node scale
	//		glScalef (node->scale.x * 0.3536f, node->scale.y * 0.3536f,
	//					node->scale.z * 0.3536f);	//node scale
	}
	else if( parent->topo == kNPtopoCylinder || parent->topo == kNPtopoZcylinder )
	{
		//position object
		glRotatef (node->translate.x + 180.0f, 0.0f, 0.0f, 1.0f);	//longitude, +180 so N = 0
		glRotatef (90.0f, 1.0f, 0.0f, 0.0f);	//orient perpendicular to cyl
		
		//latitude plus translate 1 unit to surface adding converted node z local units
		/// Do not offset to surface if kNPtopoZ... type
		if( parent->topo == kNPtopoZcylinder )
			glTranslatef (0.0, node->translate.y / 90.0f,
						node->translate.z / (4.0f*kRADtoDEG) );
		else
			glTranslatef (0.0, node->translate.y / 90.0f,
						1.0f + node->translate.z / (4.0f*kRADtoDEG) );
		
		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

		//scale 1/4 size of parent
		if (node->topo == kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (0.25f, 0.25f, 0.25f);
		else
			glScalef (node->scale.x * 0.25f, node->scale.y * 0.25f, node->scale.z * 0.25f);			//node scale
	}
	else if ( parent->topo == kNPtopoPin || parent->topo == kNPtopoRod )
	{
		//position the node vertically on the pin, note using translate.x
		if (node->branchLevel == 1 && parent->topo != kNPtopoRod)
									 //zzoff
			glTranslatef (0.0f, 0.0f, kNPoffsetPin + node->translate.x / 37.22f);	//root pin workaround, zz debug
		else if (parent->topo == kNPtopoRod)	//replace kNPoffsetPin with
			glTranslatef (0.0f, 0.0f, 
				parent->scale.z * (kNPoffsetRod + 2.0f * node->translate.x / 37.22f) );
		else
			glTranslatef (0.0f, 0.0f, kNPoffsetPin + node->translate.x / 37.22f);

		if ( node->topo == kNPtopoPin && parent->topo == kNPtopoPin )
			glScalef (node->scale.x * 0.5f, node->scale.y * 0.5f, node->scale.z * 0.5f);
		else if (node->topo != kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (node->scale.x, node->scale.y, node->scale.z);	//rod does not scale child
	
		//offset from center after scaling, preserves ring offset when scaling
		glTranslatef (node->translate.z * .008333333f, //kRADtoDEG,	//scale 180 to ground
					 0.0f, 0.0f); // node->translate.y / -37.22f, 0.0f); //locked y, makes more sense when swtiching topos

		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
	}
	else if( node->branchLevel > 1								 //zz debug remove? all pinchild should be > 1
			|| parent->topo == kNPtopoTorus
			|| parent->topo == kNPtopoZtorus )
	{
		//scale up 1.5X to match torus main center to inner tube center
		glScalef( kNPtorusRadius, kNPtorusRadius, kNPtorusRadius ); 
		
		//position at torus outer radius, inside center of tube
		glRotatef( node->translate.x + 90.0f, 0.0f, 0.0f, 1.0f );
//		glRotatef (node->translate.x, 0.0f, 0.0f, 1.0f);		//longitude
		glTranslatef (1.0f, 0.0f, 0.0f);	//translate to center of tube

		//?
//		glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
		
		//treat null as torus, later make this depend on the geometry
		if (node->topo == kNPtopoTorus )
		{
			glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef (node->translate.y, 0.0f, 0.0f, 1.0f);	//latitude
			//translate.z based on scale.x and is converted from deg 57.29...
			glTranslatef (0.0f, 0.1591f * node->scale.x * node->translate.z 
						  / kRADtoDEG, 0.0f);
			if(parent->topo == kNPtopoZtorus)
				glScalef (0.5f, 0.5f, 0.5f);	//results in 1/4 parent
		}
		else
		{	
			glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		
			glRotatef (node->translate.y, 0.0f, -1.0f, 0.0f);	//latitude

			glRotatef (90.0f, 0.0f, 0.0f, 1.0f);				//orient North
			//translate 1 unit to surface then convert node z local units
			//uses parent->ratio for torus inner radius and 1.5f for scaling factor
			
			if( parent->topo == kNPtopoZtorus )
				glTranslatef ( 0.0f, 0.0f,
								(node->translate.z / (k2PI * kRADtoDEG)) );
			else
				glTranslatef ( 0.0f, 0.0f,
					parent->ratio + (node->translate.z / (k2PI * kRADtoDEG)) );

			glScalef (0.5f, 0.5f, 0.5f);	//results in 1/4 parent
		}
		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

		//scale 1/2 size of parent, smaller is good for torus .5 / 1.5
		if (node->topo == kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (0.33333333f, 0.33333333f, 0.33333333f);
		else
			glScalef (node->scale.x * 0.33333333f, node->scale.y * 0.33333333f,
					node->scale.z * 0.33333333f);	//node scale
	}
	else //assume primary torus with parent as pin
	{
		if (node->branchLevel == 1)
		{
			if (parent->topo == kNPtopoPin || parent->topo == kNPtopoRod )
			{	
				glTranslatef (0.0f, 0.0f, node->translate.x / 37.22f);
				if (node->topo == kNPtopoPin)
					glScalef (0.5f, 0.5f, 0.5f);
			}
			else if (parent->topo == kNPtopoTorus)
			{
				glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
				//translate.z based on scale.x and is converted from deg 57.29...
				glTranslatef (0.3722f * node->scale.x * node->translate.z 
							  / kRADtoDEG, 0.0f, 0.0f);
				glRotatef (node->translate.y, 0.0f, 0.0f, 1.0f);	//latitude
			}
			else
				glTranslatef (node->translate.x * node->scale.x,
						node->translate.y * node->scale.y,
						node->translate.z );				// z is not scaled
		}
		else	
			glTranslatef (node->translate.x * node->scale.x,
						node->translate.y * node->scale.y,
						node->translate.z );				// z is not scaled
		
		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
		
		if (node->topo != kNPtopoRod)		 //rod is scaled at draw time and does not pass along scale to children
			glScalef (node->scale.x, node->scale.y, node->scale.z);	//root child
	}

	//offset height to keep object at the surface, several exceptions...
	if(    parent->topo != kNPtopoPin 
		&& parent->topo != kNPtopoRod
		&& parent->topo != kNPtopoPoint 
		&& parent->topo != kNPtopoZcube 
		&& parent->topo != kNPtopoZsphere
		&& parent->topo != kNPtopoZtorus
		&& parent->topo != kNPtopoZcylinder )
	{
		if ( node->topo == kNPtopoCube )
			glTranslatef (0.0f, 0.0f, kNPoffsetCube);
		else if ( node->topo == kNPtopoTorus )
		{
			if ( parent->topo != kNPtopoTorus )
				glTranslatef (0.0f, 0.0f, kNPoffsetTorus);
		}
		else if (	node->topo != kNPtopoPin
				 && node->topo != kNPtopoRod
				 && node->topo != kNPtopoGrid )
			glTranslatef (0.0f, 0.0f, kNPoffsetUnit);
	}

	//update the world coordinates of the node
	//perhaps add logic to only calculate origin coordinates when needed,	 zz debug
	//typically only need if cam target or connected by a link topo
	if (node->topo == kNPtopoPin)
	{
		glPushMatrix();
			glTranslatef (0.0f, 0.0f, kNPoffsetPin);// * node->scale.z);	//inverted position with negative scale, zz debug
			glGetFloatv (GL_MODELVIEW_MATRIX, modelView);				//for some reason the rod does not have same issue
			npLocalToWorld (&node->world, camData->inverseMatrix, modelView);
		glPopMatrix();
	}
	else if (node->topo == kNPtopoRod)
	{
		glPushMatrix();
			glTranslatef (0.0f, 0.0f, kNPoffsetRod);// * node->scale.z);	//zz debug bug 115
			glGetFloatv (GL_MODELVIEW_MATRIX, modelView);
			npLocalToWorld (&node->world, camData->inverseMatrix,  modelView);
		glPopMatrix();
	}
	else
	{
		//zzhp can comment out the modelview if links not used
		glGetFloatv (GL_MODELVIEW_MATRIX, modelView);
		npLocalToWorld (&node->world, camData->inverseMatrix,  modelView);
	}

	// draw the node unless hidden
	if (!node->hide)
	{
		glLineWidth(node->lineWidth);

		if ( node->topo == kNPtopoPin
			&& !(node->geometry == kNPgeoPin || node->geometry == kNPgeoPinWire) )
		{
			glTranslatef (0.0f, 0.0f, kNPoffsetPin);	/// @todo change primitive to not need offset
			npGLSurface (true, node, data);
			glTranslatef (0.0f, 0.0f, -kNPoffsetPin);
		}
		else if ( node->topo == kNPtopoRod )
		{					//width uses ratio, length uses scale 2 * 5 = 10
			glPushMatrix();
				glScalef (node->ratio * 2.0f, node->ratio * 2.0f, 
							node->scale.z * kNPoffsetPin);
				glTranslatef (0.0f, 0.0f, kNPoffsetUnit);
				npGLSurface (true, node, data);
			glPopMatrix();
		}
		else
			npGLSurface (true, node, data);	//default for all other topo types

		glLineWidth(1.0f);
	}

	/// calculate screen coordinates
	node->screen = npProjectWorldToScreen (&node->tagOffset);

	//if tagMode then calculate screen position and add to tags draw list
	if (node->tagMode)
		npAddTagToDraw(node, data);

	/// @todo update to not pick hidden nodes
	// draw a selection status wireframes slightly scaled up on top...
	if (data->io.mouse.pickMode == kNPmodePin)
	{
		if (node->selected)
		{	
			glPushMatrix();
				// draw nodeID color on pickPass to allow picking on the wireframe
				if( !data->io.gl.pickPass )
					glColor4f (1.0f, 1.0f, 0.0f, 0.8f);			//draw yellow
				glScalef (1.03f, 1.03f, 1.03f);
				npGLPrimitive (kNPgeoTorusWire, node->ratio);
			glPopMatrix();
		}

		// draw a wireframe slightly scaled up on top...
		if (node == data->map.selectedPinNode)
		{	
			glPushMatrix();
				// draw nodeID color on pickPass to allow picking on the wireframe
				if( !data->io.gl.pickPass )
				{
					if (node->freeze)
						glColor4f (0.0f, 1.0f, 1.0f, 0.6f);			//draw cyan
					else
						glColor4f (1.0f, 0.0f, 0.0f, 1.0f);		//draw red
				}
				glScalef (1.04f, 1.04f, 1.04f);
				npGLPrimitive (kNPgeoTorusWire, node->ratio);
			glPopMatrix();
		}
	}

	// draw the child nodes recursively
	for (i=0; i < node->childCount; i++)
		DrawPinChild (node->child[i], data);
		
	glPopMatrix();
}

/// @todo Implement parallel VBO procedures for performance
/// perhaps move some of this from CPU to GPU
// update physics 
// calculate node matrix for transform, world and screen coords
// cull objects outside the view frustrum
// calculate volumetric/ray picking
// z-sort visible objects, apply depth based (fade in/out) sub-sampling
// split z-sort list into (4) tasks for each GPU 
// draw L/R 3D scene with 2D overlay for GLSL FX and text tags

//------------------------------------------------------------------------------
void npDrawNode (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode parent = node->parent;

	int i = 0;
	int idRed = 0, idGrn = 0, idBlu = 0;


	//calculate the unit vector world coords for camera auto-centering


	//if pickPass then set the ID color, otherwise use node color
	if (data->io.gl.pickPass)
	{	
		npIDtoRGB (node->id, &idRed, &idGrn, &idBlu);
		glColor4ub (idRed, idGrn, idBlu, 255);		//set color mapped as id
	}
	else
		glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

	glPushMatrix();

	glTranslatef (parent->world.x, parent->world.y, parent->world.z);

	//orient glRotatef

	//scale the width using ratio and length is half the distance
	//glScalef (node->ratio, node->ratio, distance * 0.5f);	//cyl is 2 units

	//offset to the end of the cylinder instead of its center
	glTranslatef (0.0f, 0.0f, kNPoffsetUnit);

	// calculate screen position for tags and z-sort
	node->screen = npProjectWorldToScreen (&node->tagOffset);

	//don't draw if node hidden
	if( node->hide )
	{
		glPopMatrix();
		return;
	}

	//draw the rod, default geometry = kNPgeoCylinder
	npGLSurface (true, node, data);

	if (node->tagMode)
		npAddTagToDraw(node, data);

	glScalef (1.1f, 1.1f, 1.4142f);	//z scaled for cube to be 2 units

	if (node->selected)
	{
		glColor4ub (255, 255, 0, 204);				//yellow if selected
		npGLPrimitive (kNPgeoCubeWire, 0.0f);
	}

	if (node == data->map.currentNode)
	{
		glColor4ub (255, 0, 0, 255);				//red if active
		glRotatef (22.0f, 0.0f, 0.0f, 1.0f);		//offset from selected
		npGLPrimitive (kNPgeoCubeWire, 0.0f);
	}

	glPopMatrix();
	
}
#define kNPerr7422 "err 7422 - link parent missing"		///< Links have to parents A&B
void npErr( int err, void*dataRef);
void npErr( int err, void*dataRef)
{
	npPostMsg( kNPerr7422, kNPmsgErr, dataRef );
}

// first draw child here, then break it up, draw nodes should iterate down, debug, zz
// the tree branches and take care of selected glows, etc...
// this routine should be generic regardless of branch level
//------------------------------------------------------------------------------
#define kNPinvertDeg 0.005555555555f	//equals 1/180

void npDrawLink (pNPnode node, void* dataRef)
{
	int i = 0;
	int idRed = 0, idGrn = 0, idBlu = 0;

	GLfloat distance = 0.0f;
	GLfloat rotZ = 0.0f;
	GLfloat rotX = 0.0f;
	NPfloatXYZ vec;
	NPfloatXYZ unitVec;
	

	pData data = (pData) dataRef;

	pNPnode parent = node->parent;		//link A
	pNPnode child = node->child[0];		//link B

	/// links have to have both parents A&B
	if( parent == NULL || child == NULL ){ npErr( 7422, data);  return;}

	/// don't draw if node or ends hidden and we don't need the world coords
	if (node->hide || parent->hide || child->hide) return;

	vec.x = parent->world.x - child->world.x;
	vec.y = parent->world.y - child->world.y;
	vec.z = parent->world.z - child->world.z;
	
												//optimize to use a lookup table, zz debug
	distance = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	/// calculate the unit vector
	unitVec.x = vec.x / distance;
	unitVec.y = vec.y / distance;	
	unitVec.z = vec.z / distance;

	/// provide the node coords for camera auto-centering
	node->world.x = parent->world.x - vec.x * 0.5f;
	node->world.y = parent->world.y - vec.y * 0.5f;
	node->world.z = parent->world.z - vec.z * 0.5f;

	/// if pickPass then set the ID color, otherwise use node color
	if (data->io.gl.pickPass)
	{	
		npIDtoRGB (node->id, &idRed, &idGrn, &idBlu);
		glColor4ub (idRed, idGrn, idBlu, 255);		//set color mapped as id
	}
	else
		glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

	glPushMatrix();

	glTranslatef (parent->world.x, parent->world.y, parent->world.z);

	/// draw center line, kinda nice look and helps aliasing when cam far away
	glLineWidth (node->lineWidth);	//also effects wireframe version of geometry

	glBegin (GL_LINES);
		glVertex3f (0.0f, 0.0f, 0.0f);
		glVertex3f (-vec.x, -vec.y, -vec.z);
	glEnd();

	/// @todo orient the object using a rotation matrix, avoid costly trig
	rotZ = atan2f (unitVec.y, unitVec.x);
	rotX = asinf (unitVec.z);

	glRotatef (rotZ * kRADtoDEG - 90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef (rotX * kRADtoDEG + 90.0f, 1.0f, 0.0f, 0.0f);

	/// scale the width using ratio and length is half the distance
	glScalef (node->ratio, node->ratio, distance * 0.5f);	//cyl is 2 units

	/// offset to the end of the cylinder instead of its center
	glTranslatef (0.0f, 0.0f, kNPoffsetUnit);

	/// calculate screen coordinates
	node->screen = npProjectWorldToScreen (&node->tagOffset);

	/// draw the rod, default geometry = kNPgeoCylinder
	npGLSurface (true, node, data);	

	/// if tagMode then calculate screen position and add to tags draw list
	if (node->tagMode)
		npAddTagToDraw(node, data);

	if (data->io.gl.pickPass)	//no picking for selection highlight
	{
		glPopMatrix();
		return;
	}

	glScalef (1.1f, 1.1f, 1.4142f);	/// z scaled for cube to be 2 units

	if( node->selected )
	{
		glColor4ub (255, 255, 0, 204);				/// yellow if selected
		npGLPrimitive (kNPgeoCubeWire, 0.0f);
	}

	if( node == data->map.currentNode )
	{
		glColor4ub (255, 0, 0, 255);				/// red if active
		glRotatef (22.0f, 0.0f, 0.0f, 1.0f);		// rotate from selected
		npGLPrimitive (kNPgeoCubeWire, 0.0f);
	}

	glPopMatrix();
}
	

/*
//------------------------------------------------------------------------------
void npProcessLinkQue (void* dataRef)
{
	pData data = (pData) dataRef;

	int i = 0;

	for (i=0; i < data->io.gl.linkQueCount; i++)
	{
		DrawLink (data->io.gl.linkQue[i], data);	//draw the link
		data->io.gl.linkQue[i] = NULL;				//clears the que
	}

	data->io.gl.linkQueCount = 0;					//clears the que
}
*/
//------------------------------------------------------------------------------
// new que generated each cycle, cleared while drawing
void npDrawLinkQue (void* dataRef)
{
	pData data = (pData) dataRef;
	void** linkQue = data->io.gl.linkQue;
	int count = data->io.gl.linkQueCount;
	pNPnode link = NULL;
	int i = 0;

	for (i=0; i < count; i++)
	{
		link = (pNPnode)linkQue[i];
		npDrawLink (link, data);		//draw the link
		link = NULL;				//clears the que
	}

	data->io.gl.linkQueCount = 0;					//clears the que
}

/// Links with one parent hidden are not drawn but need their linkFlag reset.
//------------------------------------------------------------------------------
void npClearLinkQue (void* dataRef)
{
	pData data = (pData) dataRef;
	void** linkPreQue = data->io.gl.linkPreQue;
	pNPnode link = NULL;
	int i = 0;
	int count = data->io.gl.linkPreQueCount;

	/// Clearing the linkFlag is optimized for speed
	for (i=0; i < count; i++)
	{
		link = (pNPnode)linkPreQue[i];
		link->linkFlag = 0;
	//	link = NULL;	//zz i don't think is necessary
	}

	data->io.gl.linkPreQueCount = 0;
}

//first time called we set the link node flag to indicate one end is rendered
//second time we add to the link queue to render after the second pin is done
//links need both ends rendered first to know the world coordinates of the ends
//------------------------------------------------------------------------------
void npAddLinkQue (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgl gl = &data->io.gl;

	if( node->linkFlag )// < 3 )
	{
	//	node->linkFlag = false; //reset the flag for next pass

		if( gl->linkQueCount >= kNPlinkQueMax )
		{
			npPostMsg("err 6289 - kNPlinkQueMax exceeded", kNPmsgErr, data);
			return;
		}

		//add the link to the que to be drawn after current root pin is rendered
		gl->linkQue[gl->linkQueCount++] = node;
	}
	else if( gl->subsample ) //small optimization....
	{
		/// LinkPreQue is used to avoid subsampled link	flicker issue# 90 & 97
		if( gl->linkPreQueCount >= kNPlinkQueMax )
		{
			npPostMsg("err 6290 - kNPlinkQueMax exceeded", kNPmsgErr, data);
			return;
		}
		gl->linkPreQue[gl->linkPreQueCount++] = node;
		node->linkFlag = true;	// flag that one parent has been rendered
	}
	else
		node->linkFlag = true;	// flag that one parent has been rendered
}

//------------------------------------------------------------------------------
void DrawVideo (pNPnode node)
{
	return;
}


//------------------------------------------------------------------------------
void DrawGrid (pNPnode node, void* dataRef)
{
	int			i = 0;
	int			j = 0;
	int			k = 0;
	int			idRed = 0, idGrn = 0, idBlu = 0;
	float		gridOffset = -0.4f;			//obscured grid line workaround, debug zz
	GLfloat		x = 0.0f;
	GLfloat		y = 0.0f;
	GLfloat		z = 0.0f;
	NPfloatXYZ	length;

	pData data = (pData) dataRef;
	NPgridPtr grid = (NPgridPtr) node->data;
	pNPnode camNode = data->map.currentCam;

	//MB-TEXTURE
	const GLfloat sgenparams[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat tgenparams[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	//MB-END

	//if pickPass then don't draw grid
	if (data->io.gl.pickPass && data->io.mouse.pickMode != kNPmodeGrid) //debug, zz
		return;

	//if pickPass then set the ID color
	if (data->io.gl.pickPass)
	{	
		npIDtoRGB (node->id, &idRed, &idGrn, &idBlu);
		printf("r: %d   g: %d   b: %d\n", idRed, idGrn, idBlu);
		glColor4ub (idRed, idGrn, idBlu, 255);		//set color mapped as id
	}

	//turn two-sided rendering of polygon faces on
	if (node->branchLevel > 0)
		glDisable (GL_CULL_FACE);

	glDisable (GL_LIGHTING);	//draw 100% ambient white
	
	glPushMatrix();				//restore after calling child nodes

	//used to calculate grid center based on segment count and spacing
//	length.x = grid->spacing.x * node->segments.x;	//zz grid
//	length.y = grid->spacing.y * node->segments.y;
	length.x = node->auxA.x * node->segments.x;
	length.y = node->auxA.y * node->segments.y;		//zz grid end

	//position, rotate and scale node
	glTranslatef (node->translate.x, node->translate.y, node->translate.z);

	glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
	glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
	glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
		
	glScalef (node->scale.x, node->scale.y, node->scale.z);

	//center based on length
	glTranslatef (length.x * -0.5f, length.y * -0.5f, 0.0f);

	//draw texture quad
	if (node->textureID || data->io.gl.pickPass)	//allow for click on background tex's for mouse flying
	{
		//if not pickPass then enable and bind texture, set color to white
		if (!data->io.gl.pickPass)
		{
			if (node->branchLevel > 0)	//child grids use opacity for textures
				glColor4ub (255, 255, 255, node->color.a);
			else
				glColor4ub (255, 255, 255, 255);	 //root grid is fully opaque

			glEnable ( GL_TEXTURE_2D );
			glBindTexture ( GL_TEXTURE_2D, node->textureID );

			//using GL_MODULATE instead of GL_DECAL to allow alpha with RGBA textures
			glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

			glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
			glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );

			glDisable( GL_TEXTURE_GEN_S );
			glDisable( GL_TEXTURE_GEN_T );

			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			
			//rather sharp setting, but probably best for mapping
			//for video use GL_LINEAR_MIPMAP_NEAREST when the angle is close 
			//to perpendicular less artifacts and a bit more blurry
//zz mipmap
			if(0)//gl->mipmap		
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);		//zz debug, add mipmapping...
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//zz debug, add mipmapping...
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}

			glTexGenfv( GL_S, GL_OBJECT_PLANE, sgenparams );
			glTexGenfv( GL_T, GL_OBJECT_PLANE, tgenparams );
		}

		//draw the quad
		glBegin (GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f(0.0f,  0.0f, gridOffset);	 // center quad
			glTexCoord2f(1.0f,0.0f); glVertex3f(length.x, 0.0f, gridOffset);
			glTexCoord2f(1.0f,1.0f); glVertex3f(length.x, length.y, gridOffset);
			glTexCoord2f(0.0f,1.0f); glVertex3f(0.0f,  length.y, gridOffset);
		glEnd();
		
		glDisable ( GL_TEXTURE_2D );
	}

	//draw grid lines unless hidden
	if (!node->hide)
	{
		// if not pickPass then set the node color
		if (!data->io.gl.pickPass)
			glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

		//draw grid lines on top of texture map
//		glDisable (GL_DEPTH_TEST);					//debug
		glLineWidth (node->lineWidth);

		//draw grid lines
		glBegin (GL_LINES);
			for (i=0; i <= node->segments.z; i++)
			{
				//need to do this before rest of grid lines, otherwise no showup
				//if currentNode then draw yellow selection box
				if (node == data->map.currentNode)
				{
					glColor4ub(255,255,0,192);	//yellow, 75% opacity

					glVertex3f (0.0f, 0.0f, z);			//side one
					glVertex3f (length.x, 0.0f, z);

					glVertex3f (length.x, 0.0f, z);		//two
					glVertex3f (length.x, length.y, z);

					glVertex3f (length.x, length.y, z);	//three
					glVertex3f (0.0f, length.y, z);

					glVertex3f (0.0f, length.y, z);		//four
					glVertex3f (0.0f, 0.0f, z);

					glVertex3f (length.x * 0.5f, 0.0f, z);	 //cross hairs
					glVertex3f (length.x * 0.5f, length.y, z);

					glVertex3f (0.0f, length.y * 0.5f, z);	 //cross hairs
					glVertex3f (length.x, length.y * 0.5f, z);

					// restore node color 
					glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);
				}
		
				for (j=0; j <= node->segments.x; j++)
				{
					glVertex3f (x, 0.0f, z);
					glVertex3f (x, length.y, z);
					x += node->auxA.x;					//zz grid
				}
				for (j=0; j <= node->segments.y; j++)
				{
					glVertex3f (0.0f, y, z);
					glVertex3f (length.x, y, z);
					y += node->auxA.y;					//zz grid
				}

				x = 0.0f;						//reset x and y
				y = 0.0f;
				z += node->auxA.z;	//zz grid	//set the z for 3D stacked grids
			}
		glEnd();

		glLineWidth (1.0f);
//		glEnable (GL_DEPTH_TEST);
	}

	//turn two-sided rendering of polygon faces off
	if (node->branchLevel > 0)
		glEnable (GL_CULL_FACE);

	// if not pickPass then restore lighting
	if (!data->io.gl.pickPass)
		glEnable (GL_LIGHTING);

	//undo center based on length
	glTranslatef (length.x * 0.5f, length.y * 0.5f, 0.0f);

	//do not want scaling of root grid to effect children, single case
	if (node == data->map.node[kNPnodeRootGrid])
	{
		glPopMatrix();				//restore for root grid only
		return;				//current compromise is to draw root befoer pins, zz debug
	}

	//recursively traverse and draw children
	for (i=0; i < node->childCount; i++)
		DrawGrid(node->child[i], data);

	//for all other nodes the transformations are inherited
	if (node != data->map.node[kNPnodeRootGrid])
		glPopMatrix();				// restore after traversing child nodes
}


//zz debug, add handling for character color map using index colors
//zz debug, add character selection using pickPass and/or mouseXY to cell calc
//zz debug, add inverse blinking cursor, underbar, vertical bar and whole char
//draw console GUI messages based on mode
//------------------------------------------------------------------------------
void npDrawConsole (void* dataRef)
{
	int i = 0;
	int index = 0;
	int level = 0;

	float charWidth = 9.0f;
	float charHeight = 15.0f;

	float boxHeight = 0.0f;			//top edge of text background box
	float boxWidth = 0.0f;

	float selectBoxWidth = 0.0f;

	int lineCount = 0;

	pData data = (pData) dataRef;
	pNPconsole console = (pNPconsole) &data->io.gl.hud.console;

	//exit to not draw if mode is null
	if ( console->level == 0 && console->mode == 0 )		//update to use the hide 
		return;

	//zz debug, does not allow mouse picking the console, update for text entry
	if (data->io.gl.pickPass)
		return;

	//set the number of text rows
	switch (console->level) 
	{
		case kNPconsoleLevelOne : lineCount = 1; break;
		case kNPconsoleLevelThree : lineCount = 3; break;
		case kNPconsoleLevelMax : lineCount = kNPconsoleLineMax; break;
	}

	//if viewer mode then set console lines to max
	if ( console->mode == kNPconsoleMenu )					//zz debug, move this to npconsole.c
		lineCount = kNPconsoleLineMax;

	//calculate background box with margin
	boxWidth = console->box.x;//charsPerLine * charWidth + 14.0f;		//zz debug
	boxHeight = lineCount * charHeight + 8.0f;

	//draw background box
	glColor4ub(console->boxColor.r, console->boxColor.g, console->boxColor.b, console->boxColor.a);
	
	glPushMatrix();
	glTranslatef(console->screen.x, console->screen.y, 0.0f);

	glBegin (GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(boxWidth, 0.0f);
		glVertex2f(boxWidth, boxHeight);
		glVertex2f(0.0f,  boxHeight);
	glEnd();

	//draw text color
	glColor4ub(console->color.r, console->color.g, console->color.b, console->color.a);
	glTranslatef(4.0f, 7.0f, 0.0f);	//text margin offset

	//set the initial line index for the draw text loop
	index = console->lineIndex;

	//draw number of text lines specified by lineCount
	for (i=0; i < lineCount; i++)
	{
		//set position and draw text by line index
		glRasterPos2f( 0.0f, charHeight * (float)i );
		npGlutDrawString( GLUT_BITMAP_9_BY_15, &console->line[index--][0] );

		if (index < 0)
			index = kNPconsoleLineMax - 1;		//handles buffer roll-over
	}
	
	//draw blinking cursor and highlight on top of text using inverse XOR color
	if( console->cursorShow )
	{
		if( data->io.blinkState )
		{
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);
				glRasterPos2f( charWidth * (float)console->cursorColumn,
							   charHeight * (float)console->cursorLine );
				npGlutDrawString( GLUT_BITMAP_9_BY_15, "_" );	// blink cursor
			glDisable(GL_COLOR_LOGIC_OP);
		}
		else if( console->selectText )	// select highlight 180 deg out of phase
		{
			selectBoxWidth = charWidth * (float)console->cursorColumn;

			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);
				if( console->selectText )						// highlight box
				{
					glBegin (GL_QUADS);
						glVertex2f(-2.0f + charWidth, -4.0f);	// border offset
						glVertex2f(selectBoxWidth, -4.0f);
						glVertex2f(selectBoxWidth, charHeight - 3.0f );
						glVertex2f(-2.0f + charWidth,  charHeight - 3.0f );
					glEnd();
				}
			glDisable(GL_COLOR_LOGIC_OP);

			//now darken the highlight, so its not so blinding...
			glColor4ub( 0, 0, 0, 64 );
			glBegin (GL_QUADS);
						glVertex2f(-2.0f + charWidth, -4.0f);	// border offset
						glVertex2f(selectBoxWidth, -4.0f);
						glVertex2f(selectBoxWidth, charHeight - 3.0f );
						glVertex2f(-2.0f + charWidth,  charHeight - 3.0f );
			glEnd();
		}
	}

	glPopMatrix();
}

//------------------------------------------------------------------------------
void npDrawFPS (void* dataRef)
{
	float fps = 0.0f;

	pData data = (pData) dataRef;
	pNPtag tag = &data->io.gl.hud.fps;

	//zz debug, does not allow mouse picking the FPS, update to allow hiding
	if (data->io.gl.pickPass)
		return;

	if (data->io.cycleDelta)
		fps = (float) (1.0 / data->io.cycleDelta);

	sprintf (tag->title, "FPS:%6.2f", fps);

	//updates the box size, we really only need to do this once, debug zz
	npUpdateTag (tag);

	glPushMatrix();
		glTranslatef (data->io.gl.width * 0.5f - 105.0f,
					  data->io.gl.height * 0.5f - 25.0f, 0.0f);
		//now draw the tag
		npDrawTextTag (tag, data);
	glPopMatrix();
}

//zz select
//draw selection box
//------------------------------------------------------------------------------
void npDrawSelect (void* dataRef)
{
	pData data = (pData) npGetDataRef();
	pNPnode tool = data->ctrl.user[0]->tool;

	if(tool->hide)
		return;

	glBlendFunc (GL_SRC_ALPHA, GL_DST_COLOR);

	glPushMatrix();

	//offset screen coordinates such that 0,0 is in lower left corner
	glTranslatef(data->io.gl.width * -0.5f, data->io.gl.height * -0.5f, 0.0f);

	glTranslatef(tool->translate.x, tool->translate.y, 0.0f);
	
	glColor4ub(64,32,0,100);

	if ( (tool->translateVec.x * tool->translateVec.y) >= 0.0f)
	{
		glBegin (GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(tool->translateVec.x, 0.0f);
			glVertex2f(tool->translateVec.x, tool->translateVec.y);
			glVertex2f(0.0f, tool->translateVec.y);
		glEnd();
	}
	else		//reverse loop direction for forward facing normal
	{
		glBegin (GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, tool->translateVec.y);
			glVertex2f(tool->translateVec.x, tool->translateVec.y);
			glVertex2f(tool->translateVec.x, 0.0f);
		glEnd();
	}

	glColor4ub(255,255,0,100);

	glBegin (GL_LINE_LOOP);
		glVertex2f(1.0f, 2.0f);
		glVertex2f(tool->translateVec.x - 2.0f, 2.0f);
		glVertex2f(tool->translateVec.x - 2.0f, tool->translateVec.y - 1.0f);
		glVertex2f(1.0f, tool->translateVec.y - 1.0f);
	glEnd();

	glPopMatrix();
}

//------------------------------------------------------------------------------
void npDrawCompass (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPnode hudParent = data->map.node[kNPnodeRootHUD];
	pNPnode hudItem = NULL;
	pNPnode node = data->map.currentNode;
	pNPnode parent = node->parent;

	int i = 0;
	int parentTopo = 0;
	NPboolXYZ axes = data->io.axes;	//for convienance
	static const NPubyteRGBA black = {0,0,0,85};
	static const NPubyteRGBA grey = {127,127,127,127};
	static const NPubyteRGBA green = {0,100,0,127};
	static const NPubyteRGBA red = {255,0,0,200};
	static const NPubyteRGBA yellow = {255,255,0,220};


	if (parent != NULL)
		parentTopo = parent->topo;

	//pNPtag tag = &data->io.gl.hud.fps;								//zz debug

	//draw with Heading for tile or as compass directions
	hudItem = hudParent->child[kNPhudCompass];
	if (hudItem->childIndex)
		sprintf (hudItem->tag->title, "Heading: %6.2f", node->rotate.y);
	else
	{
		if (node->rotate.y > 337.5f || node->rotate.y < 22.5f)
			sprintf (hudItem->tag->title, "   %6.2f N    ", node->rotate.y);
		else if (node->rotate.y > 292.5f) 
			sprintf (hudItem->tag->title, "   %6.2f NW   ", node->rotate.y);
		else if (node->rotate.y > 247.5f) 
			sprintf (hudItem->tag->title, "   %6.2f W    ", node->rotate.y);
		else if (node->rotate.y > 202.5f) 
			sprintf (hudItem->tag->title, "   %6.2f SW   ", node->rotate.y);
		else if (node->rotate.y > 157.5f) 
			sprintf (hudItem->tag->title, "   %6.2f S    ", node->rotate.y);
		else if (node->rotate.y > 112.5f) 
			sprintf (hudItem->tag->title, "   %6.2f SE   ", node->rotate.y);
		else if (node->rotate.y > 67.5f) 
			sprintf (hudItem->tag->title, "   %6.2f E    ", node->rotate.y);
		else if (node->rotate.y > 22.5f) 
			sprintf (hudItem->tag->title, "   %6.2f NE   ", node->rotate.y);
	}	
	npUpdateTag (hudItem->tag);

	//draw XYZ axes with green for active and grey for disabled
	if ( node->scaleRate.x || node->scaleRate.y || node->scaleRate.z
		|| data->io.mouse.tool == kNPtoolSize 
		&& data->map.currentNode != data->map.currentCam
		|| ( node->branchLevel && data->io.mouse.buttonR //does not handle if root is a torus, no ratio displayed, zz debug
			 && data->io.mouse.pickMode == kNPmodePin
			 && ( data->io.mouse.tool == kNPtoolCombo 
				 || data->io.mouse.tool == kNPtoolCreate  ) ) )
	{
		hudItem = hudParent->child[kNPhudAngle];
		sprintf (hudItem->tag->title, "Ratio:  %7.2f", node->ratio);
		npUpdateTag (hudItem->tag);
		if (data->io.mouse.buttonR)
			hudItem->tag->boxColor = green;			//green if active
		else
			hudItem->tag->boxColor = grey;			//grey if unavailable

		hudItem = hudParent->child[kNPhudCoordX];
		sprintf (hudItem->tag->title, "Scale X:%7.2f", node->scale.x);
		npUpdateTag (hudItem->tag);
		if (axes.x)
		{
			if (data->io.mouse.buttonR)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = green;
		}
		else
			hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordY];
		sprintf (hudItem->tag->title, "Scale Y:%7.2f", node->scale.y);
		npUpdateTag (hudItem->tag);
		if (axes.y)
		{
			if (data->io.mouse.buttonR)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = green;
		}
		else
			hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordZ];
		sprintf (hudItem->tag->title, "Scale Z:%7.2f", node->scale.z);
		npUpdateTag (hudItem->tag);
		if (axes.z)
		{
			if (data->io.mouse.buttonR)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = green;
		}
		else
			hudItem->tag->boxColor = black;
	}
	else if ( data->io.mouse.tool == kNPtoolRotate
			&& data->map.currentNode != data->map.currentCam ) 
	{
		hudItem = hudParent->child[kNPhudAngle];
		if (hudItem->childIndex)					//clean this method up, zz debug
			sprintf (hudItem->tag->title, "Roll:   %7.2f", node->rotate.z);
		else
			sprintf (hudItem->tag->title, "Tilt:   %7.2f", node->rotate.x);
		hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordX];
		sprintf (hudItem->tag->title, "RotateX:%7.2f", node->rotate.x);
		npUpdateTag (hudItem->tag);
		if (axes.x)
		{	
			if (data->io.mouse.buttonR)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = green;
		}
		else
			hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordY];
		sprintf (hudItem->tag->title, "RotateY:%7.2f", node->rotate.y);
		npUpdateTag (hudItem->tag);
		if (axes.y)
		{	
			if (data->io.mouse.buttonR)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = green;
		}
		else
			hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordZ];
		sprintf (hudItem->tag->title, "RotateZ:%7.2f", node->rotate.z);
		npUpdateTag (hudItem->tag);
		if (axes.z)
		{	
			if (data->io.mouse.buttonR || !(axes.x && axes.y))
				hudItem->tag->boxColor = green;
			else
				hudItem->tag->boxColor = grey;
		}
		else
		{	
			if (data->io.mouse.buttonR)
				hudItem->tag->boxColor = green;
			else
				hudItem->tag->boxColor = black;
		}
	}
	else if (  (   data->io.mouse.tool == kNPtoolCombo
				|| data->io.mouse.tool == kNPtoolCreate
				|| data->io.mouse.tool == kNPtoolMove   )
			 && data->map.currentNode != data->map.currentCam )
	{
		hudItem = hudParent->child[kNPhudAngle];
		if (hudItem->childIndex)					//clean this method up, zz debug
			sprintf (hudItem->tag->title, "Roll:   %7.2f", node->rotate.z);
		else
			sprintf (hudItem->tag->title, "Tilt:   %7.2f", node->rotate.x);
		npUpdateTag (hudItem->tag);
		hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordX];
		sprintf (hudItem->tag->title, "Coord X:%7.2f", node->translate.x);
		npUpdateTag (hudItem->tag);
		if (axes.x)
			hudItem->tag->boxColor = green;
		else
		{
			if (axes.x)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = black;
		}

		hudItem = hudParent->child[kNPhudCoordY];
		sprintf (hudItem->tag->title, "Coord Y:%7.2f", node->translate.y);
		npUpdateTag (hudItem->tag);
		if (axes.y)
		{
			if ( !(axes.x && axes.y ))
				hudItem->tag->boxColor = green;
			else if (node->branchLevel == 0 && !axes.y)
				hudItem->tag->boxColor = black;
			else
			{
				if (parentTopo != kNPtopoPin && parentTopo != kNPtopoRod)
					hudItem->tag->boxColor = green;
				else
					hudItem->tag->boxColor = grey;
			}
		}
		else
		{
			if (data->io.axes.y)
				hudItem->tag->boxColor = grey;
			else
				hudItem->tag->boxColor = black;
		}

		hudItem = hudParent->child[kNPhudCoordZ];
		sprintf (hudItem->tag->title, "Coord Z:%7.2f", node->translate.z);
		npUpdateTag (hudItem->tag);
		if (axes.z)
		{
			if ( !(axes.x && axes.y) )
			{
				if ( parentTopo != kNPtopoPin && parentTopo != kNPtopoRod
					 && !(node->branchLevel == 0 && axes.y) )
					hudItem->tag->boxColor = green;
				else
					hudItem->tag->boxColor = grey;
			}
			else
				hudItem->tag->boxColor = grey;
		}
		else
		{
			if ( data->io.mouse.buttonR 
				&& parentTopo != kNPtopoPin
				&& parentTopo != kNPtopoRod
				&& node->branchLevel != 0 )
				hudItem->tag->boxColor = green;
			else
				hudItem->tag->boxColor = black;
		}
	}
	else
	{
		hudItem = hudParent->child[kNPhudAngle];
		if (hudItem->childIndex)					//clean this method up, zz debug
			sprintf (hudItem->tag->title, "Roll:   %7.2f", node->rotate.z);
		else
			sprintf (hudItem->tag->title, "Tilt:   %7.2f", node->rotate.x);
		npUpdateTag (hudItem->tag);
		hudItem->tag->boxColor = black;

		//	sprintf (coordX.title,	"LONG X:%7.2f", node->translate.x);	//zz debug, base on compass vs heading?
		//	sprintf (coordY.title,	"LAT  Y:%7.2f", node->translate.y);
		//	sprintf (coordZ.title,	"ALT  Z:%7.2f", node->translate.z);

		hudItem = hudParent->child[kNPhudCoordX];
		sprintf (hudItem->tag->title, "Coord X:%7.2f", node->translate.x);
		npUpdateTag (hudItem->tag);
		if (data->io.axes.x)
			hudItem->tag->boxColor = green;
		else
			hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordY];
		sprintf (hudItem->tag->title, "Coord Y:%7.2f", node->translate.y);
		npUpdateTag (hudItem->tag);
		if (data->io.axes.y)
			hudItem->tag->boxColor = green;
		else
			hudItem->tag->boxColor = black;

		hudItem = hudParent->child[kNPhudCoordZ];
		sprintf (hudItem->tag->title, "Coord Z:%7.2f", node->translate.z);
		npUpdateTag (hudItem->tag);
		if (data->io.axes.z)
			hudItem->tag->boxColor = green;
		else
			hudItem->tag->boxColor = black;
	}


	//update the mode outline color
	hudItem = hudParent->child[kNPhudMode];
	if (data->io.mouse.pickMode == kNPmodeCamera)
	{
		strcpy (hudItem->tag->title, "mode: Camera   "); //unify this with npPostMode, zz debug
		hudItem->color = red;
	}
	else if (data->io.mouse.pickMode == kNPmodeGrid)
	{
		strcpy (hudItem->tag->title, "mode: Grid     ");
		hudItem->color = yellow;
	}
	else // if (data->io.mouse.pickMode == kNPmodePin)
	{
		strcpy (hudItem->tag->title, "mode: Pin      ");
		hudItem->color = grey;
	}
	npUpdateTag (hudItem->tag);

	if (data->io.mouse.pickMode == kNPmodeCamera)
	{
		hudItem = hudParent->child[kNPhudTool];
		strcpy (hudItem->tag->title, "tool: Target   ");
		npUpdateTag (hudItem->tag);
	}

	//zzf //zz select
	hudItem = hudParent->child[kNPhudSelect];
	if (data->io.mouse.tool == kNPtoolSelect && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Select Region  ");
	npUpdateTag (hudItem->tag);

	if (1)//data->io.mouse.pickMode == kNPmodeCamera)
	{
		hudItem = hudParent->child[kNPhudSave];
		if (data->io.file.saveSelect)
		{
			hudItem->tag->boxColor.r = 255;
			strcpy (hudItem->tag->title, "Saving.....    ");
		}
		else
		{
			hudItem->tag->boxColor.r = 127;
			strcpy (hudItem->tag->title, "Save Selected  ");
		}
		npUpdateTag (hudItem->tag);
	}
	//zzf end

//zz select
	hudItem = hudParent->child[kNPhudCreate];
	if (data->io.mouse.tool == kNPtoolCreate && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Create         ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudLink];
	if (data->io.mouse.tool == kNPtoolLink && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Link           ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudCombo];
	if (data->io.mouse.tool == kNPtoolCombo && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Combo          ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudMove];
	if (data->io.mouse.tool == kNPtoolMove && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Move           ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudRotate];
	if (data->io.mouse.tool == kNPtoolRotate && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Rotate         ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudSize];
	if (data->io.mouse.tool == kNPtoolSize && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Size           ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudTopo];
	if (data->io.mouse.tool == kNPtoolTopo && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Topo           ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudGeo];
	if (data->io.mouse.tool == kNPtoolGeometry && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Geometry       ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudColor];
	if (data->io.mouse.tool == kNPtoolColor && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Color          ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudTexture];
	if (data->io.mouse.tool == kNPtoolTexture && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Texture        ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudHide];
	if (data->io.mouse.tool == kNPtoolHide && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Hide           ");
	npUpdateTag (hudItem->tag);

	hudItem = hudParent->child[kNPhudTag];
	if (data->io.mouse.tool == kNPtoolTag && data->io.mouse.pickMode == kNPmodePin)
		hudItem->color = yellow;
	else
		hudItem->color = grey;
	strcpy (hudItem->tag->title, "Text Tag       ");
	npUpdateTag (hudItem->tag);


	//draw indicators
	glPushMatrix();

		//update this to use hudRoot->tools position,						 zz degug
		//set initial position to upper left corner
		glTranslatef (data->io.gl.width * -0.5f + 5.0f,
					  data->io.gl.height * 0.5f - 5.0f, 0.0f);

		// for (i=0; i < hudParent->childCount; i++)						//zz debug
		for (i = kNPhudCompass; i < kNPhudCount; i++)		//zzf
		{
			hudItem = hudParent->child[i];
			glTranslatef (0.0f, -21.0f, 0.0f);
			npDrawNodeTextTag (hudItem, data);
		}

	glPopMatrix();
}

// draw 2D overlays including grids, text, messages, highlight objects, etc...
//------------------------------------------------------------------------------
void npDrawHUD (void* dataRef)
{
	float sc;
	pData data = (pData) dataRef;
	pNPhud hud = (pNPhud) &data->io.gl.hud;

	glPushMatrix();
	
	glDisable(GL_DEPTH_TEST);	//turn off depth testing so HUD is not obscured
	glDisable(GL_LIGHTING);		//disable lighting to draw 100% ambient

	//enable subtractive transparency unless already done
	if (data->io.gl.alphaMode != kNPalphaModeSubtractive)
	{
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//scale to Screen Coords
	sc = 63.067f / data->io.gl.height;	//63.067f for 35mm fov, upgrade for any fov, debug zz
	glScalef( sc, sc, sc );

	//draws node text tags... //MB-LABEL
	if (hud->drawTags)
		npDrawTags(data);

	if (data->io.mouse.tool == kNPtoolSelect)
		npDrawSelect(data);

	//zz debug, currently drawConsole is redundant to kNPconsoleLevelNull
	if (hud->drawConsole)
	{
		//draw FPS first puts it below other HUD elements	
		if (hud->drawFPS && hud->console.level > kNPconsoleLevelOne)
			npDrawFPS (data);
		
		//draw console before indicators, allows indicators to be clicked
		npDrawConsole (data);

		//draw compass based on level and mode
		if ( hud->drawCompass
			&& hud->console.level > kNPconsoleLevelOne
			&& hud->console.mode != kNPconsoleMenu )
			npDrawCompass (data);
	}

	glPopMatrix();
}


//MB-Transp
//------------------------------------------------------------------------------
//Distance between two points
float npDistance( NPfloatXYZ vecA, NPfloatXYZ vecB )
{
	float x = vecA.x - vecB.x;
	float y = vecA.y - vecB.y;
	float z = vecA.z - vecB.z;
	
//	float len = x*x + y*y + z*z;
//	len = sqrtf( len );
//	return len;

	return sqrtf( x*x + y*y + z*z );
}

//------------------------------------------------------------------------------
//QSort Compare function
int comparePinsDistanceFromCamera (const void* a, const void* b)
{
	pNPnode pinA = *(pNPnode*)a;
	pNPnode pinB = *(pNPnode*)b;
	
	//optimized for the common case where neither a nor b is null
	if( pinA == NULL || pinB == NULL )	//Null element in pinA & pinB: Nothing or Grid or Camera
	{
		if( pinA != NULL )				//Null element in pinB: Nothing or Grid or Camera
			return +1;					//pinB is at beginning of list... therefore pinA is "greater than" pinB. Return +1
		if( pinB != NULL )				//Null element in pinA: Nothing or Grid or Camera
			return -1;					//pinA is at beginning of list... therefore pinA is    "less than" pinB. Return -1
		return 0;						//These are "equal". Both should be at the beginning of the list.
	}

	if( pinA->distFromCamera   >  pinB->distFromCamera )	//pinA is furter from camera than pinB.
		return -1;											//Therefore should be at the beginning of the list. ie "less than"... Return -1
	if( pinA->distFromCamera   <  pinB->distFromCamera )	//pinA is closer from camera than pinB.
		return +1;											//Therefore should be at the end of the list.    ie "greater than"... Return +1
	if( pinA->distFromCamera  ==  pinB->distFromCamera )	//pinA & pinB are exactly the same distance from the camera. (impossible-ish)
		return 0;											//Return 0;

	//return ( *(int*)a - *(int*)b );
	return -1;
}

int sortCount = 0;	//zzhp
void npDrawSortThread(void* dataRef);
//------------------------------------------------------------------------------
void npDrawSortThread(void* dataRef)										 //zzhp
{
	int waitForSwap = 0;
	pData data = (pData) dataRef;

	int* killThread = false; //data->map.sortKillThread;		//zz debug

	while (!killThread)
	{
		//swap flag set by draw routine when its reaches the end of sort buffer
		//loading flag signals not to sort 
		while(data->map.sortSwapFlag || data->io.file.loading
				|| data->io.clearIndex)		//zzhp test flag pauses sorting, rigged to background colorIndex
			nposSleep(0.001);

		if (data->map.sortSwap)
		{
			data->map.sortCountA = data->map.nodeRootCount;
			npDrawSort (data->map.sortA, data->map.sortCountA, data);
		}
		else
		{
			data->map.sortCountB = data->map.nodeRootCount;
			npDrawSort (data->map.sortB, data->map.sortCountB, data);
		}

		data->map.sortSwapFlag = true;		//flag that sortDone
	}

	nposEndThread();
}

//------------------------------------------------------------------------------
void npDrawSort(void** sort, int count, void* dataRef)						//zzhp
{
	int i = 0;

	pData data = (pData) dataRef;

	pNPnode node = NULL;

	pNPnode camNode = data->map.currentCam;
	NPfloatXYZ camVec = camNode->translate;
	
	int nodeRootCount = data->map.nodeRootCount;
//	sortCount = nodeRootCount;											//zzhp

	//add root node thread locking and node delete synchro											//zz debug 

	//could pre-subsample the node list when large nodeRootCount > 100K		//zz debug

	//update root node distance from camera
	for( i = 0; i < count; i++ )		//zzhp
	{
		if( i < kNPnodeRootPin )	//Nothing, Camera, Grid, HUD...
		{
			sort[i] = NULL;
			continue;
		}
		
		node = data->map.node[i];
		node->distFromCamera = npDistance(camVec, node->translate);

		sort[i] = node;
	}

	if (count > data->map.nodeRootCount)
	{
		printf("err 2399 - count > data->map.nodeRootCount by: %d\n",
				(count - data->map.nodeRootCount));
		return;
	}
	//create z-sorted list, back to front
	//QuickSort based on Distance From Camera, back to front z-sorted
	qsort( sort, count, sizeof(pNPnode),
			comparePinsDistanceFromCamera );
}


//------------------------------------------------------------------------------
void npDrawNodes (void* dataRef)
{
	int test = 0;  //zzhp
	int i = 0;
	int selected = false;
	int selectedRootIndex = 0;
	bool drawActive = true;

	void** nodeList = NULL;													//zzhp

	pNPnode node = NULL;
	pData data = (pData) dataRef;

	int count = data->map.nodeRootCount;
	int subsample = data->io.gl.subsample;	//zzhp

	selectedRootIndex = data->map.selectedPinIndex;

	//z-sort nodes, back to front for proper transparency
//	npDrawSort(data);														//zzhp

	//need count to stored as part of the sort buffer
	//and make sure sorting the right buffer...
	/// @todo fix thread sync problem and turn z-sort back on
	if (1)//data->io.file.loading)												//zzhp
	{
		nodeList = data->map.node;
		count = data->map.nodeRootCount;
	}
	else
	{
		//swap procedure using thread locks with double buffered sort list 
		if (data->map.sortSwapFlag)												//zzhp
		{

			if (data->map.sortSwap)	//(data->map.sort == data->map.sortB)
			{
				nodeList = data->map.sortA;
				count = data->map.sortCountA;
			}
			else
			{
				nodeList = data->map.sortB;
				count = data->map.sortCountB;
			}
			//swap buffers, invert the sortSwap indicator
			data->map.sortSwap = 1 - data->map.sortSwap;

			data->map.sortSwapFlag = false;
		}

		//nodeList = data->map.sort;											//zzhp
		//count = data->map.sort
	}

	//first drawing grids causes improper transparency, debug zz
	//alternately could draw the grid lines first, then the texture after pins
	//this would cause pins to obscure grid lines
	//another approach is to turn on clipping for grid lines and offset, debug zz
	//compromise by drawing root grid first, and sub-grids second
	DrawGrid (data->map.node[kNPnodeRootGrid], data);

	//draw all pin nodes, skip over camera and grids using kNPnodeRootPin
	for( i = kNPnodeRootPin; i < count; i += subsample )	//zzhp
	{	
		//draw root nodes back to front using z-sorted list
//		
		node = nodeList[i];			//zzhp

//		node = data->map.sort[i];	//zzhp
//		node = data->map.node[i];	//zzhp	
//zzhp
/*		for (test = 0; test < 9; test++)
		{
		glPushMatrix();
		glTranslatef(node->translate.x + 0.4f * (float)test, node->translate.y, node->translate.z);
			//orientation
//		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
//		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
//		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
//		glTranslatef(node->translate.x, node->translate.y, node->translate.z);
//		glTranslatef(-node->translate.x, -node->translate.y, -node->translate.z);
//		glScalef (node->scale.x, node->scale.y, node->scale.z);

		glColor4ub(node->color.r, node->color.g, node->color.b, node->color.a);
//		npDrawTorus (kNPgeoTorus, kNPdefaultRatio);//glCallList (primitiveDL + kNPgeoTorus);//geometry);	
		npGLSurface (true, node, data);	
		glPopMatrix();
		}
*/		if( 0 ) // node->hide)
		{
		}
		else
		{											//selected = true;
			if (node == data->map.node[data->map.selectedPinIndex]) //use selectedRootIndex? debug zz

				DrawPin (true, node, data);
			else							
				DrawPin (false, node, data);		//selected = false;
		}

/*		if (!node->hide)				//if not hidden draw node
			switch (node->type)
			{
				case kNodeDefault	: DrawDefault(node); break;
				case kNodeCamera	: DrawCamera (node); break;
				case kNodeSurface	: DrawSurface(node); break;
				case kNodePoints	: DrawPoints (node); break;
				case kNodePin		: DrawPin (selected, node, data); break;
				case kNodeVideo		: DrawVideo  (node); break;
				case kNodeGrid		: DrawGrid   (node, data); break;
			}
*/
		npDrawLinkQue (data);

//zzhp		
//		i += data->io.gl.subsample - 1;		//zzhp draw every tenth pin
	}

	npClearLinkQue (data);

	//draw grids last to allow objects behind transparent grids to be seen
	//update this to allow a specified draw order using the node->shader
	//for example, root grid should probably be drawn before nodes and
	//sub-grids after nodes,												 debug zz
	//compromise by drawing root grid first, and sub-grids second
	node = data->map.node[kNPnodeRootGrid];
	for (i=0; i < node->childCount; i++)
		DrawGrid (node->child[i], data);

	//signal sort thread that we are done with our buffer
	data->map.sortSwapFlag = true;										//zzhp debug
}

/*
//------------------------------------------------------------------------------
void npDrawNodes (void* dataRef)
{
	int i = 0;
	int selected = false;

	pNPnode node = NULL;
	pData data = (pData) dataRef;

	int count = data->map.nodeRootCount;

	//z-sort nodes, back to front for proper transparency
	npDrawSort(data);

	//first drawing grids causes improper transparency, debug zz
	//alternately could draw the grid lines first, then the texture after pins
	//this would cause pins to obscure grid lines
	//another approach is to turn on clipping for grid lines and offset, debug zz
	//compromise by drawing root grid first, and sub-grids second
	DrawGrid (data->map.node[kNPnodeRootGrid], data);

	//draw all pin nodes, skip over camera and grids using kNPnodeRootPin
	for( i = kNPnodeRootPin; i < count; i++ )
	{		
		//draw root nodes back to front using z-sorted list
		node = data->map.sort[i];
		
		if (node == data->map.node[data->map.selectedPinIndex]) //use selectedRootIndex? debug zz
			selected = true;
		else
			selected = false;

		if (!node->hide)				//if not hidden draw node
			switch (node->type)
			{
				case kNodeDefault	: DrawDefault(node); break;
				case kNodeCamera	: DrawCamera (node); break;
				case kNodeSurface	: DrawSurface(node); break;
				case kNodePoints	: DrawPoints (node); break;
				case kNodePin		: DrawPin (selected, node, data); break;
				case kNodeVideo		: DrawVideo  (node); break;
				case kNodeGrid		: DrawGrid   (node, data); break;
			}

		npProcessLinkQue (data);
	}

	npClearLinkQue (data);

	//draw grids last to allow objects behind transparent grids to be seen
	//update this to allow a specified draw order using the node->shader
	//for example, root grid should probably be drawn before nodes and
	//sub-grids after nodes,												 debug zz
	//compromise by drawing root grid first, and sub-grids second
	node = data->map.node[kNPnodeRootGrid];
	for (i=0; i < node->childCount; i++)
		DrawGrid (node->child[i], data);
}
*/

//MB-LABEL
//Project CURRENT Matrix state World position Coordinates onto the Screen
//offset allows tag to be placed relative to node center
//if Z is less then 0.0f or greater then 1.0f then object is behind camera.
//------------------------------------------------------------------------------
NPfloatXYZ npProjectWorldToScreen (const pNPfloatXYZ offset)
{	
	GLdouble screenX, screenY, screenZ;	//Screen Coordinates Output

	NPfloatXYZ screen;

	GLdouble model_matrix[16];
	GLdouble projection_matrix[16];
	GLint viewport[4];

	//Model Matrix Current
	glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
	
	//Projection Matrix Current					//optimize to get once per cycle, zz debug
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

	//Viewport									//optimize to get once per cycle, zz debug
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Do the projection
	gluProject( offset->x, offset->y, offset->z,	// zz test (0.0f,0.0f,0.0f,
				model_matrix, projection_matrix, viewport,
				&screenX, &screenY, &screenZ );

	screen.x = (float)screenX;
	screen.y = (float)screenY;
	screen.z = (float)screenZ;

	//	objects behind the camera can have valid screen coordinates
	//	z is used to determine if object is behind the camera
	//	printf ("screen z: %f\n", screenZ);

	return screen;
}

