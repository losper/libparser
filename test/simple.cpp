#include "aeolus/aeolus.hpp"
#include "../src/jsonexport.h"
#include "../src/jsonvalue.hpp"
#include "../src/jsonparser.hpp"
//#include "../../libplatform/src/platform.h"
#include "string.h"


TEST(jsoncheck1,json) {
	void* json = jsonCreate();
	jsonParse(json, "{\"imx\":{\"class\":\"haa\",\"sum\":\"123\",\"age\":1234,\"bb\":234.369}}", 0);
	CHECK(!strcmp("haa",jsonGetString(json, "imx.class")));
	CHECK(!strcmp("123", jsonGetString(json, "imx.sum")));
	CHECK(1234==jsonGetNumber(json, "imx.age"));
	CHECK(234.369==jsonGetNumber(json, "imx.bb"));
	jsonFree(json);
}

TEST(jsoncheck2, b) {
	void* json = jsonCreate();
	jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);
	CHECK(strcmp("mouy", jsonGetString(json, "soft")));
	CHECK(!strcmp("moux", jsonGetString(json, "soft.os")));
	CHECK(29 == jsonGetNumber(json, "soft.age"));
	CHECK(234.369 == jsonGetNumber(json, "soft.bb"));
	jsonFree(json);
}
TEST(jsoncheck3, b) {
	void* json = jsonCreate();
	jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);
	CHECK(strcmp("mouy", jsonGetString(json, "soft.23")));
	CHECK(strcmp("moux", jsonGetString(json, "soft.we")));
	CHECK(29 != jsonGetNumber(json, "soft.sdf"));
	CHECK(234.369 != jsonGetNumber(json, "346ff"));
	jsonFree(json);
}
TEST(file,json) {
	void* json = jsonCreate();
	void* tmp=0;
	jsonSetString(json, "ip", "127.0.0.1");
	jsonSetNumber(json, "port", 2345);
	jsonAddArray(json, "arr");
	tmp = jsonGetValue(json, "arr");
	jarrPushNumber(tmp, 123);
	jarrPushString(tmp, "456");
	jsonSave(json, "test.json");
	jsonFree(json);
	
	json = jsonCreate();
	jsonParse(json, "test.json", 1);
	
	/*jsonAddObject();
	jsonPushArray();
	
	jsonPushObject();
	
	*/
	/*jsonGetType();
	jsonNext(tmp);*/
	tmp = jsonGetValue(json, "arr");
	CHECK(123 == jsonToNumber(jarrGetValue(tmp, 0)));
	CHECK(!strcmp("456", jsonToString(jarrGetValue(tmp, 1))));
	//jarrGetSize(tmp);
	CHECK(2345 == jsonGetNumber(json, "port"));
	CHECK(!strcmp("127.0.0.1",jsonGetString(json, "ip")));
	jsonFree(json);
}
TEST(temp,json) {
	jsonValue<> jv;
	jsonParser<> jp(jv);
	jp.parseBuff("{\"plugin\":[{\"hmi\":\"trr1\"},{\"hmi\":\"trr2\"},{\"hmi\":\"trr3\"}]}");
	jsonValue<>& plg = jv.find("plugin");
	char buf[20] = "trr";
	int idx = 1;
	if (plg.getType() == type_array) {
		jsonValue<>::jsonArray plgs = plg.toArray();
		for (jsonValue<>::jsonArray::iterator iter = plgs.begin(); iter != plgs.end(); iter++) {
			sprintf(buf, "trr%d", idx++);
			CHECK(!strcmp(buf, iter->find("hmi").tostring().c_str()));
		}
	}
}
TEST(tempfile,json) {
	jsonValue<> jv;
	jsonParser<> jp(jv);
	char path[256];
	strcpy(path,__FILE__);
	*strrchr(path, '\\') = 0;
	strcat(path, "/data/poseidon.json");
	jp.parseFile(path);
	char* name[] = {"hmi","script","cvip","simulator"};
	int idx = 0;
	if (jv.getType() == type_object) {
		jsonValue<>::jsonObject& plgs = jv.toObject();
		CHECK(plgs.size()!=0);
		for (jsonValue<>::jsonObject::iterator iter = plgs.begin(); iter != plgs.end(); iter++) {
			CHECK(!strcmp(name[idx],iter->first.c_str()) );
			CHECK(!strcmp("",iter->second.tostring().c_str()));
		}
	}
}
int main() {
	TestResult tr;
	TestKit::runAllTests(tr);
	TestKit::runHtml();
	return 0;
}