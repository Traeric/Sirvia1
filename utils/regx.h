/**
 * Author: Eric JIn
 * Date: 2019-04-02
 */
#ifndef REGX_H_
#define REGX_H_

#include "../utils/cJSON.h"

extern char *matchRegxOne(char *pattern, char *buffer);
char *replaceStr(char *pattern, char *content, cJSON *args, char *(*callback)(char *, cJSON *));
char *matchContain(char *pattern, char *buffer);

#endif
