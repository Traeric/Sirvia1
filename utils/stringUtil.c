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


/**
 * 检测某个字符串中是否包含某个字符
 * @param target
 * @param match
 * @return
 */
int contains(char *target, const char *match) {
    char first = *(match++);
    // 匹配第一个字符在目标字符串中的起始位置
    while (target++) {
        if (*target == first) {
            // 如果已经匹配到了就继续匹配下面的字符
            for (; *match != '\0'; match++, target++) {
                if (*match != *target) {
                    // 不相等
                    return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}
