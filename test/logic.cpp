#include "logic.h"
logic::logic() :offset_(0), len_(0), endflag_(0) {
}
void* memstr(void* dst, int dstlen, void* cmp, int cmplen) {
	BYTE* result = (BYTE*)dst;
	int len = dstlen;
	if (dst && cmp)
	{
		do {
			result = (BYTE*)memchr(result, *((BYTE*)cmp), len);
			if (result && !memcmp(result, cmp, cmplen)) {
				return result;
			}
			if (result)
			{
				result++;
				len = dstlen - (result - dst);
				if (len >= cmplen) {
					continue;
				}
				break;
			}
			else {
				result = 0;
			}
		} while (result);
	}
	return NULL;
}
void logic::get(BYTE* data, int len) {
	endflag_ = 0;
	memcpy(buf + len_, data, len);
	len_ += len;
}
int logic::end() {
	return endflag_;
}
int logic::proc(BYTE*& data, int& len) {
	//int idx=0;
	data = (BYTE*)memstr(buf + offset_, len_ - offset_, "\x7a\xa7", 2);	///查找关键字
	if (data) {
		offset_ = data - buf;
		if ((len_ - offset_)>6)///一包至少有6个字节
		{
			len = data[5] + 6;///包长
			if ((len_ - offset_) >= len) {
				offset_ = offset_ + len;
				return 1;
			}
			else {
				BYTE* tmp = (BYTE*)memstr(buf + offset_ + 1, len_ - offset_ - 1, "\x7a\xa7", 2);	///查找关键字
				if (tmp) {
					data = tmp;
					offset_ = data - buf;
				}
			}
		}
		len_ = len_ - offset_;
		memcpy(buf, data, len_);
		offset_ = 0;
	}
	else {
		if (/*len_>len &&*/ '\x7a' == buf[len_ - 1])
		{
			offset_ = 0;
			len_ = 1;
			buf[0] = '\x7a';
		}
		else {
			offset_ = 0;
			len_ = 0;
		}
		endflag_ = 1;
		return 0;
	}
	endflag_ = 1;
	return 0;
}

int verify_command_ex(BYTE* data, int len) {
	BYTE tmpSum = 0;
	BYTE chkSum = data[len - 1];
	for (int i = 0; i<len - 1; i++) {
		tmpSum += data[i];
	}
	//tmpSum=~tmpSum;
	if (tmpSum == chkSum) {
		//校验成功
		return 0;
	}
	else {
		return -1;
	}
}