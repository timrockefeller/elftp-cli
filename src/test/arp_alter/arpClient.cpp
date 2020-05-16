#include <stdafx.h>
#include"iostream"
#include "iomanip"
#define HAVE_REMOTE
#include <pcap.h>
#include <packet32.h>
#include <ntddndis.h>
#include <stdlib.h>
#include "memory"
 
#pragma pack(1)  //memory alignment
#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"Packet.lib")
#pragma comment(lib,"ws2_32.lib")
 
using std::cout;
using std::endl;
using std::cin;
using std::setfill;
using std::setw;
 
//
struct _ARP_HEAD{
	u_short hardware_type; //硬件类型  0x0001
	u_short protocal_type; //协议类型  0x0800
	u_char hardware_addr_len; //硬件地址长度  06
	u_char protocal_addr_len; //协议地址长度  04
	USHORT operation_field; //操作字段 01 request ,  02 response
	UCHAR source_mac_addr[6]; //源mac地址 will be filled in runtime
	ULONG source_ip_addr; //源ip地址 localhost
	UCHAR dest_mac_addr[6]; //目的max地址 00:00:00:00:00:00
	ULONG dest_ip_addr; //目的ip地址 
};
 
//
struct _ETHER_HEAD{
	UCHAR dest_mac_addr[6];  //目的 mac 地址
	UCHAR source_mac_addr[6]; //源 mac 地址
	USHORT type;  //帧类型
};
 
//
struct _ARP{
	_ETHER_HEAD eh;
	_ARP_HEAD ah;
	char padding[18]; // to make sure the sizeof(BYTES) >= 60 
};
 
// to make an arp protocal
void MakeArp(_ARP * _arp, unsigned char* source_mac_addr, unsigned long source_ip_addr, unsigned long dest_ip_addr);
void MakeArp(_ARP * _arp, unsigned char* source_mac_addr, unsigned char* dest_mac_addr, unsigned long source_ip_addr, unsigned long dest_ip_addr, USHORT flag);
// to get devices' name and do more things in this function
void GetDevName(ULONG,ULONG,PUCHAR,USHORT);
// to get the local mac addr
void GetLocalMac(UCHAR * _u_char, PCHAR _adapter);
// to parse the mac addr
PUCHAR ParseMac(char * mac);
 
int main(int argc,  char* argv[])
{
	if (argc<1)
	{
		return 0;
	}
	PUCHAR mac_addr = ParseMac(argv[3]);
	GetDevName(inet_addr(argv[1]),inet_addr(argv[2]),mac_addr,atoi(argv[4]));
	free(mac_addr);
	system("pause");
	return 0;
}
 
 
//to parse the mac addr
PUCHAR ParseMac(char * mac)
{
	int i = strlen(mac);
	PUCHAR TMP = (PUCHAR)malloc(i / 2);
	char tmp[2] = { 0 };
	_strlwr(mac);
	int count = 0;
	int Counttmp = 0;
	int changemod = 0;
	while (count < i)
	{
		if (mac[count] >= 'a'&& mac[count] <= 'f')
			tmp[changemod] = mac[count] - 0x60 + 9;
		else	if (mac[count] >= '0' && mac[count] <= '9')
		{
			tmp[changemod] = mac[count] - 0x30;
		}
		else break;
		++changemod;
		++count;
		if (2 == changemod)
		{
			changemod = 0;
			TMP[Counttmp] = tmp[0] * 16 + tmp[1];
			++Counttmp;
		}
	}
	return TMP;
}
 
 
 
