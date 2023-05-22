#include "stxetx_protocol.h"
#include "stxetx_defines.h"

#define ASCII_STX 0x02
#define ASCII_ETX 0x03
#define ASCII_ESCAPE '%'

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
    FRAME_FORMAT_UNKNOWN = -1,
    FRAME_FORMAT_CONTROL = 0,
    FRAME_FORMAT_DATA = 1
} stxetx_frame_format_t;


static inline uint8_t is_escape_character_(uint8_t character)
{
    return character == ASCII_ESCAPE;
}

static inline uint8_t is_control_character_(uint8_t character)
{
    return (character == ASCII_STX) || (character == ASCII_ETX) || (is_escape_character_(character));
}

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

// Returns pointer to destination buffer advanced by number of bytes written
static inline uint8_t* write_byte_to_buffer_(uint8_t* p_buffer, uint8_t byte_)
{
    if (is_control_character_(byte_))
    {
        *(p_buffer++) = ASCII_ESCAPE;
    }
    
    *(p_buffer++) = byte_;
    
    return p_buffer;
}

// Returns pointer to destination buffer advanced by number of bytes read
static inline uint8_t* read_byte_from_buffer_(uint8_t* p_buffer, uint8_t* p_destination)
{
    if (is_escape_character_(*p_buffer))
    {
        p_buffer++;
    }

    *p_destination = *(p_buffer++);

    return p_buffer;
}

static uint8_t encode_n_data_frame_(uint8_t* p_dest_buffer, stxetx_frame_t source, int n, uint8_t* p_bytes_written)
{
    // Data frame requires buffer of at least (6 + LEN) bytes
    if (n < 6 + source.len_bytes)
    {
        return ERROR_BUFFER_TOO_SMALL;
    }

    if (source.len_bytes > 0 && NULL == source.p_payload)
    {
        return ERROR_INVALID_HANDLE;
    }
    

    uint8_t* ptr = p_dest_buffer;

    // Do not do: ptr = write_byte_to_buffer_(ptr, ASCII_STX);
    // STX at the beginning and ETX at the end must not be escaped
    *(ptr++) = ASCII_STX;

    ptr = write_byte_to_buffer_(ptr, source.msg_type);
    ptr = write_byte_to_buffer_(ptr, source.flags);
    ptr = write_byte_to_buffer_(ptr, source.len_bytes);

    for (size_t i = 0; i < source.len_bytes; i++)
    {
        ptr = write_byte_to_buffer_(ptr, source.p_payload[i]);
    }

    ptr += source.len_bytes;

    if (source.flags & FLAG_IGNORE_CHECKSUM)
    {
        ptr = write_byte_to_buffer_(ptr, 0x00);
    }
    else
    {
        ptr = write_byte_to_buffer_(ptr, source.checksum);
    }
    
    // Do not do: ptr = write_byte_to_buffer_(ptr, ASCII_STX);
    // STX at the beginning and ETX at the end must not be escaped
    *(ptr++) = ASCII_ETX;
    
    if (NULL != p_bytes_written)
    {
        *p_bytes_written =  (uint8_t)(ptr - p_dest_buffer);
    }

    return ERROR_NO_ERROR;
}

static uint8_t encode_n_control_frame_(uint8_t* p_dest_buffer, stxetx_frame_t source, uint32_t n, uint8_t* p_bytes_written)
{
    // Control frame requires buffer of at least 5 bytes
    if (n < 5)
    {
        return ERROR_BUFFER_TOO_SMALL;
    }

    uint8_t* ptr = p_dest_buffer;

    // Do not do: ptr = write_byte_to_buffer_(ptr, ASCII_STX);
    // STX at the beginning and ETX at the end must not be escaped
    *(ptr++) = ASCII_STX;

    ptr = write_byte_to_buffer_(ptr, source.msg_type);
    ptr = write_byte_to_buffer_(ptr, source.flags);

    if (source.flags & FLAG_IGNORE_CHECKSUM)
    {
        ptr = write_byte_to_buffer_(ptr, 0x00);
    }
    else
    {
        ptr = write_byte_to_buffer_(ptr, source.checksum);
    }
    
    // Do not do: ptr = write_byte_to_buffer_(ptr, ASCII_STX);
    // STX at the beginning and ETX at the end must not be escaped
    *(ptr++) = ASCII_ETX;
    
    if (NULL != p_bytes_written)
    {
        *p_bytes_written =  (uint8_t)(ptr - p_dest_buffer);
    }

    return ERROR_NO_ERROR;
}

uint8_t stxetx_encode_n(uint8_t* p_dest_buffer, stxetx_frame_t source, uint32_t n, uint8_t* p_bytes_written)
{
    if (NULL == p_dest_buffer)
    {
        return ERROR_INVALID_HANDLE;
    }

    // Initialize to avoid confusion
    if (NULL != p_bytes_written)
    {
        *p_bytes_written = 0;
    }

    stxetx_frame_format_t frame_format = get_frame_format_(source.msg_type); 

    switch (frame_format)
    {
    case FRAME_FORMAT_DATA:
        return encode_n_data_frame_(p_dest_buffer, source, n, p_bytes_written);
        break;
    case FRAME_FORMAT_CONTROL:
        return encode_n_control_frame_(p_dest_buffer, source, n, p_bytes_written);
        break;
    default:
        return ERROR_INVALID_FRAME;
    }    
}


uint8_t stxetx_decode_n(uint8_t* p_src_buffer, stxetx_frame_t* p_dest_obj, uint32_t n)
{

}