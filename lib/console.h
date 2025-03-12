#ifndef H_CONSOLE_H
#define H_CONSOLE_H

#include "magic.h"

#define KLOG(FMT, ...) printf("[LOG]: " #FMT "\n", __VA_ARGS__)
#define KLOG_INFO(FMT, ...) printf("[INFO]: " #FMT "\n", __VA_ARGS__)
#define KLOG_WARNING(FMT, ...) fprintf(stderr, "[WARNING]: " #FMT "\n", __VA_ARGS__)
#define KLOG_ERROR(FMT, ...) fprintf(stderr, "[ERROR]: " #FMT "\n", __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void konsoli_single_thread();

#ifdef __cplusplus
}
#endif
   
#endif
