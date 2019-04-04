/**
 * Author: Eric JIn
 * Date: 2019-04-03
 */
#include "cJSON.h"
#include "../utils/regx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *replaceVariableFunc(char *, cJSON *);

char *replaceForFunc(char *, cJSON *);


char *handlerTemplate(char *content, cJSON *args) {
    // 替换for循环
    char *replaceFor = "\\{\\$ for \\w+ in \\w+ \\$\\}.*\\{\\$ endfor \\$\\}";
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
    char *forVariables = matchContain("\\{\\$ for \\w+ in (\\w+) \\$\\}", replaceString);
    char *key = matchContain("\\{\\$ for (\\w+) in \\w+ \\$\\}", replaceString);
    char *content = matchContain("\\{\\$ for \\w+ in \\w+ \\$\\}(.*)\\{\\$ endfor \\$\\}", replaceString);
    char *result = (char *) malloc(0);
    // 获取循环的值
    cJSON *array = cJSON_GetObjectItem(args, forVariables);
    int len = cJSON_GetArraySize(array);
    // 循环添加值
    for (int i = 0; i < len; i++) {
        char *value = cJSON_GetArrayItem(array, i)->valuestring;
        cJSON *arg = cJSON_CreateObject();
        cJSON_AddItemToObject(arg, key, cJSON_CreateString(value));
        // 替换for循环的内容
        char *pattern = "\\{zw \\w+ wz\\}";
        // 每一次循环替换的结果
        char *resultItem = replaceStr(pattern, content, arg, replaceVariableFunc);
        printf("%s\n", resultItem);
        result = (char *) realloc(result, sizeof(char) * (strlen(result) + strlen(resultItem)));
        sprintf(result, "%s%s", result, resultItem);   // 将每一次循环的结果接到结果后面
    }
    return result;
}
