#ifndef SD_LOGGER_H
#define SD_LOGGER_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t timestamp_ms;
    float temp;
    float humidity;
    uint16_t light_adc;
} sensor_log_record_t;

bool sd_logger_init(void);
bool sd_logger_log(const sensor_log_record_t *record); // write measurements to csv
bool sd_logger_sync(void); // flush pending writes to sd
bool sd_logger_close(void);

#endif