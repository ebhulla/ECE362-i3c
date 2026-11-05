#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "ff.h"
#include "storage/sd_logger.h"

#define LOG_FILE_NAME "DATA.CSV"

static FATFS filesystem;
static FIL log_file;
static bool mounted = false;
static bool file_opened = false;

// write string to file
static bool write_string(const char *string) {
    uint bytes_written = 0;
    uint length = (uint)strlen(string);
    FRESULT res = f_write(&log_file, string, length, &bytes_written);
    return (res == FR_OK && bytes_written == length);
}

bool sd_logger_init(void) {
    FRESULT result;
    result = f_mount(&filesystem, "", 1);
    if (result != FR_OK) {
        return false;
    }
    mounted = true;
    // open file or make a new one
    result = f_open(&log_file, LOG_FILE_NAME, FA_WRITE | FA_OPEN_ALWAYS);
    if (result != FR_OK) {
        sd_logger_close();
        return false;
    }

    // csv file header 
    if (f_size(&log_file) == 0) {
        const char *header = "timestamp_ms,temp,humidity_percent,light)adc\r\n";
        if (!write_string(header)) {
            sd_logger_close();
            return false;
        }
    return true;
    }
}
// append one measurement
bool sd_logger_log(const sensor_log_record_t *record) {
    char line[128];
    int length = snprintf(line, sizeof(line), "%llu,%.2f,%.2f,%u\r\n", record->timestamp_ms, record->temp, record->humidity, record->light_adc);
    if (length < 0 || length >= sizeof(line)) {
        return false; // error or line too long
    }
    uint bytes_written = 0;
    FRESULT result = f_write(&log_file, line, (uint)length, &bytes_written);
    if (result != FR_OK || bytes_written != (uint)length) {
        return false;
    }
    return true;
}

// flush pending
bool sd_logger_sync(void) {
    FRESULT result = f_sync(&log_file);
    return (result == FR_OK);
}
 // close and unmount
 bool sd_logger_close(void) {
    bool success = true;
    if (file_opened) {
        if (f_sync(&log_file) != FR_OK) {
            success = false;
        }
        if (f_close(&log_file) != FR_OK) {
            success = false;
        }
        file_opened = false;
    }
    if (mounted) {
        f_mount(NULL, "", 0);
        mounted = false;
    }
    return success;
 }