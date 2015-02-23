#include "Namuh_Main_H.h"

using namespace Ogre;


HideDescription::HideDescription() : materialName(""), cornerPos(), linkedBodies(), widthDivisions(0), heightDivisions(0), isWing(false), isBounce(false)
{
	PhysX_descr.reset();
}

bool HideDescription::Cook()
{
/*
	corners are defined as follow :

						topBorder ->
					0 --------------- 1						TODO : Explicit this in the game
					|				  |			
					|				  |
				|	|				  |
				|	|				  |		|
	leftBorder	|	|		hide	  |		|  rightBorder
			   \ /	|				  |	   \ /							/ \
				v	|				  |		v							 |
					|				  |								y	 |
					|				  |									 L------->
					2 --------------- 3										x
					  bottomBorder ->

	*/
	if (materialName == "")
		materialName = "hide";
	

	//		==== Beginning of mesh generation ====

	// Compute border vectors
	Vector3	topBorder		= cornerPos[1] - cornerPos[0],
			bottomBorder	= cornerPos[3] - cornerPos[2],
			leftBorder		= cornerPos[2] - cornerPos[0],
			rightBorder		= cornerPos[3] - cornerPos[1];

	// determine the X division level based on the longest edge between the top one and the bottom one
	float maxXBorderLength = topBorder.length();
	if(bottomBorder.length() > maxXBorderLength)
		maxXBorderLength = bottomBorder.length();
	int divisionXLvl = int(ceil(maxXBorderLength) );	// one vertex each 25cm minimum
	if (divisionXLvl > 60)
		divisionXLvl = 60;	//cap th hide at 60*60 = 1200 vertex -> 4800 poly
	
	// determine the X division level based on the longest edge between the right one and the left one
	float maxYBorderLength = leftBorder.length();
	if(rightBorder.length() > maxYBorderLength)
		maxYBorderLength = rightBorder.length();
	int divisionYLvl = int(ceil(maxYBorderLength));	// one vertex each 25cm minimum
	if (divisionYLvl > 60)
		divisionYLvl = 60;	//cap th hide at 60*60 = 1200 vertex -> 4800 poly

	// compute surface of hide
	surface = maxXBorderLength * maxYBorderLength /2;

	// beginning of geometrical algorithm TODO : generate a hide shape rather than a bended rectangle
	ldbg.send("(Hide Cooking) : generating a " + toOgStr(divisionXLvl) + "x" + toOgStr(divisionYLvl) + " hide");
	
	hideMesh.begin(NxOgre::Enums::MeshType_Cloth, (divisionXLvl +1) * (divisionYLvl +1) );
	
	
	// ==============  Custom algo, but won't cook afterwards. Dunno why =================
	// Vertex placement
//	for (int i = 0; i <= divisionXLvl; ++i)			// Loop on X axis (the <= is important so we reach left border)
//		for (int j = 0; j <= divisionYLvl; ++j)		// Loop on Y axis (the <= is important so we reach top border)
//			hide.vertex								// position a vertex
//			(
//				Vec3	// We are creating a cloth, so NxOgre vector is required
//				(		// Vertex are barycenters of corners, ponderated by i and j which itterate on X and Y from bottom right to top left
//					(
//						firstCornerPos	* (i*j)						 +					// firstCornerPos is reached when i == divisionXLvl and j == divisionYLvl
//						secondCornerPos * ((divisionXLvl - i) * j)	 +					// secondCornerPos is reached when i == 0 and j == divisionYLvl
//						thirdCornerPos  * (i * (divisionYLvl - j))	 +					// thirdCornerPos is reached when i == divisionXLvl and j == 0
//						fourthCornerPos * ((divisionXLvl - i) * (divisionYLvl - j))		// fourthCornerPos is reached when i == 0 and j == 0
//					)
//					/(divisionXLvl * divisionYLvl)										// divide by the sum of pounds  (simplified expression)
//				)
//			);
//
//	// triangle definition				
//	/*	first triangles like that :		 /|				<- row p+1
//										/ |
//									   /__|				<- row p																*/
//	//								 q+1   q
//	for (int i = 0; i < (divisionXLvl) * (divisionYLvl +1); ++i)				// Loop on all columns exept last: (divisionXLvl) to avoid q+1 goes farther than left border  and on all row (divisionYLvl +1) 
//		if ( (i/ (divisionYLvl +1)) != floor(i/ (divisionYLvl +1)))	// draw only if i isn't divisible by (divisionYLvl +1) (ie if the i th vertex isnt on topborder)
//			hide.triangle(i, i+1, i+divisionYLvl+1); // draw triangle between the i th vertex, the one above him (which exists because of the above if), and the vertex on his left (which exist because we //don't loop on last column)
//	
//	//                                _____
//	/*	now the complementary :		  |  /				<- row p+1
//									  | /
//									  |/				<- row p																	 */
//	//								 q+1   q
//	for (int i = 0; i < (divisionXLvl) * (divisionYLvl +1); ++i)				// Loop on all columns exept last: (divisionXLvl) to avoid q+1 goes farther than left border  and on all row (divisionYLvl +1) 
//		if ( (i/ (divisionYLvl +1)) != floor(i/ (divisionYLvl +1)))	// draw only if i isn't divisible by (divisionYLvl +1) (ie if the i th vertex isnt on topborder)
//			hide.triangle(i+1, i+divisionYLvl+1, i+divisionYLvl+2); // draw triangle between	the i+1 th vertex, 
//																	//							the one on his bottom left (left exists because we don't loop on last column, and bottom exists because the i th vertex isnt on topborder thus the i+1 th one isn't on bottomborder )
//																	//							and the one on his left (left exists because we don't loop on last column)
//
//	// texture coordinates generation
//	for (int i = 0; i <= divisionXLvl; ++i)			// Loop on X axis (the <= is important so we reach left border)
//		for (int j = 0; j <= divisionYLvl; ++j)		// Loop on Y axis (the <= is important so we reach top border)
//			hide.textureCoordinate(i/divisionXLvl, j/divisionYLvl); // grows from (0,0) at the bottom right corner to (1,1) at the top left corner 


	//code from NxOgreMeshGenerator.cpp
	 int i, j, i0, i1, i2, i3;
	 // Vertices
	 for (i = 0; i <= divisionYLvl; i++)
	 {
	  for (j = 0; j <= divisionXLvl; j++)
	   {
		hideMesh.vertex(
						Vec3	// We are creating a cloth, so NxOgre vector is required
						(		// Vertex are barycenter of corners, ponderated by i and j which iterate on X and Y from bottom right to top left
							(
								cornerPos[0]	* (i*j)						 +					// firstCornerPos is reached when i == divisionXLvl and j == divisionYLvl
								cornerPos[1] * ((divisionXLvl - i) * j)	 +					// secondCornerPos is reached when i == 0 and j == divisionYLvl
								cornerPos[2]  * (i * (divisionYLvl - j))	 +					// thirdCornerPos is reached when i == divisionXLvl and j == 0
								cornerPos[3] * ((divisionXLvl - i) * (divisionYLvl - j))		// fourthCornerPos is reached when i == 0 and j == 0
							)
							/(divisionXLvl * divisionYLvl)										// divide by the sum of pounds  (simplified expression)
						)
					);
	   }
	 }
		 
		 // Triangles 
		 for (i = 0; i < divisionYLvl; i++)
		 {
		  for (j = 0; j < divisionXLvl; j++)
		  {

		   i0 = i * (divisionXLvl+1) + j;
		   i1 = i0 + 1;
		   i2 = i0 + (divisionXLvl+1);
		   i3 = i2 + 1;

		   if ((j+i)%2)
		   {
			hideMesh.triangle(i2, i0, i1);
			hideMesh.triangle(i2, i1, i3);

			hideMesh.triangle(i0, i2, i1);
			hideMesh.triangle(i2, i3, i1);
		   }
		   else
		   {
			hideMesh.triangle(i2, i0, i3);
			hideMesh.triangle(i0, i1, i3);

			hideMesh.triangle(i0, i2, i3);
			hideMesh.triangle(i1, i0, i3);
		   }
		  }
		 }
  
		  float dx = divisionXLvl > 0 ? (1.0f / float(divisionXLvl)) : 1.0f;
		  float dy = divisionYLvl > 0 ? (1.0f / float(divisionYLvl)) : 1.0f;
  
		  for (i = 0; i <= divisionYLvl; i++)
		  {
		   for (j = 0; j <= divisionXLvl; j++)
		   {
			hideMesh.textureCoordinate(j*dx, i * dy);
		   }
		  }

	// algorithm finished, so we cook the mesh
	NxOgre::Mesh* mClothMesh = hideMesh.end();				
	hideMesh.clean();
	PhysX_descr.mMesh = mClothMesh;

	// now this description is cooked.
	isCooked = true;

	return true;
}

