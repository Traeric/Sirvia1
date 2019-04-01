/**
 * Author: Eric JIn
 * Date: 2019-04-02
 */
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 使用正则表达式匹配字符串
 * @param pattern
 * @param buffer
 * @return
 */
char *matchRegxOne(char *pattern, char *buffer) {
    int status = 0;
    const size_t nmatch = 1;
    regmatch_t pmatch[nmatch];
    regex_t reg;
    char *result = NULL;

    // 编译正则表达式
    if (regcomp(&reg, pattern, REG_EXTENDED) < 0) {
        printf("compile failed!\n");
        return NULL;
    }
    // 匹配正则表达式
    status = regexec(&reg, buffer, nmatch, pmatch, 0);

    if (status == 0) {
        const size_t len = (size_t) pmatch[0].rm_eo - pmatch[0].rm_so;
        result = (char *) malloc(sizeof(char) * len);
        memcpy(result, buffer + pmatch[0].rm_so, len);
    } else {
        printf("no match!\n");
        return NULL;
    }
    regfree(&reg);   // 释放正则表达式
    return result;
}

