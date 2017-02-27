#ifndef __DPZ__AEOLUS__HPP__ 
#define __DPZ__AEOLUS__HPP__

#include <windows.h>

class logic {
public:
	logic();
	void get(BYTE* data, int len);
	int end();
	int proc(BYTE*& data, int& len);
private:
	BYTE buf[1024];
	int offset_;
	int len_;
	int endflag_;
};
int verify_command_ex(BYTE* data, int len);
void* memstr(void* dst, int dstlen, void* cmp, int cmplen);

#endif