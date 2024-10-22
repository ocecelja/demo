/**
 * \ingroup litembim
 *
 * \file BasicConnectDeviceService.h
 * 
 * \note All the APIs and definitions are based on MBIM specification values.
 *       Please use "MBIM v1 0 - errata-1.pdf" as a companion reference.
 * 
 */

#ifndef __BASIC_CONNECT_DEVICE_SERVICE_H__
#define __BASIC_CONNECT_DEVICE_SERVICE_H__

#include <stdint.h>
#include <wchar.h>
#include "MbimTransport.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the UUID for the MBIM Basic Connect device service.
 *       
 * @return UUID
*/
const uint8_t* BasicConnectDeviceService_Uuid();

/**
 *  This structure represents the command IDs (CIDs) of indications 
 *  related to a specific MBIM device service for which a user
 *  will be informed.
 *  
 *  \param  DeviceServiceId
 *          - The MBIM device service's UUID which relates to the indications.
 * 
 *  \param  CidCount
 *          - Number of CIDs for which indications are to be generated.
 *            0 means all CIDs of that MBIM device service.
 * 
 *  \param  Cids
 *          - Fixed size array of CIDs. The first 'CidCount' elements of the array
 *            will be passed to the device, upto a maximum of MBIM_MAX_CIDS.
 * 
 */
typedef struct MbimEventEntry
{
    uint8_t DeviceServiceId[MBIM_UUID_SIZE];
    uint32_t CidCount;
    uint32_t Cids[MBIM_MAX_CIDS];   
} MbimEventEntry;


/**
 * Build a MBIM_CID_DEVICE_SERVICE_SUBSCRIBE_LIST set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] elementCount Number of MbimEventEntry elements in the payload.
 * @param[in] elements Array of elements.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_DeviceServiceSubscribeSetBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, 
    uint32_t elementCount,
    MbimEventEntry* elements);

/**
 * CAUTION: DEPRECATED, here for backward compatability.
 * Please use combination of BasicConnectDeviceService_DeviceServiceSubscribeSetBuild() 
 * and MbimTransport_SendCommand() going forward.
 *
 * Send a MBIM_CID_DEVICE_SERVICE_SUBSCRIBE_LIST command to the device  service
 * and return immediately.
 * 
 * @param[in] pTransaction  Transaction object which will track the command/response.
 * @param[in] pTransport  MBIM transport object which will carry out the command.
 * @param[in] elementCount Number of MbimEventEntry elements in the command.
 * @param[in] pElements Array of elements.
 *       
 * @return 0 on success, < 0 on failure. 
 *
 * @note pTransaction must have been initialized with MbimTransaction_Initialize prior to this call.
 * 
 */
int BasicConnectDeviceService_DeviceServiceSubscribeSetSend(
    MbimTransaction* pTransaction,
    MbimTransport* pTransport,
    uint32_t elementCount,
    MbimEventEntry* pElements);

/**
 * MBIM_DATA_CLASS
 * A bitmap that represents which radio technologies are supported by the function. 
 * The following table shows the possible values for this member.
 */

/** None */
#define MBIMDataClassNone 0x0
/** GPRS */
#define MBIMDataClassGPRS 0x1
/** EDGE */
#define MBIMDataClassEDGE 0x2
/** UMTS */
#define MBIMDataClassUMTS 0x4
/** HSDPA */
#define MBIMDataClassHSDPA 0x8
/** HSUPA */
#define MBIMDataClassHSUPA 0x10
/** LTE */
#define MBIMDataClassLTE 0x20
/** Reserved for future GSM */
#define MBIMDataClassReservedForFutureGSM 0x8f40
/** 1XRTT */
#define MBIMDataClass1XRTT 0x10000
/** 1XEVDO */
#define MBIMDataClass1XEVDO 0x20000
/** 1XEVDORevA */
#define MBIMDataClass1XEVDORevA 0x40000
/** 1XEVDV */
#define MBIMDataClass1XEVDV 0x80000
/** 3XRTT */
#define MBIMDataClass3XRTT 0x100000
/** 1XEVDORevB */
#define MBIMDataClass1XEVDORevB 0x200000
/** UMB */
#define MBIMDataClassUMB 0x400000
/** Reserved for future CDMA */
#define MBIMDataClassReservedForFutureCDMA 0x4f800000
/** Custom */
#define MBIMDataClassCustom 0x80000000

const char* MBIMDataClassToString(uint32_t flag);

/**
 * MBIM_CELLULAR_CLASS
 * A bitmap of cellular technologies. 
 * The following table shows the possible values for this member.
 */

/** None */
#define MBIMCellularClassGsm 0x1
/** GPRS */
#define MBIMCellularClassCdma 0x2

