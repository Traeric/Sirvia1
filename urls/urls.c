/**
 * Author: Eric JIn
 * Date: 2019-03-31
 */
#include <stdlib.h>
#include <stdio.h>
#include "../utils/map.h"
#include "../handler/handler.h"

fp_urlmap urlMapping() {
    fp_urlmap node = NULL;
    // 添加url
    node = addNode(node, "/home", home);
    return node;
}

