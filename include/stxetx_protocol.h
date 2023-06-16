#ifndef _TYPES_H
#define _TYPES_H

#include <inttypes.h>

/*  ************************************************************
    * Parameter direction convention:
    *   [IN] - Will not be changed dby callee (function)
    *   [OUT] - Need not be initialized, will be provided by callee
    *   [INOUT] - Must be allocated by caller, will be populated by callee
    *   [OPT]   - Optional, can be NULL
    ************************************************************ */

/* ************************************************************ */

#ifdef __cplusplus
	extern "C" {
#endif

typedef enum {
    MSG_TYPE_EMPTY = -1,
    MSG_TYPE_ACK = 1,
    MSG_TYPE_GO_FORWARD = 2,
	MSG_TYPE_GO_BACKWARD = 3,
	MSG_TYPE_ROTATE_LEFT = 4,
	MSG_TYPE_ROTATE_RIGHT = 5,
	MSG_TYPE_STOP = 6
} msg_type_e;

typedef enum {
    STXETX_ERROR_NO_ERROR = 0,
    STXETX_ERROR_INVALID_HANDLE = 10,
    STXETX_ERROR_BUFFER_TOO_SMALL = 11,
    STXETX_ERROR_STX_MISSING = 12,
	STXETX_ERROR_ETX_MISSING = 13
} stxetx_error_code_e;

/* ************************************************************ */

// STXETX Protocol Frame Flags
typedef enum {
    FLAG_SHOULD_ACK = (1 << 0),         /* Message must be acknowledged upon reception */
    FLAG_IGNORE_CHECKSUM = (1 << 1)     /* Do not check checksum on reception */
} stxetx_flag_e;

// STXETX Protocol Frame
typedef struct {
    uint8_t msg_type;       /* Message type */
    uint8_t flags;          /* See. flag_e enum */
    uint8_t len_bytes;      /* Length of payload in bytes */
    uint8_t checksum;       
    uint8_t* p_payload;     /* Payload contains raw, unescaped data */
} stxetx_frame_t;

// Takes `frame_t` object and encodes it into bytes (to `p_dest_buffer`)
// Argument `n` limits the number of bytes that can be read from `p_dest_buffer`
// Argument `p_bytes_written` is used as output parameter for number of bytes written.
uint8_t stxetx_encode_n(
    uint8_t* p_dest_buffer,         /* [INOUT]  Buffer where encoded frame will be stored    */
    stxetx_frame_t source,          /* [IN]     Frame that will be encoded into bytes        */
    uint32_t n,                     /* [IN]     Max number of bytes that can be written 
                                                                          to `p_dest_buffer` */
    uint8_t* p_bytes_written        /* [INOUT]  [OPT] Pointer to a buffer where number of 
                                                                written bytes will be stored */
);

// Takes an array of bytes `p_src_buffer` and decodes `frame_t` structure from it.
// Argument `n_src` limits the number of bytes that can be read from `p_src_buffer`
uint8_t stxetx_decode_n(
    uint8_t* p_src_buffer,          /* [IN]     Buffer where raw, encoded frame is stored                       */
    stxetx_frame_t* p_dest_obj,     /* [INOUT]  Empty frame which will be populated after decoding              */
    uint32_t n_src,                 /* [IN]     Max number of bytes that can be read from `p_src_buffer`        */
    uint8_t* p_payload_buffer,      /* [INOUT]  Allocated buffer that will be used to store decoded payload
                                            handle will be passed to the `p_dest_obj` frame's `p_payload`       */
    uint8_t n_payload               /* [IN]     Max number of bytes that can be written to `p_payload_buffer`   */
);

// Initializes `stxetx_frame_t` structure with default values.
uint8_t stxetx_init_empty_frame(stxetx_frame_t* p_frame);

// Adds payload data to the frame.
uint8_t stxetx_add_payload(stxetx_frame_t* p_frame, uint8_t* p_payload, uint8_t n);

uint8_t stxetx_is_character_escape(uint8_t character);
uint8_t stxetx_is_character_frame_start_delimiter(uint8_t character);
uint8_t stxetx_is_character_frame_end_delimiter(uint8_t character);

#ifdef __cplusplus
	}
#endif

#endif