#include "aeolus/aeolus.hpp"
#include "../src/jsonexport.h"

TEST(a,b) {
	void* json = jsonCreate();
	jsonParse(json, "{\"imx\":{\"class\":\"haa\",\"sum\":\"123\",\"age\":1234,\"bb\":234.369}}", 0);
	CHECK(!strcmp("haa",jsonGetString(json, "imx.class")));
	CHECK(!strcmp("123", jsonGetString(json, "imx.sum")));
	CHECK(1234==jsonGetNumber(json, "imx.age"));
	CHECK(234.369==jsonGetNumber(json, "imx.bb"));
	jsonFree(json);
}

int main() {
	
	TestResult tr;
	TestKit::runAllTests(tr);
	TestKit::runHtml();
	return 0;
}