#include "Namuh_Main_H.h"

using namespace Ogre;
MapParser Map_Parser;


// Code des méthodes de la classe
MapParser::MapParser(void)
{	
}
MapParser::~MapParser(void)
{
}

void MapParser::initialise(void)
{ 
	
}


std::vector<NamedDataPool> MapParser::ParseFile(std::vector<ParserBalise*> Syntax,string File,std::vector<string> ExcludedInclusions, string StartingBalise)
{
	// Espace mémoire du parser
	std::vector<NamedDataPool> Output;
	NamedDataPool* CurrentPool = NULL;
	std::vector<ParserBalise*> ActiveBalise;
	ActiveBalise.push_back(NULL);
	Ogre::uint no_ligne = 0;

	// On sépare le nom du fichier de son chemin d'acces
	string FileWOpath = File;
	FileWOpath.replace(0,FileWOpath.rfind("/")+1,"");
	string path = File;
	path.replace(path.rfind("/")+1,path.length()-path.rfind("/"),"");
	
	ldbg.send("(Map Parser Info) Parsing file : "+ FileWOpath );

	//entrée dans la balise de départ spécifié
	if(StartingBalise != "")
	{
		while(StartingBalise != "")
		{
			string CurrentBaliseToAdd = StartingBalise;

			StartingBalise.replace(0,StartingBalise.find("/")+1,"");//on enlève la balise que l'on considère de la pile a considérer
			CurrentBaliseToAdd.replace(CurrentBaliseToAdd.find("/"),CurrentBaliseToAdd.length()-CurrentBaliseToAdd.find("/"),""); // on ne considère que celle que l'on a enlevée
			if(ActiveBalise.back() == NULL)
				ActiveBalise.push_back(Syntax[FindBaliseIndexFromName(Syntax,CurrentBaliseToAdd)]); //On ajoute la balise
			else 
				ActiveBalise.push_back(&ActiveBalise.back()->SousBalises[FindBaliseIndexFromName(ActiveBalise.back()->SousBalises,CurrentBaliseToAdd)]); // Pareil, masi c'est juste que le protocola dffère si on est pas a la première balise
		}
	ldbg.send("(Map Parser Info) Initial Balise : "+ ActiveBalise.back()->Name );
	}

	ifstream file (&File[0],ios::in);
	if(!file)ldbg.send("(Map Parser Exception) Unable to open "+ FileWOpath +", abording...");
	else
	{

		string	balise = "",
				ligne = "",
				lignebis = "",
				variable = "",
				valeur = "",
				explicitType = "";
		bool ligneType_reconue = false,
			 foreignKeyword = false;	//tells if variable is explicitly typed. Which enable foreign keyword to be read.
		while(getline(file,ligne))
		{
			no_ligne++;
			ligneType_reconue = false;
			if(ligne != "" && ligne.find_first_not_of("	 ") < ligne.length()) // Si la ligne du fichier mérite d'être lue
			{
				if(ligne[ligne.find_first_not_of("	 ")] != '#') // Si la ligne n'est pas vide, on doit vérifier que l'on a pas affaire a un commentaire
				{
					if(ligne.find("#") < ligne.length())
						ligne.replace(ligne.find("#"),ligne.length()-ligne.find("#"),""); //on nettoie les commentaires de fin de ligne

					lignebis = ligne; // on recopie dans lignebis car on a besoin d'une deuxième string pour la valeur de la variable
					if(ligne.find("=") < ligne.length() && !ligneType_reconue) //Si c'est une affectation de variable
					{
						ligneType_reconue = true;

						if(ligne.find(":") < ligne.length())	// If type is explicitly given 
						{
							foreignKeyword = true;
							explicitType = ligne;
							explicitType = explicitType.replace(explicitType.find(":"),explicitType.length()-explicitType.find(":"),"");
							// get rid of Type specification in ligne.
							ligne.replace(0,ligne.find(":")+1,"");
							// Nettoyer explicitType des espaces.
							while(variable.find(" ")<= explicitType.length())
								explicitType.replace(explicitType.find(" "),1,""); // On fait sauter tout les espaces dans le nom du type			
							while(variable.find("	")<= explicitType.length())
								explicitType.replace(explicitType.find("	"),1,""); // On fait sauter tout les tab dans le nom du type

						}
						else
							explicitType = "";

						variable = ligne.replace(ligne.find("="),ligne.length()-ligne.find("="),"");
							while(variable.find(" ")<= variable.length())
								variable.replace(variable.find(" "),1,""); // On fait sauter tout les espaces dans le nom de la variable			
							while(variable.find("	")<= variable.length())
								variable.replace(variable.find("	"),1,""); // On fait sauter tout les tab dans le nom de la variable

						valeur = lignebis.replace(0,lignebis.find("=")+1,"");
							if(valeur.find(" ")<= valeur.find_first_not_of(" "))
								valeur.replace(0,valeur.find_first_not_of(" "),""); // On fait sauter tout les espaces avant la valeur
							if(valeur.rfind(" ")>= valeur.find_last_not_of(" "))
								valeur.replace(valeur.find_last_not_of(" ")+1,valeur.length()-valeur.find_last_not_of(" ")-1,""); // On fait sauter tout les espaces après la valeur
							if(valeur.find("	")<= valeur.find_first_not_of("	"))
								valeur.replace(0,valeur.find_first_not_of("	"),""); // On fait sauter tout les tab avant la valeur
							if(valeur.rfind("	")>= valeur.find_last_not_of("	"))
								valeur.replace(valeur.find_last_not_of("	")+1,valeur.length()-valeur.find_last_not_of("	")-1,""); // On fait sauter tout les tab après la valeur
						
						bool affected = false;

						//Affectation
						if (explicitType == "")// If keyword is known
						{
							// Bool keyword
							for(Ogre::uint t=0;t<ActiveBalise.back()->BoolKeywords.size();++t)
								if(variable == ActiveBalise.back()->BoolKeywords[t])
								{
									struct NamedBool mNamedBool(variable,StringConverter::parseBool(valeur));
									CurrentPool->Bool_Data.push_back(mNamedBool);
									affected = true;
									//ldbg.send("(Map Parser Info)Registered variable '" + variable + "' with value '"+valeur+"'" );
								}

							// Number keyword
							if(StringConverter::isNumber(valeur))
							{
								// Int keyword
								for(Ogre::uint t=0;t<ActiveBalise.back()->IntKeywords.size();++t)
									if(variable == ActiveBalise.back()->IntKeywords[t])
									{
										struct NamedInt mNamedInt(variable,StringConverter::parseInt(valeur));
										CurrentPool->Int_Data.push_back(mNamedInt);
										affected = true;
										//ldbg.send("(Map Parser Info)Registered variable '" + variable + "' with value '"+valeur+"'" );
									}

								// Float keyword
								for(Ogre::uint t=0;t<ActiveBalise.back()->FloatKeywords.size();++t)
									if(variable == ActiveBalise.back()->FloatKeywords[t])
									{
										struct NamedFloat mNamedFloat(variable,StringConverter::parseReal(valeur));
										CurrentPool->Float_Data.push_back(mNamedFloat);
										affected = true;
										//ldbg.send("(Map Parser Info)Registered variable '" + variable + "' with value '"+valeur+"'" );
									}
							}

							// Vector3 keyword
							for(Ogre::uint t=0;t<ActiveBalise.back()->Vector3Keywords.size();++t)
								if(variable == ActiveBalise.back()->Vector3Keywords[t])
								{
									struct NamedVector3 mNamedVector3(variable,StringConverter::parseVector3(valeur));
									CurrentPool->Vector3_Data.push_back(mNamedVector3);
									affected = true;
									//ldbg.send("(Map Parser Info)Registered variable '" + variable + "' with value '"+valeur+"'" );
								}

							// String Keyword
							for(Ogre::uint t=0;t<ActiveBalise.back()->StringKeywords.size();t++)
								if(variable == ActiveBalise.back()->StringKeywords[t])
								{
									struct NamedString mNamedString(variable,valeur);
									CurrentPool->String_Data.push_back(mNamedString);
									affected = true;
									//ldbg.send("(Map Parser Info)Registered variable '" + variable + "' with value '"+valeur+"'" );
								}


							//Si variable inconnue, erreur
								if(!affected)ldbg.send("(Map Parser Exeption) Unable to find in "+ balise +" the variable named '" + variable + "' ("+valeur+")"+"  (line " + toOgStr(no_ligne)+ " in "+FileWOpath+")" );
						}
						else	// Type is explicitly defined, so affect it 
						{
							if (explicitType == "bool")
								CurrentPool->Bool_Data.push_back(*(new NamedBool(variable, StringConverter::parseBool(valeur))));
							else if (explicitType == "int")
								CurrentPool->Int_Data.push_back(*(new NamedInt(variable, StringConverter::parseInt(valeur))));
							else if (explicitType == "float")
								CurrentPool->Float_Data.push_back(*(new NamedFloat(variable, StringConverter::parseReal(valeur))));
							else if (explicitType == "vector3")
								CurrentPool->Vector3_Data.push_back(*(new NamedVector3(variable, StringConverter::parseVector3(valeur))));
							else if (explicitType == "string")
								CurrentPool->String_Data.push_back(*(new NamedString(variable,valeur)));
						}
					}
					if(!ligneType_reconue)
					{
						
						if(ligne.find(" ")<= ligne.find_first_not_of(" "))
							ligne.replace(0,ligne.find_first_not_of(" "),""); // On fait sauter tout les espaces avant la commande
						if(ligne.rfind(" ")>= ligne.find_last_not_of(" "))
							ligne.replace(ligne.find_last_not_of(" ")+1,ligne.length()-ligne.find_last_not_of(" ")-1,""); // On fait sauter tout les espaces après la commande
						if(ligne.find("	")<= ligne.find_first_not_of("	"))
							ligne.replace(0,ligne.find_first_not_of("	"),""); // On fait sauter tout les tab avant la commande
						if(ligne.rfind("	")>= ligne.find_last_not_of("	"))
							ligne.replace(ligne.find_last_not_of("	")+1,ligne.length()-ligne.find_last_not_of("	")-1,""); // On fait sauter tout les tab après la commande

						if(ligne[0] == '[' && ligne[ligne.length()-1] == ']' ) // Si on a affaire a une déclaration de balise
						{
							if(ligne[1] != '/') // Si on se sort pas d'une balise (si on y rentre)
							{
								string balise_a_entrer = ligne;
								balise_a_entrer.replace(0,1,""); // on enlève le 1er crochet
								balise_a_entrer.replace(balise_a_entrer.length()-1,1,""); // on enlève le 2nd crochet
								if(balise != "")balise += "/" + balise_a_entrer;
								else balise += balise_a_entrer;

								// entrée en balise
								if(CurrentPool == NULL) 
								{
									NamedDataPool mNamedDataPool = {balise_a_entrer,NULL};
									Output.push_back(mNamedDataPool);
									CurrentPool = &Output.back();
									if(ActiveBalise.back() == NULL)
										ActiveBalise.push_back(Syntax[FindBaliseIndexFromName(Syntax,balise_a_entrer)]);
									else
										ActiveBalise.push_back(&ActiveBalise.back()->SousBalises[FindBaliseIndexFromName(ActiveBalise.back()->SousBalises,balise_a_entrer)]);

								}
								else 
								{
									NamedDataPool mNamedDataPool; mNamedDataPool.Name = balise_a_entrer; mNamedDataPool.ParentPool = CurrentPool;
									CurrentPool->SubPool.push_back(mNamedDataPool);
									CurrentPool = &CurrentPool->SubPool.back();
									if(ActiveBalise.back() == NULL)
										ActiveBalise.push_back(Syntax[FindBaliseIndexFromName(Syntax,balise_a_entrer)]);
									else
										// une mauvaise syntaxe apour unfichier s=risque de faire bugguer le parser ici (parser non sécurisé sur la cohérence fichier/syntaxe)
										ActiveBalise.push_back(&ActiveBalise.back()->SousBalises[FindBaliseIndexFromName(ActiveBalise.back()->SousBalises,balise_a_entrer)]);
								}
								//ldbg.send("Entrée dans la balise : "+ balise_a_entrer + " (Now "+balise+")");


							}
							else //si on sort d'une balise
							{
								
								string balise_a_quitter = ligne;
									balise_a_quitter.replace(0,2,""); // on enlève le 1er crochet et le slash
									balise_a_quitter.replace(balise_a_quitter.length()-1,1,""); // on enlève le 2nd crochet

								string Current_lowest_balise = balise;
									if(balise != balise_a_quitter) // Si on ne ferme pas la dernière balise, il faut extraire le nom de la plus basse du chemin total
										Current_lowest_balise = Current_lowest_balise.replace(0,Current_lowest_balise.rfind("/")+1,""); // On enlève les balises supérieures
									
								if(balise_a_quitter ==  Current_lowest_balise) // si on ferme bien la bonne balise
								{
									if(balise != balise_a_quitter) //Si on sors d'une sous balise
									{
										balise.replace(balise.rfind(balise_a_quitter)-1,balise_a_quitter.length()+1,"");
										CurrentPool = CurrentPool->ParentPool;
										ActiveBalise.pop_back();
										//ldbg.send("Sortie de la balise : "+ balise_a_quitter + " (Now "+balise+")");
									}
									else // Si l'on revient a la racine
									{
										CurrentPool = NULL;
										ActiveBalise.pop_back();
										//ldbg.send("Sortie de la balise : "+ balise_a_quitter + " (Now the global dommain)");
									}
								}
								else
								{
									ldbg.send("(Map Parser Exeption) Unable to exit balise : "+balise_a_quitter+"isn't the lowest one "+"(line " + toOgStr(no_ligne) + " in "+FileWOpath+")");
								}
							}
						}

						if(ligne[0] == '<' && ligne[ligne.length()-1] == '>' ) // Si on a affaire a une inclsion de fichier
						{
							// On nettoie la ligne
							string fichier_a_parser = ligne;
							fichier_a_parser.replace(0,1,""); // on enlève le >
							fichier_a_parser.replace(fichier_a_parser.length()-1,1,""); // on enlève le <
							ldbg.send("(Map Parser Info) Subfile parsing requested : "+fichier_a_parser);

							// Mise en String de la balise courrante
							string Current_Balise_position = "";
							for(int t = 1;t != ActiveBalise.size();t++) // ActiveBalise[0] est NULL
								Current_Balise_position += ActiveBalise[t]->Name+"/";// On génère le path des balises dans une string
							ldbg.send("(Map Parser Info) Subfile parsing active balise path : "+Current_Balise_position);

							// Is there any looping recusion in file inclusion ?
							std::vector<string> CurrentExclusion = ExcludedInclusions;
							CurrentExclusion.push_back(FileWOpath);
							bool skipflag = false;
							for(int t = 0;t != CurrentExclusion.size();t++)
								if(fichier_a_parser == CurrentExclusion[t])
									skipflag=true;

							if(!skipflag) 	
							{
								// Parse le fichier demandé
								std::vector<NamedDataPool> PoolsToImport = MapParser::ParseFile(Syntax, path+fichier_a_parser,CurrentExclusion,Current_Balise_position);
								for(int t = 0;t != PoolsToImport.size();t++)
									CurrentPool->SubPool.push_back(PoolsToImport[t]);
							}
							else // Ah bah non
								ldbg.send("(Map Parser Exeption) Detected looping recursion in this file inclusion, skippig... (in "+ FileWOpath +", ligne : "+ StringConverter::toString(no_ligne) +", "+fichier_a_parser+")");

						}
					}
				}
			}
		}
		file.close();
		ldbg.send("File "+FileWOpath+" sucessfully parsed "+"(Nblines :  " + toOgStr(Ogre::uint(no_ligne))+")");
	}
	return Output;
};

