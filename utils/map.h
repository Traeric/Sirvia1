/**
 * Author: Eric JIn
 * Date: 2019-03-26
 */
#ifndef MAP_H_
#define MAP_H_

#include "cJSON.h"

// 定义一个map 字符串对字符串
typedef struct MAP {
    char *key;
    char *value;
    struct MAP *next;
} map, *p_map;

// 添加元素
p_map addItem(p_map dict, char key[], char value[]);

// 查询元素
p_map findItem(p_map dict, char key[]);

// 定义一个结构体，字符串对方法
typedef struct urlMap {
    char *key;
    char *(*value)(cJSON *);
    struct urlMap *next;
} urlmap, *fp_urlmap;

fp_urlmap addNode(fp_urlmap node, char *key, char *(*value)(cJSON *));
fp_urlmap findNode(fp_urlmap node, char *key);
#endif

