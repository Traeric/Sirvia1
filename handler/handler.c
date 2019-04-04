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
        return result;
    } else {
        return NULL;
    }
}