Hide::Hide(HideDescription* param)
{
	// We aren't based on a rigid body, because hides are soft
	rigidBodyBasedFlag = false;
	name = param->name;

	if (!param->isCooked)
	{
		ldbg.send("(Hide Constructor) Warning : param isn't cooked ( param.name == " + param->name + "), cook it to improve constructor performance.");
		param->Cook();
	}

	descr = *param;

	cloth = PhysX_man.mRenderSystem->createCloth(param->PhysX_descr,"hide");
	
	cloth->attachToCollidingShapes(NxOgre::Enums::ClothAttachmentFlags_Twoway);
	cloth->setStretchingStiffness(1);		// the most stiff cloth possible, but the stiffness is much more influenced by the division level of the hide
	
	//if isWing, register is as a wing
	if(descr.isWing)
	{

		// don't reach this point

		// first, roughly compute the wing's area :
		ldbg.send("registering wing with a surface of : " + toOgStr(descr.surface));
	
		//second, assemble a list of bodies to link
		std::vector<Critter::Body*>* attachedBodiesPtr =  new std::vector<Critter::Body*>;
		attachedBodiesPtr->push_back(descr.linkedBodies[0]);
		attachedBodiesPtr->push_back(descr.linkedBodies[1]);
		attachedBodiesPtr->push_back(descr.linkedBodies[2]);
		attachedBodiesPtr->push_back(descr.linkedBodies[3]);
	
		//finally register it
		PhysX_Wings_man.add(cloth, descr.surface, attachedBodiesPtr);
	}
	
	//if isBounce, register it as a trampoline
	if(descr.isBounce)
	{
		ldbg.send("registering trampoline");
		PhysX_Trampoline_man.add(cloth);
	}
	
}


