#ifndef DEBUG_H_
#define DEBUG_H_

#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#include <stdio.h>
#include <time.h>

#ifdef NDEBUG
#define LOG_TIME_INIT
#define LOG_TIME_REPEAT(M, ...)
#define SEP(M)
#else
#define LOG_TIME_INIT double start = clock();
#define LOG_TIME_REPEAT(ID, M, ...) printf("[%d] Took: %8.3fms - " M "\n", ID, (clock() - start) / 1000, ##__VA_ARGS__); start = clock();
#define SEP(M, ...) {printf("==================================== " M "\n", ##__VA_ARGS__); start = clock();}
#endif

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define PRINT(M) for(UNICHR *X=M;*X;X++) { printf("%c", *X); }

//#define clean_errno() (errno == 0 ? "None" : strerror(errno))
//
//#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
//
//#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
//
//#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
//
#define throw(M, ...) { fprintf(stderr, "[FATAL] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); exit(1); }
//
//#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
//
//#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
//
//#define check_mem(A) check((A), "Out of memory.")
//
//#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
