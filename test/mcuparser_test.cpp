#include "../src/aeolus.hpp"
#include "logic.h"
#include "streamtest.hpp"
void testparser(char* buf,int len,int &result,logic& lgc) {
	BYTE *data;
	lgc.get((BYTE*)buf, len);
	while (!lgc.end())
	{
		if (lgc.proc(data, len))
		{
			if (data != NULL)
			{
				if (!verify_command_ex(data, len)) {
					stream::write(data, len);
					result++;
				}
			}
		}
	}
}
TEST(memstr, group) {
	char* test1 = "\x7a\xa7\x00\x00\x7a\x7a\x7a\xa7\x98\x09";
	CHECK((test1 + 0) == memstr(test1, 10, "\x7a\xa7", 2));
	CHECK((test1 + 6) == memstr(test1+1, 9, "\x7a\xa7", 2));
	CHECK((test1 + 6) != memstr(test1, 10, "\x7a\xa7", 2));
	char* test2 = "\x7a\xa7\x00\x00\x7a\x7a\x7a\xa7\x98\x09";
	CHECK(0 == memstr(test2 + 1, 6, "\x7a\xa7", 2));
}
TEST(logic, group) {
	///测试当收到数据长度位异常大，是否会影响其他正常的数据
	char* buf = "\x7a\xa7\x00\x00\x7a\x7a\x7a\xa7\x98\x09";
	logic lgc;
	int len = 10;
	int result = 0;

	testparser(buf, 10, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x06\x6b\x98\x09";
	testparser(buf, 10, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);
	
	CHECK(result == 3);
}
TEST(logic2, group) {
	///测试一串正确的数据后面带一串不正确的数据是否可以正常识别
	char* buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6b\x98\x09";
	logic lgc;
	int len = 10;
	int result = 0;

	testparser(buf, 10, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6b\x98\x09";
	testparser(buf, 10, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);

	CHECK(result == 2);
}
TEST(logic3, group) {
	///测试每次处理的数据不完整但是正确的数据时,并且参杂噪音数据是否可以正常识别
	char* buf = "\x7a\xa7\x01\x40\x01\x02";
	logic lgc;
	int len = 10;
	int result = 0;

	testparser(buf, 6, result, lgc);
	buf = "\x05\x6a\x98\x09\x7a";
	testparser(buf, 5, result, lgc);
	buf = "\xa7\x01\x40\x01\x02\x05\x6a\x98\x09\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 19, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);

	CHECK(result == 4);
}

TEST(logic4, group) {
	///测试每次处理的数据不完整但是正确的数据时,并且参杂噪音数据是否可以正常识别
	char* buf = "123456789\x7a";
	logic lgc;
	int len = 10;
	int result = 0;
	

	testparser(buf, 10, result, lgc);
	buf = "\xa7\x01\x40\x01\x02\x05\x6a\x98\x09\x7a";
	testparser(buf, 10, result, lgc);
	buf = "\xa7\x01\x40\x01\x02\x05\x6a\x98\x09\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 19, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);

	CHECK(result == 4);
}
TEST(logic5, group) {
	///测试每次处理的数据不完整但是正确的数据时,并且参杂噪音数据是否可以正常识别
	char* buf = "123456789\x7a";
	logic lgc;
	int len = 10;
	int result = 0;

	testparser(buf, 10, result, lgc);
	buf = "\xa7\x01\x40\x01\x02\x07a\xa7\x98\x09\x7a";
	testparser(buf, 10, result, lgc);
	buf = "\xa7\x01\x40\x01\x02\x05\x6a\x98\x09\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 19, result, lgc);
	buf = "\x7a\xa7\x01\x40\x01\x02\x05\x6a\x98\x09";
	testparser(buf, 10, result, lgc);

	CHECK(result == 3);
}
int main() {
	TestResult tr;

	/*TestKit::setCmd("\"C:\\Program Files (x86)\\Internet Explorer\\iexplore.exe\"");
	TestKit::setPath("D:\\dev\\msys2\\home\\hsae\\git\\Aeolus\\src\\");*/
	
	TestKit::runAllTests(tr);
	TestKit::runHtml();
	return 0;
}