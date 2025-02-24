#include "hash.h"

// FNV-1a 64位哈希算法
uint64_t fnv1a_64(const char* filename) {
    uint64_t hash = FNV1A_64_INIT;
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    // 使用缓冲区读取文件
    unsigned char buffer[4096];
    size_t bytes_read;
    // 逐块读取文件并计算哈希值
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            hash ^= buffer[i];
            hash *= FNV1A_64_PRIME;
        }
    }
    fclose(file);
    // 打印哈希值（十六进制格式）
    printf("文件 %s 的哈希值: 0x%016llx\n", filename, hash);
    return hash;
}