/**
 * \ingroup litembim
 *
 * \file MbimTransport.h
 */
#ifndef __MBIM_TRANSPORT_H__
#define __MBIM_TRANSPORT_H__


#include <stdint.h>
#include <pthread.h>
#include "MbimIndicator.h"
#include "MbimTransaction.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MBIM_UUID_SIZE 16 /**< Number of bytes in a MBIM device service's UUID */

#define MBIM_MAX_CIDS 64  /**< Max CIDs per device service. Chosen with some head-room. Current max is 25. */

// TODO: determine at runtime by quering USB descriptor or ioctl to driver.
#define MBIM_MAX_CTRL_TRANSFER 4096  /**< This value should be determined at run-time. */


#define MBIM_COMMAND_TYPE_QUERY 0  /**< MBIM query command */	
#define MBIM_COMMAND_TYPE_SET   1  /**< MBIM set command */

#define MBIM_STATUS_SUCCESS   0  /**< The operation succeeded. */
#define MBIM_STATUS_FAILURE   2  /**< The operation failed (a generic failure). */
#define MBIM_STATUS_INVALID_PARAMETERS 21 /**< The operation failed because of invalid parameters. */
#define MBIM_STATUS_READ_FAILURE   22  /**< The operation failed because of a read failure. */
#define MBIM_STATUS_WRITE_FAILURE   23  /**< The operation failed because of a write failure. */
#define MBIM_STATUS_CUSTOM_BUILD_FAILURE   0xfffffff0  /**< Failed to build a command payload. */
#define MBIM_STATUS_CUSTOM_PARSE_FAILURE   0xfffffff1  /**< Failed to parse a response/indication payload. */

/**
 * \ingroup litembim
 * 
 * MBIM transport error type definitions
 */
typedef enum
{
    MBIM_TRANSPORT_ERR_UNKNOWN, // Unknown error type
    MBIM_TRANSPORT_ERR_READ,    // Error occurred during read() operation
    MBIM_TRANSPORT_ERR_WRITE,   // Error occurred during write() operation
} MBIM_TRANSPORT_ERR_TYPE;

/**
 * \ingroup litembim
 * 
 * This structure contains the detailed error info occurred in MBIM transport layer.
 * 
 * \param   err_type
 *          - Type of error defined in MBIM_TRANSPORT_ERR_TYPE.
 * 
 * \param   errno_val
 *          - errno value set by the system when error occurs.
 */
typedef struct 
{
    MBIM_TRANSPORT_ERR_TYPE  err_type;
    int                      errno_val;
} MBIM_TRANSPORT_ERR_INFO;

/**
 * \ingroup litembim
 * 
 * MBIM transport error callback function.
 * 
 * @param[in]   pErrCallbackContext User context of the callback function
 * @param[in]   err_info            A MBIM_TRANSPORT_ERR_INFO structure which contains
 *                                  the detailed error info.
 * 
 * @return      None
 */
typedef void (*MBIM_TRANSPORT_ERR_CALLBACK)(void * pErrCallbackContext, MBIM_TRANSPORT_ERR_INFO err_info);