NamedDataPool* Hide::Serialize()
{
	NamedDataPool* out = new NamedDataPool();

	out->Name = "Hide";

	out->String_Data.push_back(NamedString("name", name));
	out->String_Data.push_back(NamedString("materialName", descr.materialName));

	// look for constructions elements names of linkedbodies
	for (Ogre::uint j = 0; j < descr.container->element.size(); ++j)
		if (descr.linkedBodies[0] == static_cast<ConstructionRigidElement*>(descr.container->element[j])->physXBody)
			out->String_Data.push_back(NamedString("linkedBody1",  descr.container->element[0]->name));
	for (Ogre::uint j = 0; j < descr.container->element.size(); ++j)
		if (descr.linkedBodies[1] == static_cast<ConstructionRigidElement*>(descr.container->element[j])->physXBody)
			out->String_Data.push_back(NamedString("linkedBody2",  descr.container->element[1]->name));
	for (Ogre::uint j = 0; j < descr.container->element.size(); ++j)
		if (descr.linkedBodies[2] == static_cast<ConstructionRigidElement*>(descr.container->element[j])->physXBody)
			out->String_Data.push_back(NamedString("linkedBody3",  descr.container->element[2]->name));
	for (Ogre::uint j = 0; j < descr.container->element.size(); ++j)
		if (descr.linkedBodies[3] == static_cast<ConstructionRigidElement*>(descr.container->element[j])->physXBody)
			out->String_Data.push_back(NamedString("linkedBody4",  descr.container->element[3]->name));


	out->Vector3_Data.push_back(NamedVector3("corner1Position", descr.cornerPos[0]));
	out->Vector3_Data.push_back(NamedVector3("corner2Position", descr.cornerPos[1]));
	out->Vector3_Data.push_back(NamedVector3("corner3Position", descr.cornerPos[2]));
	out->Vector3_Data.push_back(NamedVector3("corner4Position", descr.cornerPos[3]));

	out->Int_Data.push_back(NamedInt("widthDivisions", descr.widthDivisions));
	out->Int_Data.push_back(NamedInt("heightDivisions", descr.heightDivisions));

	out->Float_Data.push_back(NamedFloat("surface", descr.surface));

	out->Bool_Data.push_back(NamedBool("isWing", descr.isWing));
	out->Bool_Data.push_back(NamedBool("isBounce", descr.isBounce));

	return out;
}

Hide* Hide::MakeFromDataPool(NamedDataPool&)
{
	HideDescription loadedDesc;



	return NULL;
}



















































































