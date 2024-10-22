#ifndef EM919XMANAGEMENTCLASSHELPER_HPP
#define EM919XMANAGEMENTCLASSHELPER_HPP

#include <netinet/ip.h>
#include <netinet/in.h>
#include <net/if.h>

#include "lite-mbim/MbimTransport.h"
#include "lite-mbim/BasicConnectDeviceService.h"
#include "lite-mbim/MbimSyncObject.h"
#include "common/qmerrno.h"
#include "common/netlink_util.h"
#include "common/dev_util.h"
#include "common/proc_util.h"
#include "common/str_util.h"
#include "common/ping.h"

/* DEFINE SECTION */
#ifndef ARRAYSIZE
#define ARRAYSIZE(a)                (sizeof(a)/sizeof(a[0]))
#endif

#ifndef MIN
#define  MIN(x, y) ( ((x) < (y)) ? (x) : (y) )
#endif

#define SUCCESS 0
#define FAILURE -1

#ifndef UNUSEDPARAM
#define UNUSEDPARAM(x) (void)x
#endif

#define PING_COUNT    4
#define PING_DELAY    3

#define MAX_SESSIONS 4			// Arbitrary. May be changed by developer.
#define MAX_IP_ADDRESSES 4		// Arbitrary. May be changed by developer.
#define MAX_DNS_SERVERS 4		// Arbitrary. May be changed by developer.
#define MAX_ROUTES 4			// Arbitrary. May be changed by developer.
#define MAX_PING_DESTINATIONS 4	// Arbitrary. May be changed by developer.

typedef struct {
	uint32_t sessionId = 0;
	char szUserName[32] {0};
	char szPassword[32] {0};
	char szAccessString[32] {0};
	char szCompressionType[32] {0};
	char szAuthProtocolType[32] {0};
	char szIpType[32] {0};
	char szUseVlan[32] {0};
	char szVlanName[32] {0};
	uint16_t vlanId = 0;
} ConfigFileSettings;

typedef struct {
	char szInterface[IF_NAMESIZE];
	char szDst[INET_ADDRSTRLEN];
	uint32_t dstPrefixLength;    // This is the number of leftmost bits that make up the network mask.
	char szGateway[INET_ADDRSTRLEN];
} IPv4RouteTableEntry;

typedef struct
{
	char szInterface[IF_NAMESIZE];
	char szDst[INET6_ADDRSTRLEN];
	uint32_t dstPrefixLength;	// This is the number of leftmost bits that make up the network mask.
	char szGateway[INET6_ADDRSTRLEN];
} IPv6RouteTableEntry;

typedef struct
{
	wchar_t accessString[MBIM_ACCESS_STRING_MAX_LEN + 1];
	wchar_t userName[MBIM_USER_NAME_MAX_LEN + 1];
	wchar_t password[MBIM_PASSWORD_MAX_LEN + 1];
	MBIM_COMPRESSION compression;
	MBIM_AUTH_PROTOCOL authProtocol;
	MBIM_CONTEXT_IP_TYPE ipType;
	uint8_t contextType[MBIM_UUID_SIZE];
} Profile;

typedef struct
{
	// Profile info used for connect request
	uint32_t sessionId;
	Profile profile;

	// Connect response/indication
	MBIM_ACTIVATION_STATE activationState;
	MBIM_VOICE_CALL_STATE voiceCallState;
	MBIM_CONTEXT_IP_TYPE ipType;
	uint8_t contextType[MBIM_UUID_SIZE];
	uint32_t nwError;

	// IP configuration response/indication
	uint32_t iPv4ConfigurationAvailable;
	uint32_t iPv6ConfigurationAvailable;

	// Note: MBIM can return multiple IPv4addresses.
	// For the purposes of this program, we'll only use the first array element.
	MBIM_IPV4_ELEMENT iPv4Addresses[MAX_IP_ADDRESSES];
	uint32_t iPv4AddressCount;

	// Note: MBIM can return multiple IPv6addresses.
	// For the purposes of this program, we'll only use the first array element.
	MBIM_IPV6_ELEMENT iPv6Addresses[MAX_IP_ADDRESSES];
	uint32_t iPv6AddressCount;

	MBIM_IPV4_ADDRESS iPv4Gateway;
	MBIM_IPV6_ADDRESS iPv6Gateway;
	MBIM_IPV4_ADDRESS iPv4DnsServers[MAX_DNS_SERVERS];
	uint32_t iPv4DnsServerCount;
	MBIM_IPV6_ADDRESS iPv6DnsServers[MAX_DNS_SERVERS];
	uint32_t iPv6DnsServerCount;
	uint32_t iPv4Mtu;
	uint32_t iPv6Mtu;

	// Routing tables.
	uint32_t iPv4RouteCount;
	IPv4RouteTableEntry iPv4Routes[MAX_ROUTES];
	uint32_t iPv6RouteCount;
	IPv6RouteTableEntry iPv6Routes[MAX_ROUTES];

	// Ping destinations. Should align with routing table entries.
	uint32_t iPv4PingDestinationCount;
	char iPv4PingDestinations[MAX_PING_DESTINATIONS][INET_ADDRSTRLEN];
	uint32_t iPv6PingDestinationCount;
	char iPv6PingDestinations[MAX_PING_DESTINATIONS][INET6_ADDRSTRLEN];

	// VLAN.
	bool useVlan;	// true if this session is through a VLAN interface.
	char vlanName[IF_NAMESIZE];	// VLAN associated with this session.
	uint16_t vlanId;	// ID of VLAN.
} Session;

#endif //EM919XMANAGEMENTCLASSHELPER_HPP
