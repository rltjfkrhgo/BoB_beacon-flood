// BeaconFlood.h

#ifndef _BEACON_FLOOD_H_
#define _BEACON_FLOOD_H_

#include <string>
#include <cstring>
#include <random>
#include <fstream>

#pragma pack(push, 1)
struct dot11_management_tag
{
    const static uint8_t  NUM_BSSID = 0x00;
    const static uint8_t  NUM_SUPPORTED_RATES = 0x01;
    const static uint8_t  NUM_DS_PARAMETER_SET = 0x03;
    const static uint8_t  NUM_CF_PARAMETER_SET = 0x04;
    const static uint8_t  NUM_TIM = 0x05;

    uint8_t  id;
    uint8_t  len;
    uint8_t* data() { return (uint8_t*)(this+1); }  // 시작 지점 주소 리턴
};
#pragma pack(pop)

#pragma pack(push, 1)
struct beacon_frame
{
    static const uint8_t  TYPE = 0x80;

    uint8_t   radiotap[12];
    uint8_t   type;
    uint8_t   flags;
    uint8_t   duration[2];
    uint8_t   receiver[6];
    uint8_t   transmitter[6];
    uint8_t   bssid[6];
    uint16_t  fragment:4;
    uint16_t  sequence:12;
    // management
    // fixed
    uint64_t  timestamp;
    uint16_t  interval;
    uint16_t  capabilities;
    // tagged
    uint8_t*  tag() { return (uint8_t*)(this+1); }  // tag 시작 지점 주소 리턴
};
#pragma pack(pop)

typedef struct dot11_management_tag Dot11Tag;
typedef struct beacon_frame BeaconFrame;

BeaconFrame* newBeaconFrame(const char* ssid, uint8_t ssidLen, size_t* beaconLen);

typedef struct
{
    BeaconFrame*  beacon;
    size_t     beaconLen;
}
BeaconInfo;

void makeBeacons(const char* filename, std::vector<BeaconInfo>& beacons);
void deleteBeacons(std::vector<BeaconInfo>& beacons);

#endif