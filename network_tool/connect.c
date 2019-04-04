//
// Created by ericjin on 3/26/19.
//
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <zconf.h>
#include "../utils/map.h"
#include "../utils/stringUtil.h"
#include "../utils/fileImport.h"
#include "../urls/urls.h"
#include "../utils/cJSON.h"
#include "../utils/regx.h"
#include "../utils/templateSplite.h"

#define PORT 4140        // 服务器监听端口
#define MAX_QUEUE 20     // 待连接的队列最大数
#define MAX_LEN 20480    // 接收浏览器数据

static void analysisRequest(char *, cJSON *);

static void analysisPath(cJSON *, int);

/**
 * 开始启动socket
 */
void startConnet() {
    int server_socket_fp;   // 服务端socket句柄
    struct sockaddr_in server_addr;   // 地址结构体
    char buff[MAX_LEN];

    /**
     * 创建socket
     * AF_INET: IPV4网络操作
     * SOCK_STREAM: 使用TCP协议传输数据
     * 0: 自动选择协议
     */
    if ((server_socket_fp = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket failed!\n");
        exit(EXIT_FAILURE);
    }

    /**
     * 将socket与地址绑定
     * bind参数：
     *     socket描述符
     *     地址结构体，需要强转成struct sockaddr类型
     *     结构体长度，传入的是指针，无法再函数内部获取长度
     */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // INADDR_ANY表示任何地址访问都可以
    server_addr.sin_port = htons(PORT);   // 端口号
    if (bind(server_socket_fp, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("bind socket with addr struct failed!\n");
        exit(EXIT_FAILURE);
    }

    /**
     * 监听链接
     * 参数：
     *     socket描述符
     *     待连接队列的最大个数，一般为5
     */
    listen(server_socket_fp, MAX_QUEUE);

    while (1) {
        /**
         * 与客户端建立连接
         * 参数：
         *     socket文件描述符
         *     客户端地址信息的结构体，不关心可以传NULL
         *     客户端地址长度，不关心可以传NULL
         * 返回值：
         *     socket文件描述符，在与客户端建立连接后，accpet还是会生成一个专门用于和当前客户端通信的socket，
         *     而原来那个socket照常负责和其他等待建立连接的客户端建立通信
         */
        int client_socket_fp;
        if ((client_socket_fp = accept(server_socket_fp, NULL, NULL)) < 0) {
            printf("accept failed!\n");
            break;
        }

        /**
         * 读取浏览器的内容
         * 参数：
         *     文件描述符，从哪个文件读，这里填写accept返回的文件句柄
         *     读的内容存到buff中
         *     共读多少个字节
         */
        read(client_socket_fp, buff, MAX_LEN);

        // 分析请求信息
        cJSON *request = cJSON_CreateObject();  // 创建一个json对象
        analysisRequest(buff, request);   // 解析参数，获取前端传来的参数已经路径等信息

        // 执行指定函数
        analysisPath(request, client_socket_fp);
        
        close(client_socket_fp);
    }
    close(server_socket_fp);
}

/**
 * 获取返回的参数，包括请求方式以及请求的路径等等
 * @param str
 * @param request
 * @return
 */
static void analysisRequest(char *str, cJSON *request) {
    char *p = strtok(str, "\r\n"); // 获取第一行的数据，提取出请求方式跟请求路径
    // 处理post请求的参数
    char *postStr = strtok(NULL, "\0");

    // 对第一行的数据进行分割
    char *item = strtok(p, " ");   // 按照空格进行分隔
    cJSON_AddItemToObject(request, "method", cJSON_CreateString(item));  // 将请求方式保存
    item = strtok(NULL, " ");    // 再次分隔，分割出路径
    cJSON_AddItemToObject(request, "path", cJSON_CreateString(item));
    // 对post和get方式进行分别解析
    char *r;
    // get方式
    if (strcmp("GET", cJSON_GetObjectItem(request, "method")->valuestring) == 0) {
        // 解析url
        char *buff = cJSON_GetObjectItem(request, "path")->valuestring;
        r = strsep(&buff, "?");
        // 重新设置path，将url跟参数分开
        cJSON_GetObjectItem(request, "path")->valuestring = r;
        char *arg = strsep(&buff, "?"); // 获取到url后面的参数

        char *argItem = strsep(&arg, "&");  // 拿到url后面的参数
        // 设置请求参数的json放在根节点的args下
        cJSON *args = cJSON_CreateObject();
        cJSON_AddItemToObject(request, "args", args);
        while (argItem != NULL) {
            // 对name=eric解析
            char *k = strsep(&argItem, "=");
            char *v = strsep(&argItem, "=");
            cJSON_AddItemToObject(args, k, cJSON_CreateString(v));
            argItem = strsep(&arg, "&");
        }
    } else if (strcmp("POST", cJSON_GetObjectItem(request, "method")->valuestring) == 0) {
        // 解析POST请求
        char *pattern = "\\{.+\\}";  // 正则表达式
        char *argJsonStr = matchRegxOne(pattern, postStr);   // 参数对应的字符串
        // 转换成cJSON格式
        cJSON *args = cJSON_Parse(argJsonStr);
        free(argJsonStr);  // 在matchRegxOne中申请的空间在此释放掉
        cJSON_AddItemToObject(request, "args", args);
    }
}

/**
 * 解析请求路径
 * @param path
 * @param socket_fp
 */
static void analysisPath(cJSON *request, int socket_fp) {
    char *path = cJSON_GetObjectItem(request, "path")->valuestring;
    // 分析是对服务器资源请求还是其他请求
    char *content;   // 文件内容
    int fileSize;   // 文件长度
    int length = 0;   // 推送的长度
    char *p_bufs = NULL;  // 最终推送到浏览器的数据

    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg") || endsWith(path, ".png")
        || endsWith(path, ".gif") || endsWith(path, ".xml") || endsWith(path, ".ico")
        || endsWith(path, ".js")) {
        // 获取文件内容，静态目录static
        // 拼接路径
        char filePath[] = "./statics";
        strcat(filePath, path);
        // 读取文件
        fileSize = file_size(filePath);
        content = (char *) malloc(sizeof(char) * fileSize);
        int ret = readImage(content, filePath);
        // 未读取到文件
        if (ret == -1) {
            return;
        }

        p_bufs = (char *) malloc(sizeof(char) * (fileSize + 1024));
        if (NULL == p_bufs) {
            printf("malloc error!\n");
            return;
        }
        // 返回给浏览器的类型
        char *type = "image/jpeg";
        if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) {
            type = "image/jpeg";
        } else if (endsWith(path, ".png")) {
            type = "image/png";
        } else if (endsWith(path, ".gif")) {
            type = "image/gif";
        } else if (endsWith(path, ".xml")) {
            type = "text/xml";
        } else if (endsWith(path, ".ico")) {
            type = "image/x-icon";
        } else if (endsWith(path, ".js")) {
            type = "application/javascript";
        }
        length = sprintf(p_bufs, "HTTP/1.1 200 OK\r\n"
                                 "Server: hsoap/2.8\r\n"
                                 "Content-Type: %s\r\n"
                                 "Content-Length: %d\r\n"
                                 "Connection: close\r\n\r\n", type, fileSize);
    } else {
        // 其他请求，类似于/home
        // 如果是其他请求，就去urls.c中匹配对应的方法进行执行
        fp_urlmap node = urlMapping();   // 获取所有的url跟方法的匹配
        // 通过当前的url跟方法匹配
        if (findNode(node, path) == NULL) {
            return;
        }
        cJSON *(*func)(cJSON *) = findNode(node, path)->value;
        // 执行方法，获取返回值
        cJSON *resultData = func(request);
        char *filePath = cJSON_GetObjectItem(resultData, "target")->valuestring;
        // 进行路径拼接
        char temp[] = "./templates/";
        strcat(temp, filePath);
        // 读取文件
        fileSize = file_size(temp);
        content = (char *) malloc(sizeof(char) * fileSize);
        int ret = readImage(content, temp);   // 读取文件
        // 如果没有读取到文件
        if (ret == -1) {
            return;
        }
        p_bufs = (char *) malloc(sizeof(char) * (fileSize + 1024));
        length = sprintf(p_bufs, "HTTP/1.1 200 OK\r\n"
                               "Server: hsoap/2.8\r\n"
                               "Content-Type: text/html\r\n"
                               "Content-Length: %d\r\n"
                               "Connection: close\r\n\r\n", fileSize);
        // 在这里面对html文件中的数据进行替换
        content = handlerTemplate(content, resultData);   // 传入用户添加的变量
    }

    memcpy(p_bufs + length, content, (size_t) fileSize);
    length += fileSize;
    send(socket_fp, p_bufs, (size_t) length, 0);

    free(p_bufs);
    free(content);
}

