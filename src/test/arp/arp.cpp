#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <tchar.h>
#include <iostream>
#include <WinSock2.h>
#define HAVE_REMOTE
#include <pcap.h>
#include <process.h>
#include <remote-ext.h>
#include <stdio.h>
#include <windows.h>

#include <string>
#pragma pack(1)  // Memory Alignment

using namespace std;

#define ETH_IP 0X0800
#define ETH_ARP 0X0806
#define ARP_REQUEST 0X0001
#define ARP_REPLY 0X0002
#define ARP_HARDWARE 0X0001
#define max_num_adapter 10

// Arp Frame Structure
struct arp_head {
    unsigned short hardware_type;     //Hardware Type
    unsigned short protocol_type;     //Protocal Type
    unsigned char hardware_add_len;   //Hardware Address Length
    unsigned char protocol_add_len;   //Protocal Address Length
    unsigned short operation_field;   //Operation Field
    unsigned char source_mac_add[6];  //Source MAC Address
    unsigned long source_ip_add;      //Source IP Address
    unsigned char dest_mac_add[6];    //Target MAC Address
    unsigned long dest_ip_add;        //Target IP Address
};

//Etheret Frame
struct ethernet_head {
    unsigned char dest_mac_add[6];
    unsigned char source_mac_add[6];
    unsigned short type;  //Frame Type
};

//Arp Data Package
struct arp_packet {
    ethernet_head eh;
    arp_head ah;
};

struct pc {
    unsigned long ip;
    unsigned char mac[6];
} pcGroup[10000];

u_char selfMac[6] = {0};
u_long myip;
pcap_t* adhandle;
u_long firstip, secondip;
unsigned int HostNum = 0;
int flag = FALSE;
HANDLE mThread;

//Alarming
void warmMessage() {
    std::cout << "You must install WinPcap if you want to launch this program." << std::endl;
    return;
}

//Open Net Driver
int OpenIF() {
    int j = 0;
    int inum = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs;
    pcap_if_t* d;

    /*Get Net Driver List*/
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
        cout << "Failed to get Net Driver." << endl;
        exit(1);
    }

    /*Print Net Field Message*/
    for (d = alldevs; d != NULL; d = d->next) {
        cout << ++j << "      " << d->name << endl;
        if (d->description)
            cout << d->description << endl;
        else
            cout << "No discription for this device." << endl;
        cout << "\n\n"
             << endl;
    }

    if (j == 0) {
        cout << "Cannot Find this Net Driver Device." << endl;
        return -1;
    }

    cout << "Please choose Net Driver Device:"
         << "1-" << j << endl;
    cin >> inum;
    if (inum < 1 || inum > j) {
        cout << "Wrong Option, No Net Driver Device for this option." << endl;
        pcap_freealldevs(alldevs);
        return -1;
    }

    for (d = alldevs, j = 1; j != inum; j++, d = d->next) {
    }

    /*Open Device*/
    if ((adhandle = pcap_open(d->name, 1000, PCAP_OPENFLAG_PROMISCUOUS, 100, NULL, errbuf)) == NULL) {
        cout << "Cannot open device." << endl;
        pcap_freealldevs(alldevs);
        return -1;
    } else if (pcap_datalink(adhandle) != DLT_EN10MB) {
        cout << "Not Ethernet, cannot be used" << endl;
        pcap_freealldevs(alldevs);
        return -1;
    }

    return 1;
}
/*Get the IP address and MAC address of your own host*/
int GetSelfMac() {
    struct pcap_pkthdr* pkt_header;
    const u_char* pkt_data;
    unsigned char sendbuf[42] = {0};  // Send buffer, also the size of arp packet
    int i = -1;
    int res;
    ethernet_head eh;
    arp_head ah;

    memset(eh.dest_mac_add, 0xff, 6);
    memset(eh.source_mac_add, 0x0f, 6);
    memset(ah.source_mac_add, 0x0f, 6);
    memset(ah.source_mac_add, 0x00, 6);

    eh.type = htons(ETH_ARP);
    ah.hardware_type = htons(ARP_HARDWARE);
    ah.protocol_type = htons(ETH_IP);
    ah.hardware_add_len = 6;
    ah.protocol_add_len = 4;
    ah.source_ip_add = inet_addr("222.220.23.1");  //Source ip address bit arbitrary ip address
    ah.operation_field = htons(ARP_REQUEST);
    ah.dest_ip_add = inet_addr("192.168.1.2");

    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &eh, sizeof(eh));
    memcpy(sendbuf + sizeof(eh), &ah, 14);
    memcpy(sendbuf + sizeof(eh) + 14, &ah.source_ip_add, 10);
    memcpy(sendbuf + sizeof(eh) + 24, &ah.dest_ip_add, 4);

    if (pcap_sendpacket(adhandle, sendbuf, 42) == 0)
        cout << "Successfully sent arp packet" << endl;
    else
        cout << "Failed to send arp packet" << GetLastError() << endl;

    //Get a reply
    while ((res = pcap_next_ex(adhandle, &pkt_header, &pkt_data)) > 0) {
        if (*(unsigned short*)(pkt_data + 12) == htons(ETH_ARP) &&
            *(unsigned short*)(pkt_data + 20) == htons(ARP_REPLY) &&
            *(unsigned long*)(pkt_data + 38) == inet_addr("222.220.23.1")) {
            cout << "Local network card physical address:";
            for (i = 0; i < 5; i++) {
                selfMac[i] = *(unsigned char*)(pkt_data + 22 + i);
                cout << selfMac[i];
            }

            selfMac[i] = *(unsigned char*)(pkt_data + 22 + i);
            cout << selfMac[i] << endl;
            myip = *(unsigned long*)(pkt_data + 28);
            break;
        }
    }

    if (res == 0)
        cout << "time out! Receive network packet timeout" << endl;

    if (res == -1)
        cout << "Error reading network packet" << endl;

    if (i == 6)
        return 1;
    else
        return 0;
}