/**
 * \ingroup litembim
 * 
 *  This structure abstracts a MBIM transport layer.
 *  It is responsible for applying MBIM headers to packets 
 *  destined for MBIM device services and forwarding them to the device.
 *  It is responsible for receiving MBIM packets from the device.
 *  On the basis of MBIM headers, it assembles multi-fragment responses
 *  and indications and delivers them back to user.  
 *
 *  \param  deviceFd
 *          - File descriptor of device.
 * 
 *  \param  devRemoved
 *          - Flag indicating device has been removed.
 * 
 *  \param  shutdownFd
 *          - Event descriptor. Used to signal read thread to terminate.
 *
 *  \param  writeLock
 *          - Provides mutual exclusion for write operations. 
 *
 *  \param  readThread
 *          - Thread responsible for reading from device. 
 *
 *  \param  transactionId
 *          - MBIM transaction ID. 
 *            Upper limit 0xffffffff. Never 0. Increments for each transaction.
 *             
 *  \param  transactionIdLock
 *          - Mutex to make transactionId access thread-safe. 
 *
 *  \param  transactionList
 *          - Head of linked list of outstanding transactions. 
 *
 *  \param  transactionListLock
 *          - Provides thread safety for the transaction list.
 *             
 *  \param  pIncomingMessage
 *          - Private workspace to assemble incoming information fragments.
 *             
 *  \param  indicatorList
 *          - Head of linked list of indicators which are called when MBIM indications 
 *            are received. 
 *
 *  \param  indicatorListLock
 *          - Provides thread safety for the indicators list.
 * 
 *  \param  pErrCallback
 *          - Callback function called by MbimTransport when an error has occurred
 * 
 *  \param  pErrCallbackContext
 *          - User context for MbimTransport error callback
 *             
 */
typedef struct MbimTransport
{
    int deviceFd;   // FD of device.
    bool devRemoved;    // Flag indicating device has been removed.
#ifndef EVENT_FD_UNSUPPORTED
    int shutdownFd; // FD of shutdown event.
#else
    int shutdownFd[2]; // FDs of shutdown pipe.
#endif
    pthread_mutex_t writeLock;  // Protect write operations.
    pthread_t readThread;
    uint32_t transactionId;
    pthread_mutex_t transactionIdLock;
    MbimTransaction* transactionList;       // Head of list of outstanding transactions.
    pthread_mutex_t transactionListLock;
    struct MultiFragmentMessage* pIncomingMessage; // Workspace to assemble incoming fragments.
    MbimIndicator* indicatorList;       // Head of list of indicators.
    pthread_mutex_t indicatorListLock;
    MBIM_TRANSPORT_ERR_CALLBACK pErrCallback;
    void * pErrCallbackContext;
	int initRetry;
	time_t timeOut;
} MbimTransport;

/**
 * \ingroup litembim
 * 
 * Initialize this MBIM transport object.
 * 
 * @param[in] pThis         The primary object of this call.
 * @param[in] devicePath    Absolute path to device.
 * @param[in] maxExpectedInformationLength  Maximum expected length of information content.
 *       
 * @return 0 on success, < 0 on failure. 
 * 
 * @note   maxExpectedInformationLength is used to allocate a buffer for assembling fragments for
 *         incoming command responses and indications.
*/
int MbimTransport_Initialize(MbimTransport* pThis, char *devicePath, uint32_t maxExpectedInformationLength);

/**
 * \ingroup litembim
 * 
 * Shut down this MBIM transport object.
 * @param[in] pThis      The primary object of this call.
 *       
*/
void MbimTransport_ShutDown(MbimTransport* pThis);

/**
 * \ingroup litembim
 * 
 * Send a MBIM command to the device and return immediately.
 * 
 * @param[in] pThis         The primary object of this call.
 * @param[in] deviceServiceId Device service UUID which is recipient of command.
 * @param[in] cid   Command ID (CID) which is specific to that device service.
 * @param[in] commandType 1 = set, 0 = get.
 * @param[in] informationBuffer  Buffer of information content.
 * @param[in] informationBufferLength Size in bytes of information content.
 * @param[in] pTransaction Transaction object which will track the command/response.
 *       
 * @return 0 on success, < 0 on failure. 
 *
 * @note pTransaction must have been initialized with MbimTransaction_Initialize prior to this call.
 * 
*/
int MbimTransport_SendCommand( 
                            MbimTransport* pThis,
                            const uint8_t* deviceServiceId,
                            uint32_t cid,
                            uint32_t commandType,  
                            uint8_t* informationBuffer,
                            uint16_t informationBufferLength,
                            MbimTransaction* pTransaction
						  );

/**
 * \ingroup litembim
 * 
 * Returns next transaction ID for MBIM transport.
 * @param[in] pThis      The primary object of this call.
 *       
 * @return transaction ID
*/
uint32_t MbimTransport_GetNextTransactionId(MbimTransport* pThis);