const char* MBIMCellularClassToString(uint32_t flag);

/**
 * Parse a MBIM_CID_DEVICE_SERVICE_SUBSCRIBE_LIST response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[in,out] pElementCount If caller sets *pElementCount = 0, on return it will be set to number of elements
 *                              available, without returning any (pElements may be NULL).
 *                              If caller sets *pElementCount > 0, function will return the minimum of this value
 *                              and the number of elements available (pElements must not be NULL). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call.    
 * @param[out] pElements Array of elements to be written to by parsing response.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_DeviceServiceSubscribeParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    uint32_t* pElementCount,    // in = max number of elements which will be filled, out = number of elements filled by response
    MbimEventEntry* pElements);
    
typedef enum
{
    /** The SIM has not yet completed its initialization */    
    MBIMSubscriberReadyStateNotInitialized = 0,
    /** 
     * The SIM is initialized. When this state is
     * reported all the subscriber fields in Table
     * 10-18 of MBIM specification: MBIM_SUBSCRIBER_READY_INFO
     * must be valid. 
     */ 
    MBIMSubscriberReadyStateInitialized = 1, 
    /** The SIM card is not inserted into the device */
    MBIMSubscriberReadyStateSimNotInserted = 2, 
    /** The SIM card inserted into the device is invalid*/    
    MBIMSubscriberReadyStateBadSim = 3, 
    /** A general SIM failure has occurred */
    MBIMSubscriberReadyStateFailure = 4, 
    /** The subscription is not activated */
    MBIMSubscriberReadyStateNotActivated = 5, 
    /** The SIM is locked and requires PIN1 or PUK1 to unlock */    
    MBIMSubscriberReadyStateDeviceLocked = 6, 
} MBIM_SUBSCRIBER_READY_STATE;

const char* MBIMSubscriberReadyStateToString(MBIM_SUBSCRIBER_READY_STATE value);

typedef enum
{
    /** The device is in normal mode */
    MBIMReadyInfoFlagsNone = 0,
    /**
     * When this flag is specified, the host will
     * not display the SubscriberId specified in
     * the same CID.
     */ 
    MBIMReadyInfoFlagsProtectUniqueID = 1,
} MBIM_UNIQUE_ID_FLAGS;

const char* MBIMReadyInfoFlagsToString(MBIM_UNIQUE_ID_FLAGS value);

/** Maximum number of chars in a telephone number (excluding NULL terminator) */
#define MBIM_TELEPHONE_NUMBER_MAX_LEN 22

/**
 *  This structure represents a telephone number.
 *  
 *  \param  value
 *          - NULL-terminated telephone number.
 * 
 */
typedef struct
{
    wchar_t value[MBIM_TELEPHONE_NUMBER_MAX_LEN + 1];
} MbimTelephoneNumber;

/**
 * Build a MBIM_CID_SUBSCRIBER_READY_STATUS query payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_SubscriberReadyStatusQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType);

/*
 * Parse a MBIM_CID_SUBSCRIBER_READY_STATUS response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pReadyState Ready state will be written here.
 * @param[in,out] pSubscriberIdLen If caller sets *pSubscriberIdLen = 0, on return it will be set to number of wchar_t's
 *                              in subscriber Id (excluding NULL-terminator), without returning the pSubscriberId (pSubscriberId may be NULL). 
 *                              If caller sets *pSubscriberIdLen > 0, function will return the minimum of this value
 *                              and the number of wchar_t's available (pSubscriberId must not be NULL). Make sure
 *                              pSubscriberId points to buffer of size *pSubscriberIdLen+1 (for NULL-terminator which will be appended). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call. 
 * @param[out] pSubscriberId SubscriberId.
 * @param[in,out] pSimIccidLen If caller sets *pSimIccidLen = 0, on return it will be set to number of wchar_t's
 *                              in SIM ICCID (excluding NULL-terminator), without returning the pSimIccid (pSimIccid may be NULL). 
 *                              If caller sets *pSimIccidLen > 0, function will return the minimum of this value
 *                              and the number of wchar_t's available (pSimIccid must not be NULL). Make sure
 *                              pSimIccid points to buffer of size *pSimIccidLen+1 (for NULL-terminator which will be appended). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call. 
 * @param[out] pSimIccid SIM ICCID
 * @param[out] pReadyInfo Ready state info flags.
 * @param[in,out] pTelephoneNumberCount If caller sets *pTelephoneNumberCount = 0, on return it will be set to number of elements
 *                              available, without returning any (pTelephoneNumbers may be NULL).
 *                              If caller sets *pTelephoneNumberCount > 0, function will return the minimum of this value
 *                              and the number of elements available (pTelephoneNumbers must not be NULL). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call.    
 * @param[out] pTelephoneNumbers Array of elements to be written to by parsing response.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_SubscriberReadyStatusParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    MBIM_SUBSCRIBER_READY_STATE* pReadyState,
    uint32_t* pSubscriberIdLen,
    wchar_t* pSubscriberId,
    uint32_t* pSimIccIdLen,
    wchar_t* pSimIccId,
    MBIM_UNIQUE_ID_FLAGS* pReadyInfo,
    MbimTelephoneNumber* pTelephoneNumbers,
    uint32_t* pTelephoneNumberCount    // in = max number of elements which will be filled, out = number of elements filled by response
    );


typedef enum
{
    /** Attach */
    MBIMPacketServiceActionAttach = 0,
    /** Detach */
    MBIMPacketServiceActionDetach = 1,
} MBIM_PACKET_SERVICE_ACTION;

