/**
 * Author: Eric JIn
 * Date: 2019-03-31
 */
#include <stdlib.h>
#include <stdio.h>
#include "../utils/cJSON.h"
#include <string.h>

cJSON *home(cJSON *request) {
    if (strcmp("GET", cJSON_GetObjectItem(request, "method")->valuestring) == 0) {
        cJSON *result = cJSON_CreateObject();
        cJSON_AddItemToObject(result, "target", cJSON_CreateString("index.html"));

        cJSON_AddItemToObject(result, "name", cJSON_CreateString("Sirvia"));

        char *hobby[3] = {"basketball", "volley ball", "base ball"};
        cJSON_AddItemToObject(result, "hobby", cJSON_CreateStringArray((const char **) hobby, 3));


        cJSON *t = cJSON_CreateObject();
        cJSON_AddItemToObject(t, "111", cJSON_CreateString("222"));
        cJSON_AddItemToObject(t, "333", cJSON_CreateString("444"));
        cJSON_AddItemToObject(t, "555", cJSON_CreateString("666"));
        cJSON_AddItemToObject(t, "777", cJSON_CreateString("888"));

        cJSON_AddItemToObject(result, "test", t);
        return result;
    } else {
        return NULL;
    }
}
