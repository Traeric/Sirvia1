/**
 * Author: Eric JIn
 * Date: 2019-03-31
 */

#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

int endsWith(char *targetStr, char *matchStr);
char *replaceStrFromToEnd(int, int, char *, char *);
int contains(char *target, char match);
char *trim(char *target);

#endif

