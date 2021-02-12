// BeaconFlood.cpp
// http://www.ktword.co.kr/abbr_view.php?m_temp1=2319

#include "BeaconFlood.h"

// beaconLen: 만들어진 beacon의 길이
BeaconFrame* newBeaconFrame(const char* ssid, u_int8_t ssidLen, size_t* beaconLen)
{
    size_t ssidTagLen = sizeof(Dot11Tag)-1 + ssidLen;  // ssid tag
    size_t rateTagLen = sizeof(Dot11Tag)-1 + 4;        // supported rates tag
    size_t   dsTagLen = sizeof(Dot11Tag)-1 + 1;        // DS parameter set
    size_t   cfTagLen = sizeof(Dot11Tag)-1 + 6;        // CF parameter set
    size_t  timTagLen = sizeof(Dot11Tag)-1 + 4;
    *beaconLen = sizeof(BeaconFrame)-1 + 
                ssidTagLen + rateTagLen + dsTagLen + cfTagLen + timTagLen;  // 총 beacon frame 길이

    BeaconFrame* beacon = (BeaconFrame*)new char[*beaconLen];
    memset(beacon, 0, *beaconLen);

    beacon->radiotab[2] = 0x0c;   // radiotap length
    beacon->radiotab[4] = 0x04;
    beacon->radiotab[5] = 0x80;
    beacon->radiotab[8] = 0x02;
    beacon->radiotab[10] = 0x18;


    beacon->type = BeaconFrame::TYPE;
    memset(beacon->receiver, 0xff, 6);  // broadcast

    // 랜덤 mac
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    beacon->transmitter[0] = dis(gen);
    beacon->transmitter[1] = dis(gen);
    beacon->transmitter[2] = dis(gen);
    beacon->transmitter[3] = dis(gen);
    beacon->transmitter[4] = dis(gen);
    beacon->transmitter[5] = dis(gen);
    memcpy(beacon->bssid, beacon->transmitter, 6);
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
    rateTag->len = 0x04;
    *(u_int32_t*)&(rateTag->data) = 0x968b8482;

    Dot11Tag* dsTag = (Dot11Tag*)( &(rateTag->data) + rateTag->len );
    dsTag->num = Dot11Tag::NUM_DS_PARAMETER_SET;
    dsTag->len = 0x01;
    dsTag->data = 0x05;

    Dot11Tag* cfTag = (Dot11Tag*)( &(dsTag->data) + dsTag->len );
    cfTag->num = Dot11Tag::NUM_CF_PARAMETER_SET;
    cfTag->len = 0x06;
    (&(cfTag->data))[0] = 0x01;
    (&(cfTag->data))[1] = 0x02;

    Dot11Tag* timTag = (Dot11Tag*)( &(cfTag->data) + cfTag->len );
    timTag->num = Dot11Tag::NUM_TIM;
    timTag->len = 0x04;
    (&(timTag->data))[1] = 0x01;

    return beacon;
}

void deleteBeaconFrame(BeaconFrame* beacon)
{
    delete[] beacon;
}