const char* MBIMPacketServiceActionToString(MBIM_PACKET_SERVICE_ACTION value);

typedef enum 
{
    /** Unknown */
    MBIMPacketServiceStateUnknown = 0,
    /** Attaching */
    MBIMPacketServiceStateAttaching = 1,
    /** Attached */
    MBIMPacketServiceStateAttached = 2,
    /** Detaching */
    MBIMPacketServiceStateDetaching = 3,
    /** Detached */
    MBIMPacketServiceStateDetached = 4,
} MBIM_PACKET_SERVICE_STATE;

const char* MBIMPacketServiceStateToString(MBIM_PACKET_SERVICE_STATE value);

/**
 * Build a MBIM_CID_PACKET_SERVICE set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] packetServiceAction Packet service action.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnect_DeviceService_PacketServiceSetBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    MBIM_PACKET_SERVICE_ACTION packetServiceAction);

/**
 * Build a MBIM_CID_PACKET_SERVICE query payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 *       
  * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
*/
uint32_t BasicConnectDeviceService_PacketServiceQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType);

/*
 * Parse a MBIM_CID_PACKET_SERVICE response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pNwError Network error.
 * @param[out] pPacketServiceState Packet service state.
 * @param[out] pHighestAvailableDataClass One of the MBIM_DATA_CLASS constants.
 * @param[out] pUplinkSpeed Uplink speed.
 * @param[out] pDownlinkSpeed Downlink speed. 
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_PacketServiceParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    uint32_t* pNwError,
    MBIM_PACKET_SERVICE_STATE* pPacketServiceState,
    uint32_t* pHighestAvailableDataClass,   //(MBIM_DATA_CLASS)
    uint64_t* pUplinkSpeed,
    uint64_t* pDownlinkSpeed);


typedef enum
{
    /** Automatic */
    MBIMRegisterActionAutomatic = 0,
    /** Manual */
    MBIMRegisterActionManual = 1,
} MBIM_REGISTER_ACTION;

const char* MBIMRegisterActionToString(MBIM_REGISTER_ACTION value);

typedef enum
{
    /** Unknown */
    MBIMRegisterStateUnknown = 0,
    /** Deregistered */
    MBIMRegisterStateDeregistered = 1,
    /** Searching */
    MBIMRegisterStateSearching = 2,
    /** Home */
    MBIMRegisterStateHome = 3,
    /** Roaming */
    MBIMRegisterStateRoaming = 4,
    /** Partner */
    MBIMRegisterStatePartner = 5,
    /** Denied */
    MBIMRegisterStateDenied = 6,
} MBIM_REGISTER_STATE;

const char* MBIMRegisterStateToString(MBIM_REGISTER_STATE value);

typedef enum
{
    /** Unknown */
    MBIMRegisterModeUnknown = 0,
    /** Automatic */
    MBIMRegisterModeAutomatic = 1,
    /** Manual */
    MBIMRegisterModeManual = 2,
} MBIM_REGISTER_MODE;

const char* MBIMRegisterModeToString(MBIM_REGISTER_MODE value);

/**
 * MBIM_REGISTRATION_FLAGS
 * A bitmap that indicates network conditions related to auto-attach and 
 * manual network selection capabilities.
 */

/** None */
#define MBIM_REGISTRATION_NONE 0x0
/** Manual selection not available */
#define MBIM_REGISTRATION_MANUAL_SELECTION_NOT_AVAILABLE 0x1
/** Packet service automatic attach */
#define MBIM_REGISTRATION_PACKET_SERVICE_AUTOMATIC_ATTACH 0x2

const char* MBIMRegistrationFlagToString(uint32_t flag);

