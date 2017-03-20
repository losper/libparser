#ifndef __DPZ__JSONEXPORT__HPP__ 
#define __DPZ__JSONEXPORT__HPP__


int jsonParse(void* json, const char* path, int type);
void* jsonCreate();
void jsonFree(void* json);
const char* jsonGetString(void* json, const char* path);
double jsonGetNumber(void* json, const char* path);
void jsonSetString(void* json, const char* path, const char* value);
void jsonSetNumber(void* json, const char* path, double value);

#endif