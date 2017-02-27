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
	///���Ե��յ����ݳ���λ�쳣���Ƿ��Ӱ����������������
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
	///����һ����ȷ�����ݺ����һ������ȷ�������Ƿ��������ʶ��
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
	///����ÿ�δ�������ݲ�����������ȷ������ʱ,���Ҳ������������Ƿ��������ʶ��
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
	///����ÿ�δ�������ݲ�����������ȷ������ʱ,���Ҳ������������Ƿ��������ʶ��
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
	///����ÿ�δ�������ݲ�����������ȷ������ʱ,���Ҳ������������Ƿ��������ʶ��
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