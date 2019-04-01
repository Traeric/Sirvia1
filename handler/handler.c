/**
 * Author: Eric JIn
 * Date: 2019-03-31
 */
#include <stdlib.h>
#include <stdio.h>
#include "../utils/cJSON.h"


char *home(cJSON *request) {
    char *test = "hello world!";
    printf("%s\n", test);
    return "index.html";
}
