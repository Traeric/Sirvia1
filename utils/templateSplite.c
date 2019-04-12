/**
 * Author: Eric JIn
 * Date: 2019-04-03
 */
#include "cJSON.h"
#include "../utils/regx.h"
#include "stringUtil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *replaceVariableFunc(char *, cJSON *);

char *replaceForFunc(char *, cJSON *);


char *handlerTemplate(char *content, cJSON *args) {
    // 替换for循环
    char *replaceFor = "\\{\\$ for .+ in .+ \\$\\}.*\\{\\$ endfor \\$\\}";
    content = replaceStr(replaceFor, content, args, replaceForFunc);
    // 替换所有的变量
    char *replaceVariable = "\\{zw \\w+ wz\\}";
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

/**
 * 替换for循环
 * @param replaceStr
 * @param args
 * @return
 */
char *replaceForFunc(char *replaceString, cJSON *args) {
    // 获取三个点： count counts 以及循环里面的变量
    char *forVariables = matchContain("\\{\\$ for .+ in (.+) \\$\\}\n", replaceString);
    char *item = matchContain("\\{\\$ for (.+) in .+ \\$\\}", replaceString);
    char *content = matchContain("\\{\\$ for .+ in .+ \\$\\}(.*)\\{\\$ endfor \\$\\}", replaceString);
    char *result = (char *) malloc(0);

    if (strchr(item, ',')) {
        printf("%s\n", "有逗号");
        // 循环对象
        // 获取key跟value
        char *key = strtok(item, ",");
        char *value = strtok(NULL, ",");
        key = trim(key);
        value = trim(value);
        // 获取handler中对应的值
        cJSON *array_obj = cJSON_GetObjectItem(args, forVariables);
        int len = cJSON_GetArraySize(array_obj);
        // 循环添加值
        for (int i = 0; i < len; i++) {
            cJSON *jsonItem = cJSON_GetArrayItem(array_obj, i);
            char *jsonItemKey = jsonItem->string;   // 获取键
            char *jsonItemValue = jsonItem->valuestring;   // 获取值
            cJSON *arg = cJSON_CreateObject();
            cJSON_AddItemToObject(arg, key, cJSON_CreateString(jsonItemKey));
            cJSON_AddItemToObject(arg, value, cJSON_CreateString(jsonItemValue));
            // 替换for循环的内容
            char *pattern = "\\{zw \\w+ wz\\}";
            // 每一次循环替换的结果
            char *resultItem = replaceStr(pattern, content, arg, replaceVariableFunc);
            result = (char *) realloc(result, sizeof(char) * (strlen(result) + strlen(resultItem)));
            memcpy(result + strlen(result), resultItem, strlen(resultItem));
        }
    } else {
        printf("%s\n", "没有逗号");
        // 循环数组
        // 获取循环的值
        cJSON *array = cJSON_GetObjectItem(args, forVariables);
        int len = cJSON_GetArraySize(array);
        // 循环添加值
        for (int i = 0; i < len; i++) {
            char *value = cJSON_GetArrayItem(array, i)->valuestring;
            cJSON *arg = cJSON_CreateObject();
            cJSON_AddItemToObject(arg, item, cJSON_CreateString(value));
            // 替换for循环的内容
            char *pattern = "\\{zw \\w+ wz\\}";
            // 每一次循环替换的结果
            char *resultItem = replaceStr(pattern, content, arg, replaceVariableFunc);
            result = (char *) realloc(result, sizeof(char) * (strlen(result) + strlen(resultItem)));
            memcpy(result + strlen(result), resultItem, strlen(resultItem));
        }
    }
    return result;
}
