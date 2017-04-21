#include "jsonvalue.h"

std::string jsonValue::nullstring;
jsonValue jsonValue::nullvalue(type_null);

jsonValue & jsonValue::operator=(const jsonValue & val)
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

jsonValue::jsonValue() {
	type = type_undefined;
	value.number = NULL;
	parent_ptr = 0;
}
jsonValue::jsonValue(json_type jt) {
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
jsonValue::jsonValue(const jsonValue& val){
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
jsonValue::jsonValue(std::string val){
	type = type_string;
	value.string = new std::string;
	(*value.string) = val;
	parent_ptr = 0;
}
jsonValue::jsonValue(double val) {
	type = type_number;
	value.number = val;
	parent_ptr = 0;
}
jsonValue::jsonValue(jsonObject val) {
	type = type_object;
	value.object = new jsonObject;
	(*value.object) = val;
	parent_ptr = 0;
}
jsonValue::jsonValue(jsonArray arr) {
	type = type_array;
	value.array = new jsonArray;
	(*value.array) = arr;
	parent_ptr = 0;
}
jsonValue& jsonValue::insert(std::string name, jsonValue& val) {
	if (type == type_object)
	{
		//(*value.object).insert(std::pair<std::string,jsonValue&>(name,val));
		return (*value.object)[name]=val;
	}
	else {
		return nullvalue;
	}
}
jsonValue& jsonValue::pushBack(jsonValue& val) {
	if (type == type_array)
	{
		(*value.array).push_back(val);
		return value.array->back();
	}
	else {
		return nullvalue;
	}
}
jsonValue& jsonValue::find(std::string path) {
	if (type == type_object)
	{
		size_t pos = 0, start = 0;
		std::string name;
		jsonObject::iterator iter,end;
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
jsonValue& jsonValue::at(std::string name) {
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
jsonValue& jsonValue::at(size_t pos) {
	if (type == type_array)
	{
		return value.array->at(pos);
	}
	else {
		return nullvalue;
	}
}
std::string& jsonValue::tostring() {
	if (type == type_string)
	{
		
		return *value.string;
	}
	else {
		
		return nullstring;
	}
}
double jsonValue::tonumber() {
	if (type == type_number)
	{
		
		return value.number;
	}
	else{
		
		return 0;
	}
}
jsonValue::~jsonValue() {
	freeStorge();
}
void jsonValue::freeStorge()
{
	switch (type) {
	case type_array:
		delete value.array;
		value.array = 0;
		type = type_undefined;
		break;
	case type_object:
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
json_type jsonValue::getType() {
	return this->type;
}

void jsonValue::parent(jsonValue& jv) {
	parent_ptr = &jv;
}
jsonValue& jsonValue::parent() { 
	return *parent_ptr;

}