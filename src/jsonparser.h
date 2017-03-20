#ifndef __DPZ__JSONPARSER__HPP__ 
#define __DPZ__JSONPARSER__HPP__

#include "jsonvalue.h"
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
class jsonParser {
public:
	jsonParser(jsonValue& obj);
	int parseFile(const char* file);
	int parseBuff(const char* buf);
private:
	void json_parse_start_f();
	void json_parse_error_f();
	void json_name_start_f();
	void json_name_end_f();
	void json_split_start_f();
	void json_object_start_f();
	void json_object_end_f();
	void json_string_start_f();
	void json_number_start_f();
	void json_array_start_f();
	void json_value_end_f();
	void json_split_kv_f();
	

private:
	jsonStatus state;
	jsonValue& root;
	jsonValue* ptr;
	int idx;
	int len;
	const char* buff;
	std::string tmpname;
};

#endif