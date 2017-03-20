#ifndef __DPZ__JSONVALUE__HPP__ 
#define __DPZ__JSONVALUE__HPP__

#include <map>
#include <list>
#include <string>
#include <vector>
#include <iostream>

enum json_type {
	type_undefined,
	type_string,
	type_number,
	type_array,
	type_object,
	type_null,
};
class jsonValue;
typedef std::map<std::string, jsonValue> jsonObject;
typedef std::vector<jsonValue> jsonArray;
class jsonValue {
public:
	jsonValue& operator=(jsonValue& val);
	jsonValue();
	jsonValue(json_type);
	jsonValue(const jsonValue& val);
	jsonValue(std::string val);
	jsonValue(double val);
	jsonValue(jsonObject val);
	jsonValue(jsonArray arr);
	jsonValue& insert(std::string name, jsonValue& val);
	jsonValue& pushBack(jsonValue& val);
	jsonValue& find(std::string path);
	jsonValue& at(std::string name);
	jsonValue& at(size_t pos);
	void parent(jsonValue& jv);
	jsonValue& parent();
	json_type getType();
	std::string& tostring();
	double tonumber();
	virtual ~jsonValue();
	void freeStorge();
private:
	json_type type;
	int len;
	union storge
	{
		double number;
		std::string* string;
		jsonArray* array;
		jsonObject* object;
	};
	storge value;
	static jsonValue nullvalue;
	static std::string nullstring;
	jsonValue *parent_ptr;
};



#endif