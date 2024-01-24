#include <stdio.h>
#include <json-c/json.h>
#include <json-c/json_object.h>


int main()
{
    struct json_object *jsonObj = json_object_new_object();
    if (jsonObj == -1)
    {
        perror("json_object_new_object error");
        exit(-1);
    }
    
    struct json_object *value = json_object_new_int64(10000);
    if (value == NULL)
    {
        perror("json_object_new_int64 error");
        exit(-1);
    }
    json_object_object_add(jsonObj, "alexa", value);

    struct json_object * nameValue = json_object_new_string("runnoob");
    if (nameValue == NULL)
    {
        perror("json_object_new_string error");
        exit(-1);
    }
    json_object_object_add(jsonObj, "name", nameValue);

    json_object * array = json_object_new_array();
    json_object_array_add(array, json_object_new_string("jingdong"));
    json_object_array_add(array, json_object_new_string("pingduodou"));
    json_object_array_add(array, json_object_new_string("Taobao"));
    json_object_object_add(jsonObj, "shopping", array);

    /* 将json对象转换成字符串*/
    const char * str = json_object_to_json_string(jsonObj);



    /* 将字符串转化成json对象*/
    //struct json_object *ageOjs = json_object_new_string(str);接口用错了
    struct json_object *ageOjs = json_tokener_parse(str);

    struct json_object *keyValue = json_object_object_get(ageOjs, "alexa");

    /* 解析int类型的json对象*/
    int64_t val = json_object_get_int64(keyValue);

    json_object_put(jsonObj);


    return 0;
}