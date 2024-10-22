#ifndef __LITE_QMI_DEMO_H__
#define __LITE_QMI_DEMO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "msgid.h"
#include "qmerrno.h"

#define MAX_SERVICE_NAME_SIZE   64
#define MAX_TEST_ID_INPUT_SIZE  16

#define MAX_DEBUG_BUF_SIZE  1024

typedef int (* pack_func) (pack_qmi_t*, uint8_t*, uint16_t*, void*);
typedef int (* unpack_func) (uint8_t*, uint16_t, void*);
typedef void (* dump_func)  (void*);
typedef void (* setup_params_func) (void);

#define pack_func_item pack_func* 
#define unpack_func_item unpack_func*
#define run_pack_item(x) ((pack_func) x)
#define run_unpack_item(x) ((unpack_func) x)

typedef struct{
	int               qmi_service;
	setup_params_func setup_params;
    pack_func_item    pack;
    char              pack_func_name[128];
    void*             pack_ptr;
    unpack_func_item  unpack;
    char              unpack_func_name[128];
    void*             unpack_ptr;
    dump_func         dump;
} testitem_t;

typedef enum _status_code 
{
    SUCCESS =  0,    // The operation succeeded.
    FAILURE = -1,    // The operation failed.    
} status_code, *pstatus_code;
#define INVALID_ID -1

//return if dump function input argument is NULL
#define CHECK_DUMP_ARG_PTR_IS_NULL if (NULL == ptr) { return ;};

//return if pointer is NULL
#define RETURN_IF_PTR_IS_NULL(x) if (NULL == x) { return ;};

#define IFPRINTF(s,p)   if( NULL != p ){ printf(s,*p); }

// This could be moved to a SDKLITE header
#define QMI_HDR_LEN      6

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif //__LITE_QMI_DEMO_H__

