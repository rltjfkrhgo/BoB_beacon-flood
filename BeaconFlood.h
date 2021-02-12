// BeaconFlood.h

#ifndef _BEACON_FLOOD_H_
#define _BEACON_FLOOD_H_

#include <sys/types.h>
#include <cstring>
#include <random>

#pragma pack(push, 1)
struct dot11_management_tag
{
    const static u_int8_t  NUM_BSSID = 0x00;
    const static u_int8_t  NUM_SUPPORTED_RATES = 0x01;
    const static u_int8_t  NUM_DS_PARAMETER_SET = 0x03;
    const static u_int8_t  NUM_CF_PARAMETER_SET = 0x04;
    const static u_int8_t  NUM_TIM = 0x05;

    u_int8_t  num;
    u_int8_t  len;
    u_int8_t  data;  // 시작 지점
};
#pragma pack(pop)

#pragma pack(push, 1)
struct beacon_frame
{
    static const u_int8_t  TYPE = 0x80;

    u_int8_t   radiotab[12];
    u_int8_t   type;
    u_int8_t   flags;
    u_int8_t   duration[2];
    u_int8_t   receiver[6];
    u_int8_t   transmitter[6];
    u_int8_t   bssid[6];
    u_int16_t  fragment:4;
    u_int16_t  sequence:12;
    // management
    // fixed
    u_int64_t  timestamp;
    u_int16_t  interval;
    u_int16_t  capabilities;
    // tagged
    u_int8_t   tag;  // tag 시작 지점
};
#pragma pack(pop)

typedef struct dot11_management_tag Dot11Tag;
typedef struct beacon_frame BeaconFrame;

BeaconFrame* newBeaconFrame(const char* ssid, u_int8_t ssidLen, size_t* beaconLen);
void deleteBeaconFrame(BeaconFrame* beacon);

#endif