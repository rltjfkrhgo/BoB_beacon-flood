// BeaconFloodMain.cpp

#include <cstdio>
#include <pcap.h>
#include <unistd.h>  // usleep()
#include <string>
#include "BeaconFlood.h"

void usage() {
    printf("syntax: beacon-flood <interface> <ssid-list-file>\n");
    printf("sample: beacon-flood mon0 ssid-list.txt\n");
}

int main(int argc, char* argv[])
{
    std::string str = "test";
    size_t beaconLen;
    BeaconFrame* beacon = newBeaconFrame(str.c_str(), str.length(), &beaconLen);

/*
    if (argc != 3) {
        usage();
        return -1;
    }
    */

    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
        return -1;
    }

    while(true)
    {
        int res = pcap_sendpacket(
            handle, reinterpret_cast<const u_char*>( beacon ), beaconLen);
        if (res != 0) {
            fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(handle));
        }
        else
        {
            printf("sending beacon frame...\n");
            updateBeaconFrame(beacon);
        }
        usleep(100000);
    }

    pcap_close(handle);

    return 0;
}