void MapParser::destroy(void)
{ 
	
}
// /!\ on parcourt toute la liste de *_Data a l'envert. Comme ca la dernière valeur fournie est retenue

bool BoolFromName(NamedDataPool SearchScope, string Name)
{
	for(int t = SearchScope.Bool_Data.size()-1;t >= 0;--t)
		if(Name == SearchScope.Bool_Data[t].Name)
			return SearchScope.Bool_Data[t].data;
	
	ldbg.send("(BoolFromNameExeption) Error while retriving bool : No such NamedBool. Will return false. ( Requested : " + Name + " in the pool named : " + SearchScope.Name + " )" );
	return false;
};



int IntFromName(NamedDataPool SearchScope, string Name)
{
	for(int t = SearchScope.Int_Data.size()-1;t >= 0;--t)
		if(Name == SearchScope.Int_Data[t].Name)
			return SearchScope.Int_Data[t].data;

	ldbg.send("(IntFromNameExeption) Error while retriving int : No such NamedInt.  Will return  -1. ( Requested : " + Name + " in the pool named : " + SearchScope.Name + " )" );
	return -1;
};



float FloatFromName(NamedDataPool SearchScope, string Name)
{
	for(int t = SearchScope.Float_Data.size()-1;t>= 0;t--)
		if(Name == SearchScope.Float_Data[t].Name)
			return SearchScope.Float_Data[t].data;
	
	ldbg.send("(FloatFromNameExeption) Error while retriving float : No such NamedFloat.  Will return  -1. ( Requested : " + Name + " in the pool named : " + SearchScope.Name + " )" );
	return -1;
};



