#include "Namuh_Main_H.h"

using namespace Ogre;

FileSerialiser File_Serialiser;

// Code des méthodes de la classe
FileSerialiser::FileSerialiser(void)
{
}
FileSerialiser::~FileSerialiser(void)
{
}

void FileSerialiser::WriteFile(std::vector<NamedDataPool*> Data, string FileName)
{
	ofstream file (&FileName[0], ios::out | ios::trunc);
	if(!file)
		ldbg.send("(File Serialiser Exeption) Unable to open "+ FileName +", abording...");
	else
	{
		for(	std::vector<NamedDataPool*>::iterator DataIterator = Data.begin();
				DataIterator != Data.end();
				++DataIterator)
			file << SerialisePool(*DataIterator) << endl;
		file.close();
		ldbg.send("(File Serialiser info) File "+FileName+" sucessfully written ");
	}

}

inline std::string FileSerialiser::SerialisePool(NamedDataPool* Pool, string lineprefix)
{
	string	out = "",
			PreviousLevelineprefix  = lineprefix;
	lineprefix += "	";

	out += PreviousLevelineprefix + getOpenBaliseStr(Pool->Name);

	for(	std::vector<NamedBool>::iterator DataIterator = Pool->Bool_Data.begin();
			DataIterator != Pool->Bool_Data.end();
			++DataIterator)
		out += lineprefix + getAffectStr((*DataIterator).Name) + StringConverter::toString((*DataIterator).data) + "\n";

	for(	std::vector<NamedInt>::iterator DataIterator = Pool->Int_Data.begin();
			DataIterator != Pool->Int_Data.end();
			++DataIterator)
		out += lineprefix + getAffectStr((*DataIterator).Name) + StringConverter::toString((*DataIterator).data) + "\n";

	for(	std::vector<NamedFloat>::iterator DataIterator = Pool->Float_Data.begin();
			DataIterator != Pool->Float_Data.end();
			++DataIterator)
		out += lineprefix + getAffectStr((*DataIterator).Name) + StringConverter::toString((*DataIterator).data) + "\n";

	for(	std::vector<NamedVector3>::iterator DataIterator = Pool->Vector3_Data.begin();
			DataIterator != Pool->Vector3_Data.end();
			++DataIterator)
		out += lineprefix + getAffectStr((*DataIterator).Name) + StringConverter::toString((*DataIterator).data) + "\n";

	for(	std::vector<NamedString>::iterator DataIterator = Pool->String_Data.begin();
			DataIterator != Pool->String_Data.end();
			++DataIterator)
		out += lineprefix + getAffectStr((*DataIterator).Name) + (*DataIterator).data + "\n";

	for(	std::vector<NamedDataPool>::iterator SubPoolIterator = Pool->SubPool.begin();
			SubPoolIterator != Pool->SubPool.end();
			++SubPoolIterator)
		out += SerialisePool(&(*SubPoolIterator), lineprefix);

	out += PreviousLevelineprefix + getCloseBaliseStr(Pool->Name);

	return out;
}

inline std::string FileSerialiser::getOpenBaliseStr(string BaliseName)
{
	return "["+BaliseName+"]\n";
}

inline std::string FileSerialiser::getCloseBaliseStr(string BaliseName)
{
	return "[/"+BaliseName+"]\n";
}

inline std::string FileSerialiser::getAffectStr(string DataName)
{
	return DataName+" = ";
}
 





















































































































