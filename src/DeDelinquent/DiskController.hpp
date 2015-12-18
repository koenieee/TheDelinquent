#pragma once

#include <string>
#include "rapidjson/document.h"
#include <map>
#include <vector>

/**
* The DiskController makes it possible to read a json into a list of text values.
* @version 1.0
*/

class DiskController{
public:

	/**
	* Reads a file into a new JSON Document
	*
	* @param fileName the filename of the json file the DiskController has to read.
	* @return a pointer to the generated JSON Document
	*/
	static rapidjson::Document * DiskController::readFileIntoJson(std::string fileName);

	/**
	* Print a JSON value from the specified key and JSON Document
	*
	* @param theDoc is the JSON generated Document, use function readFileIntoJson to generate this
	* @param key the specified key connected to the value.
	*/
	static void DiskController::printJsonObject(rapidjson::Document & theDoc, const char * Key);

	/**
	* Returns one JSON value according to the specified key
	*
	* @param theDoc is the JSON generated Document, use function readFileIntoJson to generate this
	* @param key the specified key connected to the value.
	* @return string containing the JSON value specified by the key
	*/
	static std::string * DiskController::returnOneObject(rapidjson::Document & theDoc, const char * Key);

	/**
	* Returns a map of JSON values and key names
	*
	* @param theDoc is the JSON generated Document, use function readFileIntoJson to generate this
	* @param listOfKeys is the vector list containing a list of keys to get the JSON value from.
	* @return a string map containing the JSON KEY, and JSON VALUE
	*/
	static std::map<std::string, std::string *> * DiskController::returnJsonObjects(rapidjson::Document & theDoc, std::vector<std::string> & listOfKeys);
	 
private:

};