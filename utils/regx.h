/**
 * Author: Eric JIn
 * Date: 2019-04-02
 */
#ifndef REGX_H_
#define REGX_H_

extern char *matchRegxOne(char *pattern, char *buffer);
char *replaceStr(char *pattern, char *content, cJSON *request, char *(*callback)(char *, cJSON *));

#endif
