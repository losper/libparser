#include "windows.h"
#include <map>
#include <list>
#include <string>
enum json_type {
	enum_string,
	enum_number,
	enum_array,
	enum_object,
	enum_null,
};
class json_object {
private:
	json_type type;
	std::map<std::string, json_object> sjo;
};

json_object json_parser(const char* buf) {

}
int main() {
	char* buf = "{'imx':{'name':'liuwenjun','sex':'male','class':'3-2','age':20},'alist':[1,2,3,4,5],'blist':['123','e45','543']}";
	json_t obj=json_parser(buf);
	char* name=json_getstring(obj,"imx.name");
	int value = json_getint(obj,"imx.age");
	json_getlist(obj,"alist");

	return 0;
}