#include <stdio.h>

#include "stxetx_protocol.h"
#include "stxetx_defines.h"

#define N 100

const char g_nibble_hex_lookup_table[16] = {
            '0', /* 0000 */
            '1', /* 0001 */
            '2', /* 0010 */
            '3', /* 0011 */
            '4', /* 0100 */
            '5', /* 0101 */
            '6', /* 0110 */
            '7', /* 0111 */
            '8', /* 1000 */
            '9', /* 1001 */
            'A', /* 1010 */
            'B', /* 1011 */
            'C', /* 1100 */
            'D', /* 1101 */
            'E', /* 1110 */
            'F'  /* 1111 */
};

char get_hex_string_from_nibble(uint8_t nibble)
{
    // Take only the lower nibble of a byte
    nibble = nibble & 0x0F;
    return g_nibble_hex_lookup_table[nibble];
} 

int main(void)
{
    uint8_t p_data_buffer[N] = {0};
    char    p_data_str[5*N]  = {0}; // 5*N Because each byte is
                                    // encoded in "(SPC)0xAA" format

    stxetx_frame_t data = {
        .msg_type = MSG_TYPE_ACK,
        .flags = FLAG_SHOULD_ACK | FLAG_IGNORE_CHECKSUM,
        .p_payload = NULL,
        .len_bytes = 0,
        .checksum = 0
    };

    uint8_t bytes_written = 0;
    stxetx_encode_n(p_data_buffer, data, N, &bytes_written);

    char* ptr = p_data_str;
    for (size_t i = 0; i < bytes_written; i++)
    {
        uint8_t higher_nibble = (p_data_buffer[i] & 0xF0) >> 4;
        uint8_t lower_nibble  = (p_data_buffer[i] & 0x0F) ;

        (*ptr++) = ' '; // SPC
        (*ptr++) = '0';
        (*ptr++) = 'x';
        (*ptr++) = get_hex_string_from_nibble(higher_nibble);
        (*ptr++) = get_hex_string_from_nibble(lower_nibble);
    }

    *(ptr++) = 0x00;

    printf("Encoded data string: %s\n", p_data_str);

    return 0;
}