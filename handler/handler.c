/**
 * Author: Eric JIn
 * Date: 2019-03-31
 */
#include <stdlib.h>
#include <stdio.h>
#include "../utils/cJSON.h"


cJSON *home(cJSON *request) {
    char *test = "hello world!";
    printf("%s\n", test);
    cJSON *result = cJSON_CreateObject();
    cJSON_AddItemToObject(result, "target", cJSON_CreateString("index.html"));
    return result;
}
