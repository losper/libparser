#ifndef __DPZ__JSONVALUE__HPP__ 
#define __DPZ__JSONVALUE__HPP__

#include <map>
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "jsondef.h"
template<typename T=void>
class jsonValue {
public:
	typedef std::map<std::string, jsonValue> jsonObject;
	typedef std::vector<jsonValue> jsonArray;
public:
	jsonValue & operator=(const jsonValue & val)
	{
		// TODO: 在此处插入 return 语句
		freeStorge();
		switch (val.type)
		{
		case type_string:
			type = type_string;
			value.string = new std::string;
			(*value.string) = *(val.value.string);
			break;
		case type_object:
			type = type_object;
			value.object = new jsonObject;
			(*value.object) = *(val.value.object);
			break;
		case type_array:
			type = type_array;
			value.array = new jsonArray;
			(*value.array) = *(val.value.array);
			break;
		case type_number:
			type = type_number;
			value.number = val.value.number;
			break;
		default:
			break;
		}
		parent_ptr = 0;
		return *this;
	}

	jsonValue() {
		type = type_undefined;
		value.number = 0;
		parent_ptr = 0;
	}
	jsonValue(json_type jt) {
		type = jt;
		parent_ptr = 0;
		switch (type)
		{
		case type_string:
			type = type_string;
			value.string = new std::string;
			break;
		case type_object:
			type = type_object;
			value.object = new jsonObject;
			break;
		case type_array:
			type = type_array;
			value.array = new jsonArray;
			break;
		case type_number:
			type = type_number;
			break;
		default:
			break;
		}
	}
	jsonValue(const jsonValue& val) {
		switch (val.type)
		{
		case type_string:
			type = type_string;
			value.string = new std::string;
			(*value.string) = *(val.value.string);
			break;
		case type_object:
			type = type_object;
			value.object = new jsonObject;
			(*value.object) = *(val.value.object);
			break;
		case type_array:
			type = type_array;
			value.array = new jsonArray;
			(*value.array) = *(val.value.array);
			break;
		case type_number:
			type = type_number;
			value.number = val.value.number;
			break;
		default:
			break;
		}
		parent_ptr = 0;
	}
	jsonValue(std::string val) {
		type = type_string;
		value.string = new std::string;
		(*value.string) = val;
		parent_ptr = 0;
	}
	jsonValue(double val) {
		type = type_number;
		value.number = val;
		parent_ptr = 0;
	}
	jsonValue(jsonObject val) {
		type = type_object;
		value.object = new jsonObject;
		(*value.object) = val;
		parent_ptr = 0;
	}
	jsonValue(jsonArray arr) {
		type = type_array;
		value.array = new jsonArray;
		(*value.array) = arr;
		parent_ptr = 0;
	}
	jsonValue& insert(std::string name, jsonValue& val) {
		if (type == type_object)
		{
			//(*value.object).insert(std::pair<std::string,jsonValue&>(name,val));
			return (*value.object)[name] = val;
		}
		else {
			return nullvalue;
		}
	}
	jsonValue& pushBack(jsonValue& val) {
		if (type == type_array)
		{
			(*value.array).push_back(val);
			return value.array->back();
		}
		else {
			return nullvalue;
		}
	}
	jsonValue& find(std::string path) {
		if (type == type_object)
		{
			size_t pos = 0, start = 0;
			std::string name;
			jsonObject::iterator iter, end;
			while (pos != -1)
			{
				pos = path.find('.', start);
				name = path.substr(start, pos);
				if (start) {
					end = iter->second.value.object->end();
					iter = iter->second.value.object->find(name);
				}
				else {
					end = value.object->end();
					iter = value.object->find(name);
				}
				start = pos + 1;
			}
			if (iter != end) {
				return iter->second;
			}
			return nullvalue;
		}
		else {
			return nullvalue;
		}
	}
	jsonValue& at(std::string name) {
		if (type == type_object)
		{
			jsonObject::iterator iter = value.object->find(name);
			if (iter != value.object->end()) {
				return iter->second;
			}
			return nullvalue;
		}
		else {
			return nullvalue;
		}
	}
	jsonValue& at(size_t pos) {
		if (type == type_array)
		{
			return value.array->at(pos);
		}
		else {
			return nullvalue;
		}
	}
	std::string& tostring() {
		if (type == type_string)
		{

			return *value.string;
		}
		else {

			return nullstring;
		}
	}
	double tonumber() {
		if (type == type_number)
		{

			return value.number;
		}
		else {

			return 0;
		}
	}
	~jsonValue() {
		freeStorge();
	}
	void freeStorge()
	{
		switch (type) {
		case type_array:
			for (jsonArray::iterator iter = value.array->begin(); iter != value.array->end(); iter++) {
				iter->freeStorge();
			}
			delete value.array;
			value.array = 0;
			type = type_undefined;
			break;
		case type_object:
			for (jsonObject::iterator iter = value.object->begin(); iter != value.object->end(); iter++) {
				iter->second.freeStorge();
			}
			delete value.object;
			value.object = 0;
			type = type_undefined;
			break;
		case type_string:
			if (value.string)
			{
				delete value.string;
				value.string = 0;
				type = type_undefined;
			}
			break;
		default:
			break;
		}
	}
	std::string serialize()
	{
		std::string ctx = "";
		int flag = 0;
		switch (getType())
		{
		case type_string:
			ctx += "\"";
			ctx.append(*value.string);
			ctx += "\"";
			break;
		case type_number:
		{
			std::ostringstream ost;
			if (ost << value.number)
				ctx.append(ost.str());
		}
		break;
		case type_array:
			ctx.append("[");
			for (jsonArray::iterator iter = value.array->begin(); iter != value.array->end(); iter++)
			{
				if (flag) { ctx += ","; }
				else { flag = 1; }
				ctx.append(iter->serialize());
			}
			ctx.append("]");
			break;
		case type_object:
			ctx.append("{");
			for (jsonObject::iterator iter = value.object->begin(); iter != value.object->end(); iter++)
			{
				if (flag) { ctx += ","; }
				else { flag = 1; }
				ctx += "\"";
				ctx.append(iter->first);
				ctx += "\":";
				ctx.append(iter->second.serialize());
			}
			ctx.append("}");
			break;
		default:
			break;
		}
		return ctx;
	}
	json_type getType() {
		return this->type;
	}

	void parent(jsonValue& jv) {
		parent_ptr = &jv;
	}
	jsonValue& parent() {
		return *parent_ptr;

	}
	jsonObject& toObject() {
		return *value.object;
	}
	jsonArray& toArray() {
		return *value.array;
	}
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

template<typename T> std::string jsonValue<T>::nullstring;
template<typename T> jsonValue<T> jsonValue<T>::nullvalue(type_null);
#endif