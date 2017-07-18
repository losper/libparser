#include "aeolus/aeolus.hpp"
#include "../src/jsonValue.hpp"
#include "../src/jsonParser.hpp"
#include "string.h"
TEST(x,y){
	jsonValue<> root;
	jsonParser<> parser(root);
	parser.parseBuff("{\"imx\":{\"class\":\"haa\",\"sum\":\"123\",\"age\":1234,\"bb\":234.369}}");
	CHECK(!strcmp("haa", root.find("imx.class").tostring().c_str()));
}
int main() {
	TestResult tr;
	TestKit::runAllTests(tr);
	TestKit::runHtml();
	return 0;
}