#include "jsonParser.hpp"
#include "jsonValue.hpp"
#include <fstream>
#include <string.h>
#include <stdlib.h>

int jsonSave(void* json, const char* path) {
	jsonValue<> &obj = *(jsonValue<>*)json;
	std::string tmp = obj.serialize();
	FILE* f = fopen(path, "w");
	fwrite(tmp.c_str(), 1, tmp.length(), f);
	fclose(f);
	return 0;
}
int jsonParse(void* json,const char* path,int type) {
	if (json)
	{
		jsonValue<> &obj = *(jsonValue<>*)json;
		jsonParser<> jpr(obj);
		if (type) {
			return jpr.parseFile(path);
		}
		else {
			return jpr.parseBuff(path);
		}
	}
	return -1;
}
void * jsonCreate()
{
	jsonValue<>* obj = new jsonValue<>();
	return obj;
}
void jsonFree(void* json)
{
	delete (jsonValue<>*)json;
}
const char* jsonGetString(void* json,const char* path) {
	return ((jsonValue<>*)json)->find(path).tostring().c_str();
}
double jsonGetNumber(void* json,const char* path) {
	return ((jsonValue<>*)json)->find(path).tonumber();
}
const char* jsonToString(void* json) {
	return ((jsonValue<>*)json)->tostring().c_str();
}
double jsonToNumber(void* json) {
	return ((jsonValue<>*)json)->tonumber();
}
int jsonSetString(void* json,const char* path,const char* value) {
	jsonValue<>& obj=*(jsonValue<>*)json;
	switch (obj.getType())
	{
	case type_object:
		break;
	case type_undefined:
		obj = jsonValue<>(jsonValue<>::jsonObject());
		break;
	default:
		return -1;
		break;
	}
	
	jsonValue<>& tmp=obj.find(path);
	switch (tmp.getType())
	{
	case type_string:
		tmp = jsonValue<>(value);
		break;
	case type_null:
		obj.insert(path, jsonValue<>(value));
		break;
	default:
		return -2;
		break;
	} 
	return 0;
}
int jsonSetNumber(void* json,const char* path,double value) {
	jsonValue<>& obj = *(jsonValue<>*)json;
	switch (obj.getType())
	{
	case type_object:
		break;
	case type_undefined:
		obj = jsonValue<>(jsonValue<>::jsonObject());
		break;
	default:
		return -1;
		break;
	}

	jsonValue<>& tmp = obj.find(path);
	switch (tmp.getType())
	{
	case type_string:
		tmp = jsonValue<>(value);
		break;
	case type_null:
		obj.insert(path, jsonValue<>(value));
		break;
	default:
		return -2;
		break;
	}
	return 0;
}
int jsonAddArray(void* json, const char* path) {
	jsonValue<>& obj = *(jsonValue<>*)json;
	switch (obj.getType())
	{
	case type_object:
		break;
	case type_undefined:
		obj = jsonValue<>(jsonValue<>::jsonObject());
		break;
	default:
		return -1;
		break;
	}
	jsonValue<>& tmp=obj.find(path);
	switch (tmp.getType())
	{
	case type_null:
		{
			obj.insert(path, jsonValue<>(jsonValue<>::jsonArray()));
		}
		break;
	case type_array:
		break;
	default:
		return -2;
		break;
	}
	return 0;
}
void* jsonGetValue(void* json, const char* path) {
	jsonValue<>& obj = *(jsonValue<>*)json;
	return &obj.find(path);
}
int jarrPushString(void* json, const char* value) {
	jsonValue<>& obj = *(jsonValue<>*)json;
	if (obj.pushBack(jsonValue<>(value)).getType()!=type_null) {
		return 0;
	}
	else {
		return -1;
	}
}
int jarrPushNumber(void* json, double value) {
	jsonValue<>& obj = *(jsonValue<>*)json;
	if (obj.pushBack(jsonValue<>(value)).getType() != type_null) {
		return 0;
	}
	else {
		return -1;
	}
}
void* jarrGetValue(void* json, int idx) {
	jsonValue<>& obj = *(jsonValue<>*)json;
	return &obj.at(idx);
}

