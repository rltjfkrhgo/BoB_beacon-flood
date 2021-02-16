// BeaconFlood.cpp
// 정보통신기술용어해설
// http://www.ktword.co.kr/abbr_view.php?m_temp1=2319
// mdk3 참고

#include "BeaconFlood.h"

// beaconLen: 만들어진 beacon의 길이
BeaconFrame* newBeaconFrame(const char* ssid, uint8_t ssidLen, size_t* beaconLen)
{
    size_t ssidTagLen = sizeof(Dot11Tag) + ssidLen;  // ssid tag
    size_t rateTagLen = sizeof(Dot11Tag) + 4;        // supported rates tag
    size_t   dsTagLen = sizeof(Dot11Tag) + 1;        // DS parameter set
    size_t   cfTagLen = sizeof(Dot11Tag) + 6;        // CF parameter set
    size_t  timTagLen = sizeof(Dot11Tag) + 4;        // tim tag
    // 총 beacon frame 길이
    *beaconLen = sizeof(BeaconFrame) + 
                 ssidTagLen + rateTagLen + dsTagLen + cfTagLen + timTagLen;

    BeaconFrame* beacon = (BeaconFrame*)new char[*beaconLen];
    memset(beacon, 0, *beaconLen);

    beacon->radiotap[2] = 0x0c;   // radiotap length
    beacon->radiotap[4] = 0x04;
    beacon->radiotap[5] = 0x80;
    beacon->radiotap[8] = 0x02;
    beacon->radiotap[10] = 0x18;

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

    Dot11Tag* ssidTag = (Dot11Tag*)beacon->tag();
    ssidTag->id = Dot11Tag::NUM_BSSID;
    ssidTag->len = ssidLen;
    memcpy(ssidTag->data(), ssid, ssidLen);

    // ssid data 시작 지점부터 ssid 길이 만큼 건너 뛴 곳이
    // rate tag 시작 주소가 된다.
    Dot11Tag* rateTag = (Dot11Tag*)( ssidTag->data() + ssidTag->len );
    rateTag->id = Dot11Tag::NUM_SUPPORTED_RATES;
    rateTag->len = 0x04;
    *(uint32_t*)(rateTag->data()) = 0x968b8482;

    // 채널
    Dot11Tag* dsTag = (Dot11Tag*)( rateTag->data() + rateTag->len );
    dsTag->id = Dot11Tag::NUM_DS_PARAMETER_SET;
    dsTag->len = 0x01;
    std::uniform_int_distribution<int> dis2(1, 11);
    *(dsTag->data()) = dis2(gen);

    Dot11Tag* cfTag = (Dot11Tag*)( dsTag->data() + dsTag->len );
    cfTag->id = Dot11Tag::NUM_CF_PARAMETER_SET;
    cfTag->len = 0x06;
    cfTag->data()[0] = 0x01;
    cfTag->data()[1] = 0x02;

    Dot11Tag* timTag = (Dot11Tag*)( cfTag->data() + cfTag->len );
    timTag->id = Dot11Tag::NUM_TIM;
    timTag->len = 0x04;
    timTag->data()[1] = 0x01;

    return beacon;
}

void makeBeacons(const char* filename, std::vector<BeaconInfo>& list)
{
    std::ifstream  fin(filename);

    while (!fin.eof())
    {
		std::string  ssid;
		fin >> ssid;
        
        if(ssid.length() < 1)
            continue;
        
        BeaconInfo info;
        info.beacon = newBeaconFrame(ssid.c_str(), ssid.length(), &info.beaconLen);
        list.push_back(info);
	}

    fin.close();
}

void deleteBeacons(std::vector<BeaconInfo>& beacons)
{
    for(auto it = beacons.begin(); it != beacons.end(); it++)
    {
        delete it->beacon;
        it->beacon = nullptr;
    }
}
