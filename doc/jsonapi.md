# C语言版本API说明

## jsonCreate

创建json对象，后续解析的内容存放再这个对象钟。  
参数：无  
例子  

    void* json = jsonCreate();

## jsonParse

解析字符串或者指定路径的文件  
参数1{void\*}：json对象  
参数2{char\*}:字符串或者路径  
参数3{int}:0指定以字符串解析，1指定以文件解析  
例子  

    void* json = jsonCreate();
    jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);

## jsonFree

释放json对象  
参数：无  
示例  

    void json=jsonCreate();
    jsonFree(json);

## jsonGetNumber

获取指定名字的数值  
参数1{void\*}:json对象，解析以后的json对象  
参数1{const char\*}：名字，支持以点分隔，查找指定对象的值  
返回{double}:返回指定名字的数值  
示例  

    void json=jsonCreate();
    jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);
    double num=jsonGetString(json, "soft.cost");
    jsonFree(json);

## jsonGetString

获取指定名字的字符串  
参数1{void\*}:json对象，解析以后的json对象  
参数1{const char\*}：名字，支持以点分隔，查找指定对象的值  
返回{const char\*}:返回指定名字的字符串  
示例  

    void json=jsonCreate();
    jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);
    const char* str=jsonGetString(json, "soft.os");
    jsonFree(json);

## jsonGetArray

目前暂时不支持获取数组中的数据