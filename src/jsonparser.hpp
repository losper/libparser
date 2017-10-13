#ifndef __DPZ__JSONPARSER__HPP__ 
#define __DPZ__JSONPARSER__HPP__

#include "jsonValue.hpp"
#include <string.h>

enum jsonStatus {
	json_parse_start,
	json_parse_end,
	json_parse_error,
	json_name_start,
	json_name_end,
	json_split_start,
	json_split_end,
	json_string_start,
	json_string_end,
	json_number_start,
	json_number_end,
	json_array_start,
	json_array_end,
	json_object_start,
	json_object_end,
	json_split_kv,
};
template<typename T=void>
class jsonParser {
public:
	jsonParser(jsonValue<T> & obj) :root(obj)
	{
		ptr = &root;
		idx = 0;
		state = json_parse_start;
	}
	int parseFile(const char* file)
	{
		FILE *fs = fopen(file, "rb");
		char buf[1024];
		long len = 0;
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
					buf[1023] = 0;
					len = len - 1024;
				}
				parseBuff(buf);
			}
			fclose(fs);
		}
		else {
			printf("parseFile error\r\n");
			return -1;
		}
		printf("parseFile ok\r\n");
		return 0;
	}
	int parseBuff(const char* buf)
	{
		buff = buf;
		while (buff[idx])
		{
			switch (state) {
			case json_parse_start:
				json_parse_start_f();
				break;
			case json_parse_end:
				state = json_parse_error;
				continue;
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

private:
	void skip() {
		while (buff[idx])
		{
			switch (buff[idx])
			{
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				idx++;
				continue;
			default:
				return ;
			}
		}
		
	}
	void json_parse_start_f() {
		skip();
		while (buff[idx])
		{
			switch (buff[idx])
			{
			case '{':
				state = json_object_start;
				return;
			case '[':
				state = json_array_start;
				return;
			default:
				state = json_parse_error;
				return;
			}
			idx++;
		}
		state = json_parse_end;
	}
	void json_parse_error_f()
	{
		std::cout << "parse error!!!\r\n" << std::endl;
	}
	void json_name_start_f()
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
	void json_name_end_f()
	{
		skip();
		while (buff[idx])
		{
			switch (buff[idx]) {
			case ':':
				state = json_split_start;
				return;
			default:
				state = json_parse_error;
				return;
			}
			idx++;
		}
	}
	void json_split_start_f()
	{
		skip();
		while (buff[idx])
		{
			switch (buff[idx]) {
			case '"':
				state = json_string_start;
				return;
			case '{':
				state = json_object_start;
				return;
			case '[':
				state = json_array_start;
				return;
			default:
				state = json_number_start;
				idx--;
				return;
			}
			idx++;
		}
	}
	void json_object_start_f()
	{
		state = json_parse_error;
		switch (ptr->getType()) {
		case type_array:
		{
			typename jsonValue<T>::jsonObject tobj;
			jsonValue<T> tval(tobj);
			jsonValue<T>& arr = ptr->pushBack(tval);
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
			typename jsonValue<T>::jsonObject tobj;
			jsonValue<T> tval(tobj);
			jsonValue<T>& obj = ptr->insert(tmpname, tval);
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
			typename jsonValue<T>::jsonObject tobj;
			jsonValue<T> tval(tobj);
			*ptr = tval;

		}
		break;
		default:
			break;
		}
		skip();
		while (buff[idx])
		{
			switch (buff[idx])
			{
			case '"':
				state = json_name_start;
				return;
			default:
				return;
			}
			idx++;
		}
	}
	void json_object_end_f()
	{
	}
	void json_string_start_f()
	{

		const char* end = strchr(buff + idx, '"');
		if (end) {
			std::string ctx;
			ctx.append(buff + idx, end);
			jsonValue<T> tval(ctx);
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
	void json_number_start_f()
	{
		double num = 0;
		state = json_parse_error;
		char* endptr;
		num = strtod(buff + idx, &endptr);
		switch (*endptr) {
		case '}':
		case ']':
		case ',':
		case ' ':
		{
			state = json_number_end;
			idx = endptr - buff - 1;
			jsonValue<T> tval(num);
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
	void json_array_start_f()
	{
		state = json_parse_error;
		switch (ptr->getType()) {
		case type_array:
		{
			typename jsonValue<T>::jsonArray tarr;
			jsonValue<T> tval(tarr);
			jsonValue<T>& arr = ptr->pushBack(tval);
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
			typename jsonValue<T>::jsonArray tarr;
			jsonValue<T> tval(tarr);
			jsonValue<T>& obj = ptr->insert(tmpname, tval);
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
			typename jsonValue<T>::jsonArray tarr;
			jsonValue<T> tval(tarr);
			*ptr = tval;

		}
		break;
		default:
			break;
		}
		state = json_split_start;
		idx--;
	}
	void json_value_end_f()
	{
		state = json_parse_error;
		switch (ptr->getType())
		{
		case type_object:
			skip();
			while (buff[idx])
			{
				switch (buff[idx])
				{
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
					return;
				default:
					return;
				}
				idx++;
			}
			break;
		case type_array:
			skip();
			while (buff[idx])
			{
				switch (buff[idx])
				{
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
					return;
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
	void json_split_kv_f()
	{
		state = json_parse_error;
		skip();
		while (buff[idx])
		{
			switch (buff[idx])
			{
			case '"':
				state = json_name_start;
				return;
			default:
				return;
			}
			idx++;
		}
	}
	
	

private:
	jsonStatus state;
	jsonValue<T>& root;
	jsonValue<T>* ptr;
	int idx;
	int len;
	const char* buff;
	std::string tmpname;
};

#endif