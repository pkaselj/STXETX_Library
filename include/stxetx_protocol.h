#ifndef _TYPES_H
#define _TYPES_H

#include <inttypes.h>

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
    uint8_t* p_payload;
} stxetx_frame_t;

// Takes `frame_t` object and encodes it into bytes (to `p_dest_buffer`)
// Argument `n` limits the number of bytes that can be read from `p_dest_buffer`
// Argument `p_bytes_written` is used as output parameter for number of bytes written.
uint8_t stxetx_encode_n(uint8_t* p_dest_buffer, stxetx_frame_t source, uint32_t n, uint8_t* p_bytes_written);

// Takes an array of bytes `p_src_buffer` and decodes `frame_t` structure from it.
// Argument `n` limits the number of bytes that can be read from `p_src_buffer`
uint8_t stxetx_decode_n(uint8_t* p_src_buffer, stxetx_frame_t* p_dest_obj, uint32_t n);

#endif