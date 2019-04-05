/**
 * Author: Eric JIn
 * Date: 2019-03-31
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * 查询字符串的末尾是否是指定的子串，匹配返回1，不匹配返回0
 * @param targetStr: 原始字符串
 * @param matchStr: 待匹配的字符串
 * @return
 */
int endsWith(char *targetStr, char *matchStr) {
    int targetLen = (int) strlen(targetStr);
    int matchLen = (int) strlen(matchStr);
    int j = 0;
    for (int i = targetLen; i > (targetLen - matchLen); i--, j++) {
        if (targetStr[i] != matchStr[matchLen - j]) {
            return 0;
        }
    }
    return 1;
}

/**
 * 计算一个指针指向的字符串的长度
 * @param str
 * @return
 */
int calcLen(const char *str) {
    int len = 0;
    while (*(str + len++) != '\0');
    return --len;   // 最后会多加一下，在这里减掉
}

/**
 * 替换中间的字符串
 * @param start
 * @param end
 * @param target
 * @param replace
 * @return
 */
char *replaceStrFromToEnd(int start, int end, char *target, char *replace) {
    char *previous = (char *) malloc(sizeof(char) * start);
    memcpy(previous, target, (size_t) start);
    char *suffix = target + end;

    int len = (int) (strlen(target) - (end - start) + strlen(replace));
    char *res = (char *) malloc(sizeof(char) * (len + 1024));
    sprintf(res, "%s%s%s", previous, replace, suffix);

    free(previous);
    return res;
}

