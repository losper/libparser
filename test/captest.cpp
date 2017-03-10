#include <xmlparser.hpp>
#include <aeolus.hpp>

TEST(xmlparser,xml) {
	char* buf = "<person><name>liuwenjun</name><sex>male</sex></person>";
	xmlparser xmlp;
	xmlp.parse(buf);
	CHECK(!strcmp(xmlp.get("persion.name").c_str(),"liuwenjun"));
	xmlp.get("person").getTagName();
	xmlp.get("person.name").getText();
}

int main() {
	TestResult tr;
	TestKit::runAllTests(tr);
	TestKit::runHtml();
	return 0;
}