/**
 * Build a MBIM_CID_REGISTER_STATE set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] providerId provider ID (NULL-terminated).
 * @param[in] registerAction Register action.
 * @param[in] dataClass One of MBIM_DATA_CLASS.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_RegisterStateSetBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    wchar_t* providerId,
    MBIM_REGISTER_ACTION registerAction,
    uint32_t dataClass);

/**
 * Build a MBIM_CID_REGISTER_STATE query payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_RegisterStateQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType);

/*
 * Parse a MBIM_CID_REGISTER_STATE response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pNwError Network error be written here.
 * @param[out] pRegisterState Register state.
 * @param[out] pRegisterMode Register mode.
 * @param[out] pAvailableDataClasses Bitmap of MBIM_DATA_CLASS flags will be returned here.
 * @param[out] pCurrentCellularClass Bitmap MBIM_CELLULAR_CLASS flags will be returned here. 
 * @param[in,out] pProviderIdLen If caller sets *pProviderIdLen = 0, on return it will be set to number of wchar_t's
 *                              in provider Id (excluding NULL-terminator), without returning the pProviderId (pProviderId may be NULL). 
 *                              If caller sets *pProviderIdLen > 0, function will return the minimum of this value
 *                              and the number of wchar_t's available (pProviderId must not be NULL). Make sure
 *                              pProviderId points to buffer of size *pProviderIdLen+1 (for NULL-terminator which will be appended). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call. 
 * @param[out] pProviderId Buffer to receive provider ID.
 * @param[in,out] pProviderIdLen If caller sets *pProviderNameLen = 0, on return it will be set to number of wchar_t's
 *                              in provider name (excluding NULL-terminator), without returning the pProviderName (pProviderName may be NULL). 
 *                              If caller sets *pProviderNameLen > 0, function will return the minimum of this value
 *                              and the number of wchar_t's available (pProviderName must not be NULL). Make sure
 *                              pProviderName points to buffer of size *pProviderNameLen+1 (for NULL-terminator which will be appended). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call. 
 * @param[out] pProviderName Buffer to receive provider name.
 * @param[in,out] pRoamingTextLen If caller sets *pRoamingTextLen = 0, on return it will be set to number of wchar_t's
 *                              in provider name (excluding NULL-terminator), without returning the pRoamingText (pRoamingText may be NULL). 
 *                              If caller sets *pRoamingTextLen > 0, function will return the minimum of this value
 *                              and the number of wchar_t's available (pRoamingText must not be NULL). Make sure
 *                              pRoamingText points to buffer of size *pRoamingTextLen+1 (for NULL-terminator which will be appended). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call. 
 * @param[out] pRoamingText Buffer to receive provider name.
 *
 * @param[out] pRegistrationFlag A bitmap of MBIM_REGISTRATION_FLAGS will be returned.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_RegisterStateParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    uint32_t* pNwError,
    MBIM_REGISTER_STATE* pRegisterState,
    MBIM_REGISTER_MODE* pRegisterMode,
    uint32_t* pAvailableDataClasses,
    uint32_t* pCurrentCellularClass,
    uint32_t* pProviderIdLen,
    wchar_t* pProviderId,
    uint32_t* pProviderNameLen,
    wchar_t* pProviderName,
    uint32_t* pRoamingTextLen,
    wchar_t* pRoamingText,
    uint32_t* pRegistrationFlag
    );

/** Maximum number of chars in access string (excluding NULL terminator) */
#define MBIM_ACCESS_STRING_MAX_LEN 100

/** Maximum number of chars in user name (excluding NULL terminator) */
#define MBIM_USER_NAME_MAX_LEN 255

/** Maximum number of chars in password (excluding NULL terminator) */
#define MBIM_PASSWORD_MAX_LEN 255

typedef enum 
{
    MBIMActivationCommandDeactivate = 0,
    MBIMActivationCommandActivate = 1,
} MBIM_ACTIVATION_COMMAND;

const char* MBIMActivationCommandToString(MBIM_ACTIVATION_COMMAND value);

typedef enum
{
    MBIMCompressionNone = 0,
    MBIMCompressionEnable = 1,
} MBIM_COMPRESSION;

const char* MBIMCompressionToString(MBIM_COMPRESSION value);

typedef enum
{
    MBIMAuthProtocolNone = 0,
    MBIMAuthProtocolPap = 1,
    MBIMAuthProtocolChap = 2,
    MBIMAuthProtocolMsChapV2 = 3,
} MBIM_AUTH_PROTOCOL;

const char* MBIMAuthProtocolToString(MBIM_AUTH_PROTOCOL value);

typedef enum
{
    MBIMContextIPTypeDefault = 0,
    MBIMContextIPTypeIPv4 = 1,
    MBIMContextIPTypeIPv6 = 2,
    MBIMContextIPTypeIPv4v6 = 3,
    MBIMContextIPTypeIPv4AndIPv6 = 4,
} MBIM_CONTEXT_IP_TYPE;

