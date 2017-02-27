#ifndef __DPZ__STREAMTEST__HPP__ 
#define __DPZ__STREAMTEST__HPP__

#include <stdlib.h>
template<typename T>
class streamtest {
public:
	static size_t write(void* buf,int len) {
		return instance().writebuf(buf,len);
	}
private:
	static streamtest& instance() {
		static streamtest st;
		return st;
	}
	streamtest() {
		fp = fopen("data.txt", "wb+");
	}
	size_t writebuf(void* buf,int len) {
		return fwrite(buf, 1, len, fp);
	}
private:
	FILE* fp;
};
typedef streamtest<void> stream;

#endif