/**
 * \ingroup litembim
 * 
 * Cancel an outstanding MBIM transaction.
 * @param[in] pThis      The primary object of this call.
 * @param[in] transactionId  ID of transaction to be cancelled.
 *       
*/
void MbimTransport_CancelTransaction(MbimTransport* pThis, uint32_t transactionId);

/**
 * \ingroup litembim
 * 
 * Attaches a MBIM indicator to this MBIM transport object.
 * @param[in] pThis      The primary object of this call.
 * @param[in] pIndicator Indicator to be attached.
 *       
*/
void MbimTransport_AttachIndicator(MbimTransport* pThis, MbimIndicator* pIndicator);

/**
 * \ingroup litembim
 * 
 * Detaches a MBIM indicator to this MBIM transport object.
 * @param[in] pThis      The primary object of this call.
 * @param[in] pIndicator Indicator to be detached.
 *       
*/
void MbimTransport_DetachIndicator(MbimTransport* pThis, MbimIndicator* pIndicator);

/**
 * This function prototype represents the callback which is called  to parse a command response
 * or indication.
 * 
 * @param[in] informationBuffer Transaction response's informationBuffer.
 *
 * @param[in] informationBufferLength Number of bytes in transaction response's informationBuffer.
 *
 * @param[in] pParseCallbackContext User context supplied when transaction was initialized.
 * 
 * @return MBIM_STATUS_SUCCESS on success. MBIM_STATUS_CUSTOM_PARSE_FAILURE on error.
*/
typedef uint32_t (*MBIM_PARSE_CALLBACK)(
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength, 
    void* pParseCallbackContext
    );

/**
 * Convenience function to send a MBIM command and wait for response.
 * 
 * @param[in] pTransport    The primary object of this call.
 * @param[in] deviceService Device service UUID which is recipient of command.
 * @param[in] cid   Command ID (CID) which is specific to that device service.
 * @param[in] commandType 1 = set, 0 = get.
 * @param[in] informationBuffer  Buffer of information content.
 * @param[in] informationBufferLength Size in bytes of information content.
 * @param[in] pParseCallback User supplied callback. This is called only if function returns MBIM_STATUS_SUCCESS.
 * @param[in] pParseCallbackContext Context for user supplied callback.
 * @param[in] timeout Timeout in seconds for this command to execute.
 *       
 * @return MBIM_STATUS_SUCCESS on success, 
 *         MBIM_STATUS_WRITE_FAILURE on failure to send command,
 *         MBIM_STATUS_READ_FAILURE on response timeout,
 *         Any other MBIM_STATUS_xxx as reported by the device.
 *
 * @note The caller is required to handle any parsing errors in the context of pParseCallback.
 */
uint32_t MbimTransport_ExecuteCommandSynchronously(
	MbimTransport* pTransport,
	const uint8_t* deviceService,
	uint32_t cid,
	uint32_t commandType,  
	uint8_t* informationBuffer,
	uint16_t informationBufferLength,
	MBIM_PARSE_CALLBACK pParseCallback,
    void* pParseCallbackContext,
	time_t timeout
);

/**
 * \ingroup litembim
 * 
 * This function is for client applications to register for lite-mbim transport error notifications.
 * The most recent callback registration is the only function that is invoked when 
 * device is removed.
 *
 * @param[in]   pThis               The primary object of this call.
 * @param[in]   pErrCallback        A valid function pointer to be notified when transport error occurred.
 *                                  NULL to disable callback notifications.
 * @param[in]   pErrCallbackContext User context when transport error callback is called.
 *
 * @return      None
 */ 
void MbimTransport_RegisterErrCallback(
    MbimTransport * pThis, 
    MBIM_TRANSPORT_ERR_CALLBACK pErrCallback,
    void * pErrCallbackContext
);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif //__MBIM_TRANSPORT_H__