const char* MBIMContextIPTypeToString(MBIM_CONTEXT_IP_TYPE value);

typedef enum
{
    MBIMActivationStateUnknown = 0,
    MBIMActivationStateActivated = 1,
    MBIMActivationStateActivating = 2,
    MBIMActivationStateDeactivated = 3,
    MBIMActivationStateDeactivating = 4,
} MBIM_ACTIVATION_STATE;

const char* MBIMActivationStateToString(MBIM_ACTIVATION_STATE value);

typedef enum
{
    MBIMVoiceCallStateNone = 0,
    MBIMVoiceCallStateInProgress = 1,
    MBIMVoiceCallStateHangUp = 2,
} MBIM_VOICE_CALL_STATE;

const char* MBIMVoiceCallStateToString(MBIM_VOICE_CALL_STATE value);

/** 
 * MBIM_CONTEXT_TYPES
 * To use, declare a variable: uint8_t uuid[] = MBIMContextTypeXXX; 
 */

/** 
 * The context is not yet provisioned 
 */
#define MBIMContextTypeNone { \
    0xb4, 0x3f, 0x75, 0x8c, 0xa5, 0x60, 0x4b, 0x46, \
    0xb3, 0x5e, 0xc5, 0x86, 0x96, 0x41, 0xfb, 0x54 \
}

/**
 * The context represents a connection to the Internet
 * This context type is mandatory.
 */
#define MBIMContextTypeInternet { \
    0x7e, 0x5e, 0x2a, 0x7e, 0x4e, 0x6f, 0x72, 0x72, \
    0x73, 0x6b, 0x65, 0x6e, 0x7e, 0x5e, 0x2a, 0x7e \
}

/**
 * The context represents a connection
 * to virtual private network (VPN to a
 * corporate network).
 */ 
#define MBIMContextTypeVpn { \
    0x9b, 0x9f, 0x7b, 0xbe, 0x89, 0x52, 0x44, 0xb7, \
    0x83, 0xac, 0xca, 0x41, 0x31, 0x8d, 0xf7, 0xa0 \
}

/**
 * The context represents a connection
 * to a Voice-over-IP (VOIP) service
 */ 
#define MBIMContextTypeVoice { \
    0x88, 0x91, 0x82, 0x94, 0x0e, 0xf4, 0x43, 0x96, \
    0x8c, 0xca, 0xa8, 0x58, 0x8f, 0xbc, 0x02, 0xb2 \
}

/**
 * The context represents a connection
 * to a video sharing service
 */ 
#define MBIMContextTypeVideoShare { \
    0x05, 0xa2, 0xa7, 0x16, 0x7c, 0x34, 0x4b, 0x4d, \
    0x9a, 0x91, 0xc5, 0xef, 0x0c, 0x7a, 0xaa, 0xcc \
}

/**
 * The context represents a connection
 * to an over-the-air activation site.
 */ 
#define MBIMContextTypePurchase { \
    0xb3, 0x27, 0x24, 0x96, 0xac, 0x6c, 0x42, 0x2b, \
    0xa8, 0xc0, 0xac, 0xf6, 0x87, 0xa2, 0x72, 0x17 \
}

/**
 * The context represents a connection to IMS.
 */ 
#define MBIMContextTypeIMS { \
    0x21, 0x61, 0x0d, 0x01, 0x30, 0x74, 0x4b, 0xce, \
    0x94, 0x25, 0xb5, 0x3a, 0x07, 0xd6, 0x97, 0xd6 \
}

/**
 * The context represents a connection to MMS.
 */ 
#define MBIMContextTypeMMS { \
    0x46, 0x72, 0x66, 0x64, 0x72, 0x69, 0x6b, 0xc6, \
    0x96, 0x24, 0xd1, 0xd3, 0x53, 0x89, 0xac, 0xa9 \
}

/**
 * The context represents a local
 * connection which is terminated at
 * the device. Hence, the IP traffic will
 * not be sent over the air.
 */
#define MBIMContextTypeLocal { \
    0xa5, 0x7a, 0x9a, 0xfc, 0xb0, 0x9f, 0x45, 0xd7, \
    0xbb, 0x40, 0x03, 0x3c, 0x39, 0xf6, 0x0d, 0xb9 \
}

const char* MBIMContextTypeToString(uint8_t* value);