/**
* Print the device info
* 
*/
void GetDevName(ULONG source_ip, ULONG dest_ip, PUCHAR dest_mac, USHORT flag)
{
	pcap_if_t * alldevs;
	void packet_backFUNC(u_char *param, const struct pcap_pkthdr *header, const u_char * pkt_data);
	int count = 0;
	char source[PCAP_BUF_SIZE + 1] = { "rcap://\0" };
	char error[PCAP_BUF_SIZE + 1] = { 0 };
	
 
	//print all the device we had found
	if (pcap_findalldevs_ex(source,NULL,&alldevs,error) == -1)
	{
		cout << " Can't find the device ! \n";
		return;
	}
	else
	{
 
		 // this part is to display the info of NIC ,now we don't need it 
		 // 由于题目没有要求查看设备信息，所以注释掉这一部分，如果需要查看设备信息，可以打开这部分注释
		for (auto d = alldevs; d; d = d->next)
		{
			++count;
			/*cout << "Number : " << count << endl
				<< "Name :" << d->name << endl
				<< "Description : " << d->description << endl;*/
			
			//for (auto tmp = d->addresses; tmp;tmp=tmp->next)
			//{
			//	/*cout << "Address Family : #" << (int)tmp->addr->sa_family << endl << endl;*/
			//	if (tmp->addr->sa_family == AF_INET)
			//	{
			//		/*	cout << "Address Family Name : AF_INET" << endl
			//				<< "IP addr : " << inet_ntoa(((struct sockaddr_in*)tmp->addr)->sin_addr) << endl
			//				<< "Net mask : " << inet_ntoa(((struct sockaddr_in*)tmp->netmask)->sin_addr) << endl
			//				<< "BroadAddr : " << inet_ntoa(((struct sockaddr_in*)tmp->broadaddr)->sin_addr) << endl;*/
			//	}
			//	else{
			//		/*cout << "Address Family Name : Unknown\n";*/
			//	}
			//}
			/*cout.fill('-'); cout.width(79);
			cout << endl << "" << endl << endl;*/
		}
	}
	if (count == 0)
	{
		cout << "No Interface has found ,make sure the WinPcap has installed \n";
	}
 
 
	// to choose an interface 
	
	int ntmp_ = 1;
	if (ntmp_ <1  || ntmp_ >count)
	{
		cout << "The number out of range \n";
		pcap_freealldevs(alldevs);
		return;
	}
 
	//jump to the choice
	pcap_if_t *tmp_ = alldevs;
	for (int nNumber = 1; nNumber < ntmp_; nNumber++, tmp_ = tmp_->next);
 
	//open the choice
	auto pcap = pcap_open(
		tmp_->name,
		65536,
		PCAP_OPENFLAG_PROMISCUOUS,    //以混杂模式打开网卡
		1000,
		NULL,
		error);
 
	if (pcap == NULL)
	{
		cout << "Error has occured at open the device\n FileName : " << __FILE__ << "\nLine : " << __LINE__ << endl;
		pcap_freealldevs(alldevs);
		return;
	}
 
	
	// get an input of dest IP
 
 
	// to get the mac addr
	UCHAR macbuff[10] = { 0 };
	// "&(tmp_->name)[8]" is the beginning of a device's name with out "rpcap://" so that we can use PacketOpenAdapter successfully
	GetLocalMac(macbuff, &(tmp_->name)[8]);  
 
 
	//create an arp
	_ARP arp_;
	MakeArp(&arp_,
		macbuff,
		dest_mac,
		source_ip,
		dest_ip,
		flag
		);
 
 
	//begin to send 
	pcap_sendpacket(pcap, (UCHAR*)&arp_, sizeof(arp_));
 
	
 
 
	//free the devices list
	pcap_freealldevs(alldevs);
	pcap_close(pcap);
	cout << "ARP has been sent correctly !\n";
}
 
 
 
 
// #define UNDO
void packet_backFUNC(u_char *param, const struct pcap_pkthdr *header, const u_char * pkt_data)
{
	// TODO
}
 
 
void MakeArp(_ARP * _arp, unsigned char* source_mac_addr, unsigned long source_ip_addr, unsigned long dest_ip_addr)
{
	// set the ethernet info 
	memset(_arp->eh.dest_mac_addr, 0xff, 6);
	memcpy(_arp->eh.source_mac_addr, source_mac_addr, 6);
	_arp->eh.type = htons(0x0806);
 
	// set the arphead info
	_arp->ah.hardware_addr_len = 6;
	_arp->ah.hardware_type = htons(0x0001);
	_arp->ah.operation_field = htons(1);
	_arp->ah.protocal_addr_len = 4;
	_arp->ah.protocal_type = htons(0x0800);
	_arp->ah.source_ip_addr = source_ip_addr;
	memcpy(_arp->ah.source_mac_addr, source_mac_addr, 6);
	_arp->ah.dest_ip_addr = dest_ip_addr;
	memset(_arp->ah.dest_mac_addr, 0, 6);
 
	//Zero the padding
	ZeroMemory(_arp->padding, 18);
}
 
void MakeArp(_ARP * _arp, unsigned char* source_mac_addr,unsigned char* dest_mac_addr, unsigned long source_ip_addr, unsigned long dest_ip_addr, USHORT flag)
{
	if (flag == 0)
	{
		MakeArp(_arp, source_mac_addr, source_ip_addr, dest_ip_addr);
		return;
	}
	else
	{
		MakeArp(_arp, source_mac_addr, source_ip_addr, dest_ip_addr);
		memcpy(_arp->eh.dest_mac_addr, dest_mac_addr, 6);
		memcpy(_arp->eh.source_mac_addr, source_mac_addr, 6);
		memcpy(_arp->ah.source_mac_addr, source_mac_addr, 6);
		memcpy(_arp->ah.dest_mac_addr, dest_mac_addr, 6);
		_arp->ah.operation_field = htons(2);
		return;
	}
}
 
 
// to get the local mac
#define ADAPTER_NAME_SIZE 512
void GetLocalMac(UCHAR * _u_char,PCHAR _adapter)
{	
	LPADAPTER lpadapter;
	lpadapter = PacketOpenAdapter(_adapter);
 
	if (!lpadapter || (INVALID_HANDLE_VALUE == lpadapter->hFile))
	{
		cout << "Unable to open the adapter,Error Code：" << GetLastError();
		return;
	}
 
	PPACKET_OID_DATA poid_data_;
	poid_data_ = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	poid_data_->Oid = OID_802_3_CURRENT_ADDRESS;
	poid_data_->Length = 6;
	ZeroMemory(poid_data_->Data, 6);
	auto status  = PacketRequest(lpadapter, FALSE, poid_data_);
	if (status)
	{
		memcpy(_u_char, poid_data_->Data, 6);
	}
	else
	{
		cout << "An error has occured at file : " << __FILE__ << " line : " << __LINE__<<endl;
	}
	free(poid_data_);
	PacketCloseAdapter(lpadapter);
}
————————————————
版权声明：本文为CSDN博主「登陆_注册_帮助」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qqqqqqqqqq1qqqqqqqqq/java/article/details/47341607