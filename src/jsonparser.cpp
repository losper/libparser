#include "jsonparser.h"
#include "jsonvalue.h"
#include <fstream>
#include <string.h>


int jsonParse(void* json,const char* path,int type) {
	if (json)
	{
		jsonValue &obj = *(jsonValue*)json;
		jsonParser jpr(obj);
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
	jsonValue* obj = new jsonValue();
	return obj;
}
void jsonFree(void* json)
{
	delete json;
}
const char* jsonGetString(void* json,const char* path) {
	return ((jsonValue*)json)->find(path).tostring().c_str();
}
double jsonGetNumber(void* json,const char* path) {
	return ((jsonValue*)json)->find(path).tonumber();
}
void jsonSetString(void* json,const char* path,const char* value) {
	
}
void jsonSetNumber(void* json,const char* path,double value) {
	
}

jsonParser::jsonParser(jsonValue & obj):root(obj)
{
	ptr = &root;
	idx = 0;
	state = json_parse_start;
}

void jsonParser::json_parse_start_f() {
	while (buff[idx])
	{
		switch (buff[idx])
		{
		case ' ':
			break;
		case '{':
			state = json_object_start;
			return ;
		case '[':
			state = json_array_start;
			return ;
		default:
			state = json_parse_error;
			return ;
		}
		idx++;
	}
	state = json_parse_end;
}
void jsonParser::json_parse_error_f()
{
	std::cout << "parse error!!!\r\n" << std::endl;
}
void jsonParser::json_name_start_f()
{
	tmpname.clear();
	const char* end = strchr(buff + idx, '"');
	if (end) {
		tmpname.append(buff + idx, end);
		idx = end - buff;
		state = json_name_end;
	}
	else {
		state = json_parse_error;
	}
}
void jsonParser::json_name_end_f()
{
	while (buff[idx])
	{
		switch (buff[idx]) {
		case ' ':
			break;
		case ':':
			state = json_split_start;
			return ;
		default:
			state = json_parse_error;
			return ;
		}
		idx++;
	}
}
void jsonParser::json_split_start_f()
{
	while (buff[idx])
	{
		switch (buff[idx]) {
		case ' ':
			break;
		case '"':
			state = json_string_start;
			return;
		case '{':
			state = json_object_start;
			return ;
		case '[':
			state = json_array_start;
			return ;
		default:
			state = json_number_start;
			idx--;
			return;
		}
		idx++;
	}
}
void jsonParser::json_object_start_f()
{
	state = json_parse_error;
	switch (ptr->getType()) {
		case type_array:
			{
				jsonObject tobj;
				jsonValue tval(tobj);
				jsonValue& arr = ptr->pushBack(tval);
				if (arr.getType() != type_null) {
					arr.parent(*ptr);
					ptr = &arr;
				}
				else {
					return;
				}

			}
			break;
		case type_object:
			{
				jsonObject tobj;
				jsonValue tval(tobj);
				jsonValue& obj = ptr->insert(tmpname, tval);
				if (obj.getType() != type_null) {
					obj.parent(*ptr);
					ptr = &obj;
				}
				else {
					return;
				}
			}
			break;
		case type_undefined: 
			{
				jsonObject tobj;
				jsonValue tval(tobj);
				*ptr = tval;

			}
			break;
		default:
			break;
	}
	while (buff[idx])
	{
		switch (buff[idx])
		{
		case ' ':
			continue;
			break;
		case '"':
			state = json_name_start;
			return;
		default:
			return;
		}
		idx++;
	}
}
void jsonParser::json_object_end_f()
{
}
void jsonParser::json_string_start_f()
{
	
	const char* end=strchr(buff+idx,'"');
	if (end) {
		std::string ctx;
		jsonValue tval(ctx);
		ctx.append(buff + idx, end);
		idx = end - buff;
		state = json_string_end;
		if (ptr->getType() == type_object) {
			ptr->insert(tmpname, tval);
		}
		else {
			ptr->pushBack(tval);
		}
	}
	else {
		state = json_parse_error;
	}
	
}
void jsonParser::json_number_start_f()
{
	double num=0;
	state = json_parse_error;
	char* endptr;
	num=strtod(buff + idx, &endptr);
	switch (*endptr) {
		case '}':
		case ']':
		case ',':
		case ' ':
			{
				state = json_number_end;
				idx = endptr - buff - 1;
				jsonValue tval(num);
				if (ptr->getType() == type_object) {
					ptr->insert(tmpname, tval);
				}
				else {
					ptr->pushBack(tval);
				}

			}
			return;
		default:
			return;
	}
}
void jsonParser::json_array_start_f()
{
	state = json_parse_error;
	switch (ptr->getType()) {
	case type_array:
	{
		jsonArray tarr;
		jsonValue tval(tarr);
		jsonValue& arr = ptr->pushBack(tval);
		if (arr.getType() != type_null) {
			arr.parent(*ptr);
			ptr = &arr;
		}
		else {
			return;
		}

	}
	break;
	case type_object:
	{
		jsonArray tarr;
		jsonValue tval(tarr);
		jsonValue& obj = ptr->insert(tmpname, tval);
		if (obj.getType() != type_null) {
			obj.parent(*ptr);
			ptr = &obj;
		}
		else {
			return;
		}
	}
	break;
	case type_undefined:
		{
			jsonArray tarr;
			jsonValue tval(tarr);
			*ptr = tval;

		}
		break;
	default:
		break;
	}
	state=json_split_start;
	idx--;
}
void jsonParser::json_value_end_f()
{
	state = json_parse_error;
	switch(ptr->getType())
	{
	case type_object:
		while (buff[idx])
		{
			switch (buff[idx])
			{
			case ' ':
				continue;
				break;
			case ',':
				state = json_split_kv;
				return;
			case '}':
				ptr = &ptr->parent();
				if (ptr) {
					state = json_object_end;
				}
				else {
					state = json_parse_end;
				}
				return ;
			default:
				return;
			}
			idx++;
		}
		break;
	case type_array:
		while (buff[idx])
		{
			switch (buff[idx])
			{
			case ' ':
				continue;
				break;
			case ',':
				state = json_split_start;
				return;
			case ']':
				ptr = &ptr->parent();
				if (ptr) {
					state = json_array_end;
				}
				else {
					state = json_parse_end;
				}
				return ;
			default:
				return;
			}
			idx++;
		}
		break;
	default:
		return;
	}
	
}
void jsonParser::json_split_kv_f()
{
	state = json_parse_error;
	while (buff[idx])
	{
		switch (buff[idx])
		{
		case ' ':
			continue;
			break;
		case '"':
			state = json_name_start;
			return;
		default:
			return;
		}
		idx++;
	}
}
int jsonParser::parseBuff(const char* buf)
{
	buff = buf;
	while (buff[idx])
	{
		switch (state) {
		case json_parse_start:
			json_parse_start_f();
			break;
		case json_parse_end:
			state=json_parse_error;
			continue ;
		case json_parse_error:
			json_parse_error_f();
			return -1;
			break;
		case json_name_start:
			json_name_start_f();
			break;
		case json_name_end:
			json_name_end_f();
			break;
		case json_split_start:
			json_split_start_f();
			break;
		case json_split_end:
			break;
		case json_string_start:
			json_string_start_f();
			break;
		case json_string_end:
			json_value_end_f();
			break;
		case json_number_start:
			json_number_start_f();
			break;
		case json_number_end:
			json_value_end_f();
			break;
		case json_array_start:
			json_array_start_f();
			break;
		case json_array_end:
			json_value_end_f();
			break;
		case json_object_start:
			json_object_start_f();
			break;
		case json_object_end:
			json_value_end_f();
			break;
		case json_split_kv:
			json_split_kv_f();
			break;
		default:
			break;
		}
		idx++;
	}
	return 0;
}
int jsonParser::parseFile(const char* file)
{
	FILE *fs = fopen(file, "rb");
	char buf[1024];
	long len=0;
	if (fs)
	{
		fseek(fs, 0, SEEK_END);
		len = ftell(fs);
		fseek(fs, 0, SEEK_SET);
		while (len) {			
			if (len < 1024) {
				fread(buf, 1, len, fs);
				buf[len] = 0;
				len = 0;
			}
			else {
				fread(buf, 1, 1024, fs);
				buf[1024] = 0;
				len = len - 1024;
			}
			parseBuff(buf);
		}
	}
	return 0;
}