/**
 * Build a MBIM_CID_CONNECT set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] sessionId Session ID for this session.
 * @param[in] activationCommand Activation command.
 * @param[in] accessString Access string (NULL-terminated), may be NULL.
 * @param[in] userName User name (NULL-terminated), may be NULL .
 * @param[in] password Password (NULL-terminated), may be NULL.
 * @param[in] compression Compression type.
 * @param[in] authProtocol Authentication protocol.
 * @param[in] ipType IP type.
 * @param[in] contextType Context type, UUID initialized to one of MBIM_CONTEXT_TYPES.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_ConnectSetBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,  
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    uint32_t sessionId,
    MBIM_ACTIVATION_COMMAND activationCommand,
    wchar_t* accessString,
    wchar_t* userName,
    wchar_t* password,
    MBIM_COMPRESSION compression,
    MBIM_AUTH_PROTOCOL authProtocol,
    MBIM_CONTEXT_IP_TYPE ipType,
    uint8_t* contextType 
    );

/**
 * Build a MBIM_CID_CONNECT set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] sessionId Session ID for this session.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_ConnectQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,  
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    uint32_t sessionId
    );

/*
 * Parse a MBIM_CID_CONNECT response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pSessionId Session ID will be written here.
 * @param[out] pActivationState Activation state will be returned here. 
 * @param[out] pVoiceCallState Voice call state will be returned here.
 * @param[out] pIPType IP type will be returned here. 
 * @param[out] pContextType Context, one of MBIM_CONTEXT_TYPES will be written here. 
 *                          Must point to a buffer of MBIM_UUID_SIZE bytes.
 * @param[out] pNwError Network error be written here.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_ConnectParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    uint32_t* pSessionId, 
    MBIM_ACTIVATION_STATE* pActivationState,
    MBIM_VOICE_CALL_STATE* pVoiceCallState,
    MBIM_CONTEXT_IP_TYPE* pIPType,
    uint8_t* pContextType,
    uint32_t* pNwError);

#define MBIM_IPV4_ADDRESS_SIZE 4

/**
 *  This structure represents an IPv4 address.
 *  
 *  \param  value
 *          - byte array.
 * 
 */
typedef struct
{
    uint8_t value[MBIM_IPV4_ADDRESS_SIZE];
} MBIM_IPV4_ADDRESS;

/**
 *  This structure represents an IPv4 element.
 *  
 *  \param  OnLinkPrefixLength
 *          - Number of leftmost bits used to indicate subnet mask.
 *  \param  Address
 *          - Actual IP address.
 * 
 */
typedef struct
{
    uint32_t OnLinkPrefixLength;
    MBIM_IPV4_ADDRESS Address;
} MBIM_IPV4_ELEMENT;

#define MBIM_IPV6_ADDRESS_SIZE 16

/**
 *  This structure represents an IPv6 address.
 *  
 *  \param  value
 *          - byte array.
 * 
 */
typedef struct
{
    uint8_t value[MBIM_IPV6_ADDRESS_SIZE];
} MBIM_IPV6_ADDRESS;

/**
 *  This structure represents an IPv6 element.
 *  
 *  \param  OnLinkPrefixLength
 *          - Number of leftmost bits used to indicate subnet mask.
 *  \param  Address
 *          - Actual IP address.
 * 
 */
typedef struct
{
    uint32_t OnLinkPrefixLength;
    MBIM_IPV6_ADDRESS Address;
} MBIM_IPV6_ELEMENT;

/**
 * MBIM_IPV4_CONFIGURATION_AVAILABLE_FLAGS
 * A bitmap that indicates which IPv4 configuration values are available from device.
 */

/** IP address is available */
#define MBIM_IPV4_CONFIGURATION_AVAILABLE_ADDRESS 0x1
/** Gateway is available */
#define MBIM_IPV4_CONFIGURATION_AVAILABLE_GATEWAY 0x2
/** DNS is available */
#define MBIM_IPV4_CONFIGURATION_AVAILABLE_DNS 0x4
/** MTU is available */
#define MBIM_IPV4_CONFIGURATION_AVAILABLE_MTU 0x8

/**
 * MBIM_IPV6_CONFIGURATION_AVAILABLE_FLAGS
 * A bitmap that indicates which IPv6 configuration values are available from device.
 */

/** IP address is available */
#define MBIM_IPV6_CONFIGURATION_AVAILABLE_ADDRESS 0x1
/** Gateway is available */
#define MBIM_IPV6_CONFIGURATION_AVAILABLE_GATEWAY 0x2
/** DNS is available */
#define MBIM_IPV6_CONFIGURATION_AVAILABLE_DNS 0x4
/** MTU is available */
#define MBIM_IPV6_CONFIGURATION_AVAILABLE_MTU 0x8


