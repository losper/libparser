#include "jsonparser.h"
#include "jsonvalue.h"

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

int jsonParser::parseFile(const char* file)
{
	
	return 0;
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
				jsonValue& arr = ptr->pushBack(jsonValue(jsonObject()));
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
				jsonValue& obj = ptr->insert(tmpname, jsonValue(jsonObject()));
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
			*ptr = jsonValue(jsonObject());
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
		ctx.append(buff + idx, end);
		idx = end - buff;
		state = json_string_end;
		if (ptr->getType() == type_object) {
			ptr->insert(tmpname, jsonValue(ctx));
		}
		else {
			ptr->pushBack(jsonValue(ctx));
		}
	}
	else {
		state = json_parse_error;
	}
	
}
void jsonParser::json_number_start_f()
{
	/*double before_num=0,after_num=0;
	int negative = 0,dot = 0,count=0;*/
	double num=0;
	state = json_parse_error;
	char* endptr;
	num=strtod(buff + idx, &endptr);
	switch (*endptr) {
		case '}':
		case ']':
		case ',':
		case ' ':
			state = json_number_end;
			idx=endptr-buff-1;
			if (ptr->getType() == type_object) {
				ptr->insert(tmpname, jsonValue(num));
			}
			else {
				ptr->pushBack(jsonValue(num));
			}
			return;
		default:
			return;
	}
	/*while (buff[idx])
	{
		switch (buff[idx])
		{
		case '-':
			if (negative) {
				state = json_parse_error;
				return;
			}
			negative = 1;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (dot) {
				count++;
				after_num = after_num * 10 + (buff[idx] - '0'); 
			}
			else {
				before_num = before_num * 10 + (buff[idx] - '0');
			}
			break;
		case '.':
			if (dot) {
				state = json_parse_error;
				return;
			}
			dot = 1;
			break;
		case '}':
		case ']':
		case ',':
		case ' ':
			state = json_number_end;
			idx--;
			after_num = 10 ^ count;
			before_num = after_num?after_num/(10^count):0;
			before_num = negative ? before_num*-1 : before_num;
			if (ptr->getType() == type_object) {
				ptr->insert(tmpname, jsonValue(before_num));
			}
			else {
				ptr->pushBack(jsonValue(before_num));
			}
			return ;
		default:
			return;
		}
		idx++;
	}*/
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
				break;
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
			return 0;
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
