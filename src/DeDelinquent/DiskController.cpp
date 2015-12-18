#include "rapidjson/document.h"
#include "DiskController.hpp"
#include <fstream>
#include <map>
#include <vector>

using namespace rapidjson;

Document * DiskController::readFileIntoJson(std::string fileName) {
	std::string line, text;
	std::ifstream in(fileName);
	while (std::getline(in, line))
	{
		text += line + "\n";
	}
	const char* json = text.c_str();
	Document * document = new Document;
	document->Parse(json);
	return document;
}

std::string * DiskController::returnOneObject(Document & theDoc, const char * Key){
	assert(theDoc.HasMember(Key));
	assert(theDoc[Key].IsString());
	std::string * theString = new std::string(theDoc[Key].GetString());

	return theString;
}

void DiskController::printJsonObject(Document & theDoc, const char * Key){
	assert(theDoc.HasMember(Key));
	assert(theDoc[Key].IsString());
	printf("%s = %s\n", Key, theDoc[Key].GetString());
}

std::map<std::string, std::string *> * DiskController::returnJsonObjects(
		Document & theDoc, std::vector<std::string> & listOfKeys){
	//DiskController dsk;
	std::map<std::string, std::string *> * theMap = new std::map<std::string, std::string *>;
	
	for (std::string theString : listOfKeys){
		theMap->insert(std::pair<std::string, std::string *>(theString, 
			DiskController::returnOneObject(theDoc, theString.c_str())));
		//theMap->insert[theString] = ;
	}
	return theMap;
}