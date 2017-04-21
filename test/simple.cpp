#include "aeolus/aeolus.hpp"
#include "../src/jsonexport.h"
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
	jsonParse(json, "../test/json/prop.json" ,1);
	CHECK(2345 == jsonGetNumber(json, "port"));
	CHECK(!strcmp("127.0.0.1",jsonGetString(json, "ip")));
	jsonFree(json);
}
int main() {
	TestResult tr;
	TestKit::runAllTests(tr);
	TestKit::runHtml();
	return 0;
}