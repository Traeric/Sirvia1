/**
 * Author: Eric JIn
 * Date: 2019-04-03
 */
#include "cJSON.h"
#include "../utils/regx.h"
#include <string.h>

char *replaceVariableFunc(char *, cJSON *);

char *handlerTemplate(char *content, cJSON *args) {
    // 替换所有的变量
    char *replaceVariable = "zw \\w+ wz";
    return replaceStr(replaceVariable, content, args, replaceVariableFunc);
}

/**
 * 处理替换变量
 * @param replaceStr
 * @param args
 * @return
 */
char *replaceVariableFunc(char *replaceStr, cJSON *args) {
    char *variable = strtok(replaceStr, "{zw ");  // 获取到变量的名字
    // 替换掉变量
    return cJSON_GetObjectItem(args, variable)->valuestring;
}