Ogre::Vector3 Vector3FromName(NamedDataPool SearchScope, string Name)
{
	for(int t = SearchScope.Vector3_Data.size()-1;t >= 0;--t)
		if(Name == SearchScope.Vector3_Data[t].Name)
			return SearchScope.Vector3_Data[t].data;
	
	ldbg.send("(Vector3FromNameExeption) Error while retriving Vector3 : No such NamedVector3.  Will return  (0,0,0). ( Requested : " + Name + " in the pool named : " + SearchScope.Name + " )" );
	return Ogre::Vector3::ZERO;
};



string StringFromName(NamedDataPool SearchScope, string Name)
{
	for(int t = SearchScope.String_Data.size()-1;t >= 0;--t)
		if(Name == SearchScope.String_Data[t].Name)
			return SearchScope.String_Data[t].data;

	ldbg.send("(StringFromNameExeption) Error while retriving String : No such NamedString.  Will return  a blank string. ( Requested : " + Name + " in the pool named : " + SearchScope.Name + " )" );
	return "";
};



std::vector<int> SubPoolsIndexesFromName(NamedDataPool SearchScope, string Name)
{
	std::vector<int> table;
	for( Ogre::uint t = 0; t != SearchScope.SubPool.size() ; t++ )
		if(Name ==  SearchScope.SubPool[t].Name)
			table.push_back(t);

	//if(table.size()==0)ldbg.send("(SubPoolsIndexesFromNameExeption) Error while retriving SubPoolsIndexes : No such SubPool. Returning a blank list... ( Requested : " + Name + "in the pool named : " + SearchScope.Name + " )" );
	return table;
};