/**
 * Build a MBIM_CID_IP_CONFIGURATION query payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] sessionId Session ID for this session.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_IpConfigurationQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,  
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    uint32_t sessionId
    );

/*
 * Parse a MBIM_CID_IP_CONFIGURATION response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pSession Session ID be written here.
 * @param[out] pIPv4ConfigurationAvailable Bitmask of MBIM_IPV4_CONFIGURATION_AVAILABLE_FLAGS will be returned here.
 * @param[out] pIPv6ConfigurationAvailable Bitmask of MBIM_IPV6_CONFIGURATION_AVAILABLE_FLAGS will be returned here.
 * @param[in,out] pIPv4AddressCount If caller sets *pIPv4AddressCount = 0, on return it will be set to number of elements
 *                              available, without returning any (pIPv4Addresses may be NULL).
 *                              If caller sets *pIPv4AddressCount > 0, function will return the minimum of this value
 *                              and the number of elements available (pIPv4Addresses must not be NULL). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call.    
 * @param[out] pIPv4Addresses Array of IPv4 addresses to be written to by parsing response.
 * @param[in,out] pIPv6AddressCount If caller sets *pIPv6AddressCount = 0, on return it will be set to number of elements
 *                              available, without returning any (pIPv6Addresses may be NULL).
 *                              If caller sets *pIPv6AddressCount > 0, function will return the minimum of this value
 *                              and the number of elements available (pIPv6Addresses must not be NULL). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call.    
 * @param[out] pIPv6Addresses Array of IPv6 addresses to be written to by parsing response.
 * @param[out] pIPv4Gateway Must Must not be NULL. IPv4 gateway if available, will be written here.
 * @param[out] pIPv6Gateway Must Must not be NULL. IPv6 gateway if available, will be written here.
 * @param[in,out] pIPv4DnsServerCount If caller sets *pIPv4DnsServerCount = 0, on return it will be set to number of elements
 *                              available, without returning any (pIPv4DnsServers may be NULL).
 *                              If caller sets *pIPv4DnsServerCount > 0, function will return the minimum of this value
 *                              and the number of elements available (pIPv4DnsServers must not be NULL). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call.    
 * @param[out] pIPv4DnsServers Array of IPv4 DNS servers to be written to by parsing response.
 * @param[in,out] pIPv6DnsServerCount If caller sets *pIPv6DnsServerCount = 0, on return it will be set to number of elements
 *                              available, without returning any (pIPv6DnsServers may be NULL).
 *                              If caller sets *pIPv6DnsServerCount > 0, function will return the minimum of this value
 *                              and the number of elements available (pIPv6DnsServers must not be NULL). 
 *                              This allows caller to optionally determine storage required in first call and 
 *                              retrieve actual data in second call.    
 * @param[out] pIPv6DnsServers Array of IPv6 DNS servers to be written to by parsing response.
 * @param[out] pIPv4Mtu IPv4 MTU will be written here if available.
 * @param[out] pIPv6Mtu IPv6 MTU will be written here if available.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_IpConfigurationParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    uint32_t* pSessionId,
    uint32_t* pIPv4ConfigurationAvailable,
    uint32_t* pIPv6ConfigurationAvailable,
    uint32_t* pIPv4AddressCount,
    MBIM_IPV4_ELEMENT* pIPv4Addresses, 
    uint32_t* pIPv6AddressCount,
    MBIM_IPV6_ELEMENT* pIPv6Addresses, 
    MBIM_IPV4_ADDRESS* pIPv4Gateway, 
    MBIM_IPV6_ADDRESS* pIPv6Gateway, 
    uint32_t* pIPv4DnsServerCount,
    MBIM_IPV4_ADDRESS* pIPv4DnsServers, 
    uint32_t* pIPv6DnsServerCount,
    MBIM_IPV6_ADDRESS* pIPv6DnsServers, 
    uint32_t* pIPv4Mtu,
    uint32_t* pIPv6Mtu);


typedef enum
{
    /** No PIN is pending to be entered */
    MBIMPinTypeNone = 0,
    /** 
     * The PIN type is a custom type and is none of
     * the other PIN types listed in this enumeration 
     */
    MBIMPinTypeCustom = 1,
    /** The PIN1 key */
    MBIMPinTypePin1 = 2,
    /** The PIN2 key */
    MBIMPinTypePin2 = 3,
    /** The device to SIM key */
    MBIMPinTypeDeviceSimPin = 4,
    /** The device to very first SIM key */
    MBIMPinTypeDeviceFirstSimPin = 5,
    /** The network personalization key */
    MBIMPinTypeNetworkPin = 6,
    /** The network subset personalization key */
    MBIMPinTypeNetworkSubsetPin = 7,
    /** The service provider (SP) personalization key */
    MBIMPinTypeServiceProviderPin = 8,
    /** The corporate personalization key */
    MBIMPinTypeCorporatePin = 9,
    /** The subsidy unlock key */
    MBIMPinTypeSubsidyLock = 10,
    /** The Personal Identification Number1 Unlock Key (PUK1). */
    MBIMPinTypePuk1 = 11,
    /** The Personal Identification Number2 Unlock Key (PUK2). */
    MBIMPinTypePuk2 = 12,
    /** The device to very first SIM PIN unlock key */
    MBIMPinTypeDeviceFirstSimPuk = 13,
    /** The network personalization unlock key */
    MBIMPinTypeNetworkPuk = 14,
    /** The network subset personalization unlock key */
    MBIMPinTypeNetworkSubsetPuk = 15,
    /** The service provider (SP) personalization unlock key */
    MBIMPinTypeServiceProviderPuk = 16,
    /** The corporate personalization unlock key */
    MBIMPinTypeCorporatePuk = 17,
} MBIM_PIN_TYPE;

