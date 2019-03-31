#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Author: Eric JIn
 * Date: 2019-03-27
 */

int file_size(char *);

/**
 * 读取文件内容到buff中
 * @param buff
 * @param path
 */
void loadFile(char buff[], char path[]) {
    FILE *fp;
    int ch = 0;
    int i = 0;
    // 打开文件
    if ((fp = fopen(path, "rb")) == NULL) {
        printf("读取失败\n");
        return;
    }
    // 关闭文件
    while (!feof(fp)) {
        ch = fgetc(fp);
        buff[i++] = (char) ch;
    }
    fclose(fp);
}

//获取文件名为filename的文件大小。
int file_size(char filename[]) {
    FILE *fp = fopen(filename, "rb");//打开文件。
    int size;
    // 打开文件失败
    if (fp == NULL) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);  // 定位文件指针到文件尾。
    size = (int) ftell(fp);  // 获取文件指针偏移量，即文件大小。
    fclose(fp);   // 关闭文件。
    return size;
}

int readImage(char buffer[], char filepath[]) {
    FILE * fp;
    int len;

    /* read data */
    if ((fp = fopen(filepath, "rb")) == NULL) {
        printf("读取图片失败\n");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    // 获取文件长度
    len = (int) ftell(fp);

    fseek(fp, 0, SEEK_SET);

    fread(buffer, 1, (size_t) len, fp);
    fclose(fp);
    return 0;
}