int FindPoolIndexFromName(std::vector<NamedDataPool> SearchScope, string Name)
{
	for(int t = SearchScope.size()-1;t>=0;t--)
		if(Name == SearchScope[t].Name)
			return t;

	ldbg.send("(FindPoolIndexFromNameExeption) Fatal Error while retriving pool index : No such pool in this array.  Will return  -1. ( Requested : " + Name +" )" );
	ldbg.send("(FindPoolIndexFromNameExeption) Warning : SEGFAULT inbound" );
	ldbg.send("(FindPoolIndexFromNameExeption) Dump : (Search scope lenth : "+StringConverter::toString(SearchScope.size())+" )" );
	for(Ogre::uint t = SearchScope.size()-1;t>=0;t--)
		ldbg.send("(FindPoolIndexFromNameExeption) SearchScope(" + StringConverter::toString(t) + ").Name : "+SearchScope[t].Name);
	return -1;
};


int FindBaliseIndexFromName(std::vector<ParserBalise*> SearchScope, string Name)
{
	for(int t = SearchScope.size()-1;t>=0;t--)
		if(Name == SearchScope[t]->Name)
			return t;

	ldbg.send("(FindBaliseIndexFromNameExeption) Fatal Error while retriving balise index : No such Balise in this array.  Will return  -1. ( Requested : " + Name +" )" );
	ldbg.send("(FindBaliseIndexFromNameExeption) Warning : SEGFAULT inbound" );
	ldbg.send("(FindBaliseIndexFromNameExeption) Dump : (Search scope lenth : "+StringConverter::toString(SearchScope.size())+" )" );
	for(Ogre::uint t = SearchScope.size()-1;t>=0;t--)
		ldbg.send("(FindBaliseIndexFromNameExeption) SearchScope(" + StringConverter::toString(t) + ")->Name : "+SearchScope[t]->Name);
	ldbg.send("(FindBaliseIndexFromNameExeption) exiting fct" );

	return -1;
};



int FindBaliseIndexFromName(std::vector<ParserBalise> SearchScope, string Name)
{
	for(int t = SearchScope.size()-1;t>=0;t--)
		if(Name == SearchScope[t].Name)
			return t;

	ldbg.send("(FindBaliseIndexFromNameExeption) Fatal Error while retriving balise index : No such Balise in this array.  Will return  -1. ( Requested : " + Name +" )" );
	ldbg.send("(FindBaliseIndexFromNameExeption) Warning : SEGFAULT inbound" );
	ldbg.send("(FindBaliseIndexFromNameExeption) Dump : (Search scope lenth : "+StringConverter::toString(SearchScope.size())+" )" );
	for(Ogre::uint t = SearchScope.size()-1;t>=0;t--)
		ldbg.send("(FindBaliseIndexFromNameExeption) SearchScope[" + StringConverter::toString(t) + "].Name : "+SearchScope[t].Name);

	return -1;
};





























































