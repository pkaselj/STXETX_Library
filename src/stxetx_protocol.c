#include "stxetx_protocol.h"

#define ASCII_STX 0x02
#define ASCII_ETX 0x03

// Message types and error codes defined in source file
// to avoid having to recompile all files that include the header

typedef enum {
    MSG_TYPE_EMPTY = (unsigned int)-1,
    MSG_TYPE_ACK = 1,
    MSG_TYPE_GO_FORWARD = 2
} msg_type_e;

typedef enum {
    ERROR_NO_ERROR = 0,
    ERROR_INVALID_HANDLE = 1,
    ERROR_BUFFER_TOO_SMALL = 2,
    ERROR_INVALID_FRAME = 3
} error_code_e;

// Frame formats:

// Control Frame
// Used for control messages i.e. messages without payload such as ACK message

//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      STX      |      TYPE     |     FLAGS     |    CHECKSUM   |       ETX     |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// Data Frame
// Used for messages with payload
//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      STX      |      TYPE     |     FLAGS     |     LENGTH    |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   PAYLOAD[0]  |   PAYLOAD[1]  |      ...      | PAYLOAD[LEN-1]|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |    CHECKSUM   |       ETX     |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

typedef enum {
    FRAME_FORMAT_UNKNOWN = (unsigned int)-1,
    FRAME_FORMAT_CONTROL = 0,
    FRAME_FORMAT_DATA = 1
} frame_format_t;

static uint8_t get_frame_format_(uint8_t type)
{
    switch (type)
    {
    case MSG_TYPE_EMPTY:
        return FRAME_FORMAT_CONTROL;
    case MSG_TYPE_ACK:
        return FRAME_FORMAT_CONTROL;
    case MSG_TYPE_GO_FORWARD:
        return FRAME_FORMAT_DATA;
    default:
        return FRAME_FORMAT_UNKNOWN;
    }
}  

static uint8_t encode_n_data_frame_(uint8_t* p_dest_buffer, frame_t source, int n, uint8_t* p_bytes_written)
{
    // Data frame requires buffer of at least (6 + LEN) bytes
    if (n < 6 + source.len_bytes)
    {
        return ERROR_BUFFER_TOO_SMALL;
    }

    uint8_t* ptr = p_dest_buffer;

    *(ptr++) = ASCII_STX;
    *(ptr++) = source.msg_type;
    *(ptr++) = source.flags;
    *(ptr++) = source.len_bytes;

    for (size_t i = 0; i < source.len_bytes; i++)
    {
        ptr[i] = source.p_payload[i];
    }

    ptr += source.len_bytes;

    if (source.flags & FLAG_IGNORE_CHECKSUM)
    {
        *(ptr++) = 0x00;
    }
    else
    {
        *(ptr++) = source.checksum;
    }
    
    *(ptr++) = ASCII_ETX;
    
    *p_bytes_written = 6 + source.len_bytes;
    return ERROR_NO_ERROR;
}

static uint8_t encode_n_control_frame_(uint8_t* p_dest_buffer, frame_t source, uint32_t n, uint8_t* p_bytes_written)
{
    // Control frame requires buffer of at least 5 bytes
    if (n < 5)
    {
        return ERROR_BUFFER_TOO_SMALL;
    }

    uint8_t* ptr = p_dest_buffer;

    *(ptr++) = ASCII_STX;
    *(ptr++) = source.msg_type;
    *(ptr++) = source.flags;

    if (source.flags & FLAG_IGNORE_CHECKSUM)
    {
        *(ptr++) = 0x00;
    }
    else
    {
        *(ptr++) = source.checksum;
    }
    
    *(ptr++) = ASCII_ETX;
    
    *p_bytes_written = 5;
    return ERROR_NO_ERROR;
}

uint8_t encode_n(uint8_t* p_dest_buffer, frame_t source, uint32_t n, uint8_t* p_bytes_written)
{
    if (NULL == p_dest_buffer || NULL == p_bytes_written)
    {
        return ERROR_INVALID_HANDLE;
    }

    // Initialize to avoid confusion
    *p_bytes_written = 0;

    frame_format_t frame_format = get_frame_format_(source.msg_type); 

    switch (frame_format)
    {
    case FRAME_FORMAT_DATA:
        return encode_n_data_frame_(p_dest_buffer, source, n);
        break;
    case FRAME_FORMAT_CONTROL:
        return encode_n_control_frame_(p_dest_buffer, source, n);
        break;
    default:
        return ERROR_INVALID_FRAME;
    }    
}


uint8_t decode_n(uint8_t* p_src_buffer, frame_t* p_dest_obj, int n)
{

}