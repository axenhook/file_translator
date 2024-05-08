#include "stdio.h"
#include "stdlib.h"

int translate_file(char *file_name, unsigned int code, unsigned int data_size) {
    int size;
    int read_size = data_size * 2;
    int write_size = data_size;
    
    char *buf = malloc(read_size);
    if (buf == NULL) {
        printf("Memory allocation failed\n");
        return -5;
    }
    
    unsigned int *dat = (unsigned int *)buf;
    unsigned int dat_num = data_size / sizeof(unsigned int);

    //printf("file_name: %s, code: 0x%x, data_size: %d\n", file_name, code, data_size);
    //printf("read_size: %d, write_size: %d, data_num: %d\n", read_size, write_size, dat_num);

    FILE *fp = fopen(file_name, "rb+");
    if (fp == NULL) {
        printf("open %s failed\n", file_name);
        free(buf);
        return -1;
    }

    size = fread(buf, 1, read_size, fp);
    if (size != read_size) {
        printf("read %s failed, expect %d, real %d\n", file_name, read_size, size);
        fclose(fp);
        free(buf);
        return -2;
    }

    for (int i = 0; i < dat_num; i++) {
        dat[i] ^= dat[i + dat_num];
        dat[i] ^= code;
    }

    if (fseek(fp, 0, SEEK_SET)) {
        printf("seek %s failed\n", file_name);
        fclose(fp);
        free(buf);
        return -3;
    }

    size = fwrite(buf, 1, write_size, fp);
    if (size != write_size) {
        printf("write %s failed, expect %d, real %d\n", file_name, write_size, size);
        fclose(fp);
        free(buf);
        return -4;
    }

    fclose(fp);
    free(buf);
    return 0;
}

int main(int argc, char *argv[]) {
    unsigned int code = 0;
    unsigned int data_size = 0;

    if (argc < 2) {
        printf("usage: %s file_name\n", argv[0]);
        return -1;
    }

    if (argc >= 3) {
        code = atoi(argv[2]);
    }

    if (argc >= 4) {
        data_size = atoi(argv[3]);
    }

    if (code == 0) {
        code = 0x12ab89ef;
    }

    if (data_size == 0) {
        data_size = 512;
    }

    data_size = (((data_size + sizeof(unsigned int) - 1)) / sizeof(unsigned int)) * sizeof(unsigned int);

    return translate_file(argv[1], code, data_size);
}

