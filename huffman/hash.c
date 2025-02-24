#include "hash.h"

// FNV-1a 64λ��ϣ�㷨
uint64_t fnv1a_64(const char* filename) {
    uint64_t hash = FNV1A_64_INIT;
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        printf("�޷����ļ� %s\n", filename);
        return 0;
    }
    // ʹ�û�������ȡ�ļ�
    unsigned char buffer[4096];
    size_t bytes_read;
    // ����ȡ�ļ��������ϣֵ
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            hash ^= buffer[i];
            hash *= FNV1A_64_PRIME;
        }
    }
    fclose(file);
    // ��ӡ��ϣֵ��ʮ�����Ƹ�ʽ��
    printf("�ļ� %s �Ĺ�ϣֵ: 0x%016llx\n", filename, hash);
    return hash;
}