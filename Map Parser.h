/**====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_MAP_PARSER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_MAP_PARSER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

struct ParserBalise
{
	string Name;

	std::vector<string> BoolKeywords,
						IntKeywords,
						FloatKeywords,
						Vector3Keywords,
						StringKeywords;

	std::vector<ParserBalise> SousBalises;
};
int FindBaliseIndexFromName(std::vector<ParserBalise*> SearchScope, string Name); //Fournit l'index de la dernière Balise ayant le nom demandé dans le tableau de Balises donné
int FindBaliseIndexFromName(std::vector<ParserBalise> SearchScope, string Name); //Fournit l'index de la dernière Balise ayant le nom demandé dans le tableau de Balises donné

// data containers : enable datas to be indexed with name
struct NamedBool	{NamedBool(string rqName, bool rqdata) : Name(rqName), data(rqdata){}; string Name;	bool data;};
struct NamedInt		{NamedInt(string rqName, int rqdata) : Name(rqName), data(rqdata){}; string Name;	int data;};
struct NamedFloat	{NamedFloat(string rqName, float rqdata) : Name(rqName), data(rqdata){}; string Name;	float data;};
struct NamedVector3	{NamedVector3(string rqName, Ogre::Vector3 rqdata) : Name(rqName), data(rqdata){}; string Name;	Ogre::Vector3 data;};
struct NamedString	{NamedString(string rqName, string rqdata) : Name(rqName), data(rqdata){}; string Name,	data;};

struct NamedDataPool
{
	string Name;
	NamedDataPool* ParentPool;

	std::vector<NamedBool> Bool_Data;
	std::vector<NamedInt> Int_Data;
	std::vector<NamedFloat> Float_Data; //private
	std::vector<NamedVector3> Vector3_Data;
	std::vector<NamedString> String_Data;
//public
	std::vector<NamedDataPool> SubPool;
};

//fonctions d'acces a la hiérarchie de sortie du parser
	// Attention, ces fonctions de recherches de valeurs parsés via leur noms ne chercherons pas dans les SubPools du SearchScope

//Ces fonctions fournissent la donnée ayant le nom demandé
bool BoolFromName(NamedDataPool SearchScope, string Name);
int IntFromName(NamedDataPool SearchScope, string Name);
float FloatFromName(NamedDataPool SearchScope, string Name); // faire qu'elles ne prennent en arg qu'un string donnant un chemin d'acces dans la hiérarchie avec la fin le nom de la variable, exemple : Map/Layer/Diffuse
Ogre::Vector3 Vector3FromName(NamedDataPool SearchScope, string Name);
string StringFromName(NamedDataPool SearchScope, string Name);

std::vector<int> SubPoolsIndexesFromName(NamedDataPool SearchScope, string Name); //Fournit le tabeau d'index des SubPools ayant le nom demandé // a mettre en fct memeblre
int FindPoolIndexFromName(std::vector<NamedDataPool> SearchScope, string Name); //Fournit l'index de la dernière pool ayant le nom demandé dans le tableau de Pools donné

class MapParser
{
  public:
	
    MapParser(void);
    ~MapParser(void);
    void initialise(void);
	std::vector<NamedDataPool> ParseFile(std::vector<ParserBalise*> FileSyntax, string FileName, std::vector<string> ExcludedInclusions = std::vector<string>(0),string StartingBalise = "");
	void destroy(void);
};
extern MapParser Map_Parser;

#endif // CLASS_PERSO




































































