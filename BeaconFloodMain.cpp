// BeaconFloodMain.cpp

#include <cstdio>
#include <pcap.h>
#include <unistd.h>  // usleep()
#include "BeaconFlood.h"

void usage() {
    printf("syntax: beacon-flood <interface> <ssid-list-file>\n");
    printf("sample: beacon-flood mon0 ssid-list.txt\n");
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        usage();
        return -1;
    }

    std::vector<BeaconInfo> beacons;
    makeBeacons(argv[2], beacons);

    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
        return -1;
    }

    while(true)
    {
        for(auto it = beacons.begin(); it != beacons.end(); it++)
        {
            int res = pcap_sendpacket(
                handle, reinterpret_cast<const u_char*>( it->beacon ), it->beaconLen);
            if (res != 0) {
                fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(handle));
            }
            else
            {
                printf("sending beacon frame...\n");
            }
        }
        usleep(100000);
    }

    pcap_close(handle);
    deleteBeacons(beacons);

    return 0;
}