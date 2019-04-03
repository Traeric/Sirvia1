/**
 * Author: Eric JIn
 * Date: 2019-03-26
 */
#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * 添加一个键值对
 * @param dict
 * @param key
 * @param value
 * @return
 */
p_map addItem(p_map dict, char key[], char value[]) {
    // 初始化一个结构体
    p_map item = NULL;
    item = (p_map) malloc(sizeof(map));
    item->key = key;
    item->value = value;
    // 将当前结构体连接在最后面
    item->next = dict;
    return item;
}

/**
 * 查找key对应的值
 * @param dict
 * @param key
 * @return
 */
p_map findItem(p_map dict, char key[]) {
    p_map target = dict;
    while (target != NULL) {
        if (strcmp(key, target->key) == 0) {
            // 找到正确的值
            return target;
        }
        target = target->next;
    }
    return NULL;
}


/**
 * 添加一个方法
 * @param node
 * @param key
 * @param value
 * @return
 */
fp_urlmap addNode(fp_urlmap node, char *key, cJSON *(*value)(cJSON *)) {
    // 初始化一个结构体
    fp_urlmap item = NULL;
    item = (fp_urlmap) malloc(sizeof(urlmap));
    item->key = key;
    item->value = value;
    // 将当前结构体连接在最后面
    item->next = node;
    return item;
}

fp_urlmap findNode(fp_urlmap node, char *key) {
    fp_urlmap target = node;
    while (target != NULL) {
        if (strcmp(key, target->key) == 0) {
            // 找到正确的值
            return target;
        }
        target = target->next;
    }
    return NULL;
}