const char* MBIMPinTypeToString(MBIM_PIN_TYPE value);

typedef enum 
{
    /** The device does not require a PIN */
    MBIMPinStateUnlocked = 0,
    /** The device requires the user to enter a PIN */
    MBIMPinStateLocked = 1,
} MBIM_PIN_STATE;

const char* MBIMPinStateToString(MBIM_PIN_STATE value);

typedef enum
{
    /** Enter the specified PIN into the device. */
    MBIMPinOperationEnter = 0,
    /** Enable the specified PIN */
    MBIMPinOperationEnable = 1,
    /** Disable the specified PIN */
    MBIMPinOperationDisable = 2,
    /** Change the specified PIN */
    MBIMPinOperationChange = 3,
} MBIM_PIN_OPERATION;

const char* MBIMPinOperationToString(MBIM_PIN_OPERATION value);

/**
 * Build a MBIM_CID_PIN set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] pinType PIN type to be operated upon.
 * @param[in] pinOperation Operation to be performed.
 * @param[in] pPin PIN value (NULL-terminated), may be NULL.
 * @param[in] pNewPin PIN value (NULL-terminated), may be NULL.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_PinSetBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,  
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    MBIM_PIN_TYPE pinType,
    MBIM_PIN_OPERATION pinOperation,
    wchar_t* pPin,
    wchar_t* pNewPin);
    
/**
 * Build a MBIM_CID_PIN query payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_PinQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType);
    
/*
 * Parse a MBIM_CID_PIN response.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pPinType Current PIN type will be written here. 
 * @param[out] pPinState Current PIN state will be written here.
 * @param[out] pRemainingAttempts Remaining attempts to enter PIN info wil be written here.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_PinParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    MBIM_PIN_TYPE* pPinType,
    MBIM_PIN_STATE* pPinState,
    uint32_t* pRemainingAttempts);

typedef enum 
{
    /** Radio is off */
    MBIMRadioOff = 0,
    /** Radio is on */
    MBIMRadioOn = 1,
} MBIM_RADIO_SWITCH_STATE;

const char* MBIMRadioToString(MBIM_RADIO_SWITCH_STATE value);

/**
 * Build a MBIM_CID_RADIO_STATE set payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 * @param[out] informationBuffer Buffer where MBIM payload will be constructed.
 * @param[in,out] pInformationBufferLength in = informationBuffer capacity, out = bytes used.
 * @param[in] radioState Desired radio state.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_RadioStateSetBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType,  
    uint8_t* informationBuffer, 
    uint32_t* pInformationBufferLength, // in = information buffer capacity, out = bytes used
    MBIM_RADIO_SWITCH_STATE radioState);

/**
 * Build a MBIM_CID_RADIO_STATE query payload.
 * 
 * @param[out] ppDeviceServiceId Device service UUID will be placed here on return.
 * @param[out] pCid CID for command will be placed here on return. 
 * @param[out] pCommandType MBIM_COMMAND_TYPE will be placed here on return.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_BUILD_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_RadioStateQueryBuild(
    const uint8_t** ppDeviceServiceId,
    uint32_t* pCid,
    uint32_t* pCommandType);  

/*
 * Parse a MBIM_CID_RADIO_STATE.
 * 
 * @param[in] informationBuffer  Response's information data.
 * @param[in] informationBufferLength Number of bytes of information in response.
 * @param[out] pHwRadioState Current hardware radio state will be written here. 
 * @param[out] pSwRadioState Current software radio state will be written here.
 * @param[out] pRemainingAttempts Remaining attempts to enter PIN info wil be written here.
 *       
 * @return MBIM_STATUS_SUCCESS on success,
 *         MBIM_STATUS_CUSTOM_PARSE_FAILURE on invalid parameters, or memory issues. 
 */
uint32_t BasicConnectDeviceService_RadioStateParse(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength,
    MBIM_RADIO_SWITCH_STATE* pHwRadioState,
    MBIM_RADIO_SWITCH_STATE* pSwRadioState);



#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif //__BASIC_CONNECT_DEVICE_SERVICE_H__
