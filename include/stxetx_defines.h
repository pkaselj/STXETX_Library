#ifndef STXETX_DEFINES_H_
#define STXETX_DEFINES_H_

// Message types and error codes defined in source file
// to avoid having to recompile all files that include the header

typedef enum {
    MSG_TYPE_EMPTY = -1,
    MSG_TYPE_ACK = 1,
    MSG_TYPE_GO_FORWARD = 2
} msg_type_e;

typedef enum {
    ERROR_NO_ERROR = 0,
    ERROR_INVALID_HANDLE = 1,
    ERROR_BUFFER_TOO_SMALL = 2,
    ERROR_INVALID_FRAME = 3
} error_code_e;

#endif