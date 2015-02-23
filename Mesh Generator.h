/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef FILE_MESH_GENERATOR// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define FILE_MESH_GENERATOR

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16); // Build a sphere
#endif // FILE_MESH_GENERATOR


