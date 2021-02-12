// BeaconFlood.cpp
// http://www.ktword.co.kr/abbr_view.php?m_temp1=2319

#include "BeaconFlood.h"

// beaconLen: 만들어진 beacon의 길이
BeaconFrame* newBeaconFrame(const char* ssid, u_int8_t ssidLen, size_t* beaconLen)
{
    size_t ssidTagLen = sizeof(Dot11Tag)-1 + ssidLen;  // ssid tag
    size_t rateTagLen = sizeof(Dot11Tag)-1 + 8;        // supported rates tag
    size_t   dsTagLen = sizeof(Dot11Tag)-1 + 1;        // DS parameter set
    *beaconLen = sizeof(BeaconFrame)-1 + ssidTagLen + rateTagLen + dsTagLen;  // 총 beacon frame 길이

    BeaconFrame* beacon = (BeaconFrame*)new char[*beaconLen];
    memset(beacon, 0, *beaconLen);

    beacon->radiotab[2] = 0x18;   // radiotap length
    beacon->radiotab[4] = 0x2e;
    beacon->radiotab[5] = 0x40;
    beacon->radiotab[7] = 0xa0;
    beacon->radiotab[8] = 0x20;
    beacon->radiotab[9] = 0x08;
    beacon->radiotab[13] = 0x02;
    beacon->radiotab[14] = 0x80;
    beacon->radiotab[15] = 0x09;
    beacon->radiotab[16] = 0xa0;
    beacon->radiotab[18] = 0xcd;
    beacon->radiotab[22] = 0xcd;


    beacon->type = BeaconFrame::TYPE;
    memset(beacon->receiver, 0xff, 6);  // broadcast
    beacon->transmitter[0] = 0x11;
    beacon->transmitter[1] = 0x22;
    beacon->transmitter[2] = 0x33;
    beacon->transmitter[3] = 0x44;
    beacon->transmitter[4] = 0x55;
    beacon->transmitter[5] = 0x66;
    memcpy(beacon->bssid, beacon->transmitter, 6);
    beacon->timestamp = clock();
    beacon->interval = 100;       // ms 단위
    beacon->capabilities = 0x01;  // ESS 즉 AP

    Dot11Tag* ssidTag = (Dot11Tag*)&(beacon->tag);
    ssidTag->num = Dot11Tag::NUM_BSSID;
    ssidTag->len = ssidLen;
    memcpy(&(ssidTag->data), ssid, ssidLen);

    // ssid data 시작 지점부터 ssid 길이 만큼 건너 뛴 곳이
    // rate tag 시작 주소가 된다.
    Dot11Tag* rateTag = (Dot11Tag*)( &(ssidTag->data) + ssidTag->len );
    rateTag->num = Dot11Tag::NUM_SUPPORTED_RATES;
    rateTag->len = 0x08;
    *(u_int64_t*)&(rateTag->data) = 0x6c482412968b8482;

    Dot11Tag* dsTag = (Dot11Tag*)( &(rateTag->data) + rateTag->len );
    dsTag->num = Dot11Tag::NUM_DS_PARAMETER_SET;
    dsTag->len = 0x01;
    dsTag->data = 0x01;

    return beacon;
}

// timestamp랑 sequence 업데이트
void updateBeaconFrame(BeaconFrame* beacon)
{
    beacon->timestamp = clock();
    beacon->sequence++;
}

void deleteBeaconFrame(BeaconFrame* beacon)
{
    delete[] beacon;
}