#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

// Constants
#define TIME_BUFFER_SIZE 128
#define BINARY_DISPLAY_WIDTH 71
#define MAX_BINARY_BYTES 32

#define TYPE_64BIT "64bit"
#define TYPE_32BIT "32bit"

#define TIME_UNIX_EPOCH_0L 0L                   // 1970-01-01T00:00:00Z
#define TIME_UNIX_EPOCH_1755998625L 1755998625L // 2025-08-24T01:23:45Z
#define TIME_UNIX_EPOCH_2147483647L 2147483647L // 2038-01-19T03:14:07Z
#define TIME_UNIX_EPOCH_2147483648L 2147483648L // 2038-01-19T03:14:08Z (Overflow for 32-bit system)

/*
 * Y2038 Problem Demonstration
 *
 * Shows how 32-bit time storage overflows on January 19, 2038 at 03:14:08 UTC
 * causing time to "jump back" to December 13, 1901.
 */

/*
 * Helper function for error messages
 */
void print_error_row(const char *type_label, const char *error_msg)
{
    printf("| %-16s | %-20s | %-*s |\n", type_label, error_msg, BINARY_DISPLAY_WIDTH, "N/A");
}

/*
 * Prints table separator line with appropriate column widths
 */
void print_separator(void)
{
    putchar('+');
    for (int i = 0; i < 18; i++)
    {
        putchar('-');
    }
    putchar('+');
    for (int i = 0; i < 22; i++)
    {
        putchar('-');
    }
    putchar('+');
    for (int i = 0; i < BINARY_DISPLAY_WIDTH + 2; i++)
    {
        putchar('-');
    }
    putchar('+');
    putchar('\n');
}

/*
 * Prints table header with column labels
 */
void print_header(void)
{
    print_separator();
    puts("| Time stored in   | Actual Result        | Binary Representation                                                   |");
    print_separator();
}

/*
 * Prints binary representation of data with proper spacing and padding
 */
void print_binary(const void *data, int bytes)
{
    if (!data || bytes <= 0 || bytes > MAX_BINARY_BYTES)
    {
        for (int i = 0; i < BINARY_DISPLAY_WIDTH; i++)
        {
            putchar(' ');
        }
        return;
    }

    const unsigned char *ptr = (const unsigned char *)data;

    for (int i = 0; i < bytes; i++)
    {
        for (int bit = 7; bit >= 0; bit--)
        {
            putchar('0' + ((ptr[i] >> bit) & 1));
        }
        if (i < bytes - 1)
        {
            putchar(' ');
        }
    }

    // Pad to fixed width with overflow protection
    if (bytes <= INT_MAX / 8)
    {
        int total_bits = bytes * 8;
        int separator_spaces = (bytes > 1) ? (bytes - 1) : 0;
        int chars_written = total_bits + separator_spaces;

        if (chars_written < BINARY_DISPLAY_WIDTH)
        {
            for (int i = chars_written; i < BINARY_DISPLAY_WIDTH; i++)
            {
                putchar(' ');
            }
        }
    }
}

/*
 * Demonstrates Y2038 problem by showing timestamp in both 32-bit and 64-bit formats
 */
void print_y2038_demo(time_t timestamp, const char *type)
{
    char buffer[TIME_BUFFER_SIZE];

    if (type && strcmp(type, TYPE_32BIT) == 0)
    {
        int32_t time_32 = (int32_t)timestamp;
        time_t converted = (time_t)time_32;
        struct tm tm_buf;
        struct tm *tm = gmtime_r(&converted, &tm_buf);
        if (!tm)
        {
            print_error_row("time_t (32-bit)", "INVALID_TIME");
            return;
        }

        size_t result = strftime(buffer, sizeof(buffer) - 1, "%Y-%m-%dT%H:%M:%SZ", tm);
        if (result == 0)
        {
            print_error_row("time_t (32-bit)", "FORMAT_ERROR");
            return;
        }
        buffer[sizeof(buffer) - 1] = '\0';
        if (timestamp > TIME_UNIX_EPOCH_2147483647L)
        {
            printf("| time_t (32-bit)* | %-20s | ", buffer);
        }
        else
        {
            printf("| %-16s | %-20s | ", "time_t (32-bit)", buffer);
        }
        print_binary(&time_32, 4);
    }
    else
    {
        struct tm tm_buf;
        struct tm *tm = gmtime_r(&timestamp, &tm_buf);
        if (!tm)
        {
            print_error_row("time_t (64-bit)", "INVALID_TIME");
            return;
        }
        size_t result = strftime(buffer, sizeof(buffer) - 1, "%Y-%m-%dT%H:%M:%SZ", tm);
        if (result == 0)
        {
            print_error_row("time_t (64-bit)", "FORMAT_ERROR");
            return;
        }
        buffer[sizeof(buffer) - 1] = '\0';
        printf("| %-16s | %-20s | ", "time_t (64-bit)", buffer);
        print_binary(&timestamp, 8);
    }
    puts(" |");
}

/*
 * Prints input timestamp in human-readable format
 */
void print_input(time_t timestamp)
{
    char buffer[TIME_BUFFER_SIZE];
    struct tm tm_buf;
    struct tm *tm = gmtime_r(&timestamp, &tm_buf);
    if (!tm)
    {
        printf("Input: INVALID_TIME\n");
        return;
    }

    size_t result = strftime(buffer, sizeof(buffer) - 1, "%Y-%m-%dT%H:%M:%SZ", tm);
    if (result == 0)
    {
        printf("Input: FORMAT_ERROR\n");
        return;
    }
    buffer[sizeof(buffer) - 1] = '\0';
    printf("Input: %s\n", buffer);
}

int main(void)
{
    // Loop through all test cases
    time_t test_cases[] = {
        TIME_UNIX_EPOCH_0L,
        TIME_UNIX_EPOCH_1755998625L,
        TIME_UNIX_EPOCH_2147483647L,
        TIME_UNIX_EPOCH_2147483648L,
    };

    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++)
    {
        print_input(test_cases[i]);
        print_header();
        print_y2038_demo(test_cases[i], TYPE_64BIT);
        print_y2038_demo(test_cases[i], TYPE_32BIT);
        print_separator();
        printf("\n");
    }
    printf("Note: Row marked with * shows the Y2038 overflow problem.\n");

    return 0;
}
