// FNV-1a 64λ��ϣ�㷨�ĳ�ʼֵ
#define FNV1A_64_INIT 0xcbf29ce484222325ULL
// FNV-1a 64λ��ϣ�㷨������
#define FNV1A_64_PRIME 0x100000001b3

#include <stdio.h>
#include <stdint.h>

uint64_t fnv1a_64(const char* filename);
