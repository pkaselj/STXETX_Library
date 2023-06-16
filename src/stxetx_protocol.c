#include "stxetx_protocol.h"

#define ASCII_STX 0x02
#define ASCII_ETX 0x03
#define ASCII_ESCAPE (uint8_t)'%'

#ifndef NULL
#define NULL (void*)0x00
#endif

// Frame Format
//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      STX      |      TYPE     |     FLAGS     |     LENGTH    |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   PAYLOAD[0]  |   PAYLOAD[1]  |      ...      | PAYLOAD[LEN-1]|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |    CHECKSUM   |       ETX     |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

uint8_t stxetx_is_character_escape(uint8_t character)
{
	return character == ASCII_ESCAPE;
}

uint8_t stxetx_is_character_frame_start_delimiter(uint8_t character)
{
	return character == ASCII_STX;
}

uint8_t stxetx_is_character_frame_end_delimiter(uint8_t character)
{
	return character == ASCII_ETX;
}

static inline uint8_t is_control_character_(uint8_t character)
{
	return stxetx_is_character_escape(character)
	|| stxetx_is_character_frame_end_delimiter(character)
	|| stxetx_is_character_frame_start_delimiter(character);
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
	if (stxetx_is_character_escape(*p_buffer))
	{
		p_buffer++;
	}

	*p_destination = *(p_buffer++);

	return p_buffer;
}


uint8_t stxetx_encode_n(uint8_t* p_dest_buffer, stxetx_frame_t source, uint32_t n, uint8_t* p_bytes_written)
{
	if (NULL == p_dest_buffer)
	{
		return STXETX_ERROR_INVALID_HANDLE;
	}

	// Initialize to avoid confusion
	if (NULL != p_bytes_written)
	{
		*p_bytes_written = 0;
	}

	// Data frame requires buffer of at least (6 + LEN) bytes
	//if (n < 6 + source.len_bytes)
	//{
		//return STXETX_ERROR_BUFFER_TOO_SMALL;
	//}

	if (source.len_bytes != 0 && NULL == source.p_payload)
	{
		return STXETX_ERROR_INVALID_HANDLE;
	}
	
	uint8_t* ptr = p_dest_buffer;

	// Do not do: ptr = write_byte_to_buffer_(ptr, ASCII_STX);
	// STX at the beginning and ETX at the end must not be escaped
	*(ptr++) = ASCII_STX;

	ptr = write_byte_to_buffer_(ptr, source.msg_type);
	ptr = write_byte_to_buffer_(ptr, source.flags);
	
	// Since payload length has to be calculated after writing
	// payload, because length can be a special character (STX, ETX, ESC)
	// Preemptively escape length field (has no effect other than adding another byte)
	(*ptr++) = ASCII_ESCAPE;
	// Write 0 to length field and save iterator to length field
	uint8_t* it_length_field = ptr;
	ptr = write_byte_to_buffer_(ptr, 0x00);

	uint8_t* it_payload_begin = ptr;
	for (uint32_t i = 0; i < source.len_bytes; i++)
	{
		ptr = write_byte_to_buffer_(ptr, source.p_payload[i]);
	}
	
	// After adding escapes
	uint8_t actual_payload_length = (uint8_t)(unsigned)(ptr - it_payload_begin);
	*it_length_field = actual_payload_length;
	
	//ptr += source.len_bytes;

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

	// TODO: Checksum
	
	if (NULL != p_bytes_written)
	{
		*p_bytes_written =  (uint8_t)(ptr - p_dest_buffer);
	}

	return STXETX_ERROR_NO_ERROR;

}

// TODO: Add guards (n_payload currently unused)
uint8_t stxetx_decode_n(
uint8_t* p_src_buffer,
stxetx_frame_t* p_dest_obj,
uint32_t n_src,
uint8_t* p_payload_buffer,
uint8_t n_payload
)
{
	if (NULL == p_src_buffer || NULL == p_dest_obj)
	{
		return STXETX_ERROR_INVALID_HANDLE;
	}

	// Frame requires buffer of at least 6 bytes
	//if (n_src < 6)
	//{
		//return STXETX_ERROR_BUFFER_TOO_SMALL;
	//}

	// Iterator (starts at the first frame data byte, right after STX)
	uint8_t* ptr = p_src_buffer;

	// Check if first byte is STX
	if (*(ptr++) != ASCII_STX)
	{
		return STXETX_ERROR_STX_MISSING;
	}
	
	ptr = read_byte_from_buffer_(ptr, &(p_dest_obj->msg_type));
	ptr = read_byte_from_buffer_(ptr, &(p_dest_obj->flags));
	
	// Length of payload in bytes including escape characters.
	// (Must be corrected after reading all bytes to exclude escape bytes)
	uint8_t payload_length_bytes = 0;
	ptr = read_byte_from_buffer_(ptr, &payload_length_bytes);

	if (payload_length_bytes > 0)
	{
		p_dest_obj->p_payload = p_payload_buffer;
		
		uint8_t* it_write_payload = p_payload_buffer;
		uint8_t* it_write_payload_begin = it_write_payload;
		
		// TODO: Add guards (n_payload currently unused)
		uint8_t* it_read_payload_end = ptr + payload_length_bytes;
		while (ptr != it_read_payload_end)
		{
			ptr = read_byte_from_buffer_(ptr, it_write_payload++);
		}
		
		// After removing escapes
		uint8_t actual_payload_size = (uint8_t)(unsigned)(it_write_payload - it_write_payload_begin);
		p_dest_obj->len_bytes = actual_payload_size;
	}
	else
	{
		p_dest_obj->p_payload = NULL;
	}
	
	ptr = read_byte_from_buffer_(ptr, &(p_dest_obj->checksum));

	// Check if last byte is ETX
	if (*(ptr++) != ASCII_ETX)
	{
		return STXETX_ERROR_ETX_MISSING;
	}

	// TODO: Checksum

	return STXETX_ERROR_NO_ERROR;
}


uint8_t stxetx_init_empty_frame(stxetx_frame_t* p_frame)
{
	if (NULL == p_frame)
	{
		return STXETX_ERROR_INVALID_HANDLE;
	}

	p_frame->msg_type = -1;
	p_frame->flags = 0;
	p_frame->len_bytes = 0;
	p_frame->p_payload = NULL;
	p_frame->checksum = 0;

	return STXETX_ERROR_NO_ERROR;
}


uint8_t stxetx_add_payload(stxetx_frame_t* p_frame, uint8_t* p_payload, uint8_t n)
{
	if (NULL == p_frame || NULL == p_payload)
	{
		return STXETX_ERROR_INVALID_HANDLE;
	}

	if (n < 1)
	{
		p_frame->len_bytes = 0;
		p_frame->p_payload = NULL;

		return STXETX_ERROR_BUFFER_TOO_SMALL;
	}

	p_frame->len_bytes = n;
	p_frame->p_payload = p_payload;

	return STXETX_ERROR_NO_ERROR;
}