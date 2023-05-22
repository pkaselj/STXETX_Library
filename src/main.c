#include <stdio.h>

#include "stxetx_protocol.h"
#include "stxetx_defines.h"

#define N 100

#define STR(X) #X

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

void hex_to_hex_string(uint8_t* p_data, char* sz_data, uint8_t n)
{
    char* ptr = sz_data;
    for (size_t i = 0; i < n; i++)
    {
        uint8_t higher_nibble = (p_data[i] & 0xF0) >> 4;
        uint8_t lower_nibble  = (p_data[i] & 0x0F) ;

        (*ptr++) = ' '; // SPC
        (*ptr++) = '0';
        (*ptr++) = 'x';
        (*ptr++) = get_hex_string_from_nibble(higher_nibble);
        (*ptr++) = get_hex_string_from_nibble(lower_nibble);
    }

    *(ptr++) = 0x00;
}

void print_frame(stxetx_frame_t frame)
{
    char sz_payload[5*N] = {0};
    hex_to_hex_string(frame.p_payload, sz_payload, frame.len_bytes);

    printf(
        "frame.msg_type = %d\n"
        "frame.flags = %x\n"
        "frame.checksum = %d\n"
        "frame.len_bytes = %d\n"
        "frame.payload = %s\n",
        frame.msg_type,
        frame.flags,
        frame.checksum,
        frame.len_bytes,
        sz_payload
    );
}

typedef void(*test_callback)(void);
void run_test(test_callback test, const char* sz_name)
{
    printf("/*********************************************/\n");
    printf("Running test: %s\n", sz_name);
    test();
    printf("/*********************************************/\n");
}

void test_encode(void)
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

    hex_to_hex_string(p_data_buffer, p_data_str, bytes_written);

    printf("Encoded data string: %s\n", p_data_str);
}

void test_decode_no_payload(void)
{
    uint8_t p_data_buffer[N] = 
    {0x02, 0x25, 0x02, 0x25, 0x03, 0x00, 0xFF, 0x03};
    uint8_t p_payload_buffer[N] = {0};

    stxetx_frame_t frame;

    (void)stxetx_decode_n(p_data_buffer, &frame, N, p_payload_buffer, N);

    print_frame(frame);
}

void test_decode_with_payload(void)
{
    uint8_t p_data_buffer[N] = 
    {0x02, 0x25, 0x02, 0x25, 0x03, 0x05, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x03};
    uint8_t p_payload_buffer[N] = {0};

    stxetx_frame_t frame;

    (void)stxetx_decode_n(p_data_buffer, &frame, N, p_payload_buffer, N);

    print_frame(frame);
}

int main(void)
{
    run_test(test_encode, STR(test_encode));
    run_test(test_decode_no_payload, STR(test_decode_no_payload));
    run_test(test_decode_with_payload, STR(test_decode_with_payload));
    return 0;
}