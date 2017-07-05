#ifndef __DPZ__JSONEXPORT__HPP__ 
#define __DPZ__JSONEXPORT__HPP__

//#ifdef __cplusplus
//extern "C" {
//#endif
int jsonSave(void* json, const char* path);
int jsonParse(void* json, const char* path, int type);
void* jsonCreate();
void jsonFree(void* json);
const char* jsonGetString(void* json, const char* path);
double jsonGetNumber(void* json, const char* path);
const char* jsonToString(void* json);
double jsonToNumber(void* json);
int jsonSetString(void* json, const char* path, const char* value);
int jsonSetNumber(void* json, const char* path, double value);
int jsonAddArray(void* json, const char* path);
void* jsonGetValue(void* json, const char* path);
int jarrPushString(void* json, const char* value);
int jarrPushNumber(void* json, double value);
void* jarrGetValue(void* json,int idx);

//#ifdef __cplusplus
//}
//#endif
#endif