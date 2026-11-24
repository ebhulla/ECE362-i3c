#include <stdio.h>
#include <string.h>
#include "sensor/sht40.h"

static void check(const char *name,bool ok){
    printf("%s: %s\n", name, ok ? "PASS" : "FAIL");
}

void sht40_run_tests(void){

    uint8_t crc_in[2] = {0xBE,0xEF};
    uint8_t crc = sht40_crc8(crc_in,2);
    check("crc8 BEEF == 0x92", crc == 0x92);

        // ----Test2: Valid Parse ----- 
    uint8_t buf[6] = {0x66,0x66,0x00,0x80,0x00,0x00};

    buf[2] = sht40_crc8(buf + 0,2);
    buf[5] = sht40_crc8(buf + 3,2);

    float t = 0.0f, rh = 0.0f;
    bool ok = false;

    ok = sht40_parse(buf,&t,&rh);
    check("parse valid returns true",ok);
    check("parse temp ~25.0", fabsf(t - 25.0f) < 0.1f);
    check("parse rh ~56.5", fabsf(rh - 56.5f) < 0.1f);

    // ---- Test 3: corrupted byte ----
    uint8_t bad[6];
    memcpy(bad,buf,sizeof(buf));
    bad[0] ^= 0x01;
    check("parse corrupted returns false", !(sht40_parse(bad,&t,&rh)));

    // ---- Test 4: RH clamp ----
    check("rh raw 0 -> 0", fabsf(sht40_raw_to_rh(0) - 0.0f) < 0.1f);       
    check("rh raw 65535 -> 100", fabsf(sht40_raw_to_rh(65535) - 100) < 0.1f); 

    check("temp raw 0 -> -40",fabsf(sht40_raw_to_celsius(0) -(-40)) < 0.1f);
    check("temp raw 65535 -> 100", fabsf(sht40_raw_to_celsius(65535)-125) < 0.1f);
}