//Send arp request
unsigned int _stdcall sendArpPacket(void* arglist)
{
	unsigned char sendbuf[42];
	unsigned long ip;
	const char iptosendh[20] = {0};
	ethernet_head eh;
	arp_head ah;

	memset(eh.dest_mac_add, 0xff, 6);
	memcpy(eh.source_mac_add, selfMac, 6);
	memcpy(ah.source_mac_add, selfMac, 6);
	memset(ah.source_mac_add, 0x00, 6);

	eh.type = htons(ETH_ARP);
	ah.hardware_type = htons(ARP_HARDWARE);
	ah.protocol_type = htons(ETH_IP);
	ah.hardware_add_len = 6;
	ah.protocol_add_len = 4;
	ah.operation_field = htons(ARP_REQUEST);
	ah.source_ip_add = myip;

	for (unsigned long i = 0; i < HostNum; i++)
	{
		for (unsigned long j = 0; j < 1; j++)
		{
			ip = firstip;
			ah.dest_ip_add = htonl(htonl(ip) + i);
			memset(sendbuf, 0, sizeof(sendbuf));
			memcpy(sendbuf, &eh, sizeof(eh));
			memcpy(sendbuf + sizeof(eh), &ah, 14);
			memcpy(sendbuf + sizeof(eh) + 14, &ah.source_ip_add, 10);
			memcpy(sendbuf + sizeof(eh) + 24, &ah.dest_ip_add, 4);

			if (pcap_sendpacket(adhandle, sendbuf, 42) == 0)
			{
				//cout << "Send pack sucess." << endl;
			}
			else
				cout << "Send pack failed. " << GetLastError() << endl;
		}
	}

	Sleep(1000);
	flag = TRUE;
	return 1;
}

//Get ARP related thread
unsigned int _stdcall GetlivePc(void* arglist) {
    int res;
    int aliveNum = 0;

    struct pcap_pkthdr* pkt_header;
    const u_char* pkt_data;
    unsigned char tempMac[6];

    while (TRUE) {
        if (flag) {
            cout << "Scanning finished, exit listening process." << endl;
            break;
        }

        if ((res = pcap_next_ex(adhandle, &pkt_header, &pkt_data)) > 0) {
            if (*(unsigned short*)(pkt_data + 12) == htons(ETH_ARP)) {
                arp_packet* recv = (arp_packet*)pkt_data;

                recv->ah.source_ip_add = *(unsigned long*)(pkt_data + 28);
                if (*(unsigned short*)(pkt_data + 20) == htons(ARP_REPLY)) {
                    cout << "Catched ARP Packs: ";
                    cout << "IP address: " << (unsigned long)(recv->ah.source_ip_add & 255)
                         << "." << (unsigned long)((recv->ah.source_ip_add >> 8) & 255)
                         << "." << (unsigned long)((recv->ah.source_ip_add >> 16) & 255)
                         << "." << (unsigned long)((recv->ah.source_ip_add >> 24) & 255)
                         << "         ";
                    pcGroup[aliveNum].ip = *(unsigned long*)(pkt_data + 28);
                    memcpy(pcGroup[aliveNum].mac, (pkt_data + 22), 6);
                    aliveNum++;

                    cout << "MAC address: ";
                    for (int i = 0; i < 6; i++) {
                        tempMac[i] = *((unsigned char*)(pkt_data + 22 + i));
                        printf("%x-", tempMac[i]);
                    }
                    cout << "" << endl;
                }
            }
        }
    }

    for (int j = 0; j < 255; j++) {
        if (pcGroup[j].ip != 0) {
            cout << "IP address: " << (pcGroup[j].ip & 255) << "."
                 << ((pcGroup[j].ip >> 8) & 255) << "."
                 << ((pcGroup[j].ip >> 16) & 255) << "."
                 << ((pcGroup[j].ip >> 24) & 255) << "         ";
            printf("MAC address: %2x - %2x - %2x - %2x - %2x - %2x\n", pcGroup[j].mac[0], pcGroup[j].mac[1], pcGroup[j].mac[2], pcGroup[j].mac[3], pcGroup[j].mac[4], pcGroup[j].mac[5]);
        }
    }

    ResumeThread(mThread);
    return 1;
}
int main() {
    std::cout.setf(std::ios::left);
    warmMessage();
    HANDLE hThread1, hThread2;
    string fip;
    string sip;
    cout << "Please enter the first IP:" << endl;
    cin >> fip;
    cout << "Enter the 1st IP:" << fip << endl;

    cout << "Please enter the second IP:" << endl;
    cin >> sip;
    cout << "Enter the 2nd IP:" << sip << endl;

    cout << fip << endl;
    cout << sip << endl;

    firstip = inet_addr(fip.data());
    secondip = inet_addr(sip.data());
    cout << "First network address:" << firstip << endl;
    cout << "Second network address:" << secondip << endl;

    HostNum = htonl(secondip) - htonl(firstip) + 1;
    OpenIF();
    GetSelfMac();

    mThread = GetCurrentThread();
    hThread1 = (HANDLE)_beginthreadex(NULL, 0, sendArpPacket, NULL, 0, NULL);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0, GetlivePc, NULL, 0, NULL);
    SuspendThread(mThread);
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(mThread);
    return 0;
}
