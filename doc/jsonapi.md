# C���԰汾API˵��

## jsonCreate

����json���󣬺������������ݴ������������ӡ�  
��������  
����  

    void* json = jsonCreate();

## jsonParse

�����ַ�������ָ��·�����ļ�  
����1{void\*}��json����  
����2{char\*}:�ַ�������·��  
����3{int}:0ָ�����ַ���������1ָ�����ļ�����  
����  

    void* json = jsonCreate();
    jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);

## jsonFree

�ͷ�json����  
��������  
ʾ��  

    void json=jsonCreate();
    jsonFree(json);

## jsonGetNumber

��ȡָ�����ֵ���ֵ  
����1{void\*}:json���󣬽����Ժ��json����  
����1{const char\*}�����֣�֧���Ե�ָ�������ָ�������ֵ  
����{double}:����ָ�����ֵ���ֵ  
ʾ��  

    void json=jsonCreate();
    jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);
    double num=jsonGetString(json, "soft.cost");
    jsonFree(json);

## jsonGetString

��ȡָ�����ֵ��ַ���  
����1{void\*}:json���󣬽����Ժ��json����  
����1{const char\*}�����֣�֧���Ե�ָ�������ָ�������ֵ  
����{const char\*}:����ָ�����ֵ��ַ���  
ʾ��  

    void json=jsonCreate();
    jsonParse(json, "{\"soft\":{\"fee\":[1,2,\"shenme\",{\"cost\":54}],\"app\":\"mouy\",\"os\":\"moux\",\"age\":29,\"bb\":234.369}}", 0);
    const char* str=jsonGetString(json, "soft.os");
    jsonFree(json);

## jsonGetArray

Ŀǰ��ʱ��֧�ֻ�ȡ�����е�����