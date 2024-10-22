/**
 * \ingroup litembim
 *
 * \file MbimSyncObject.h
 */
#ifndef __MBIM_SYNC_OBJECT_H__
#define __MBIM_SYNC_OBJECT_H__

#include <pthread.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup litembim
 * 
 *  This structure is a wrapper around a mutex and condition object
 *  and an external buffer. It is a convenience object which may be used 
 *  to create synchronous MBIM APIs.   
 *
 *  \param  mutex
 *          - For thread safety.
 * 
 *  \param  condition
 *          - Triggered on completion.
 *             
 *  \param  status
 *          - The returned MBIM transaction status.
 *             
 *  \param  informationBuffer
 *          - Buffer to which the transaction response's informationBuffer will be copied.
 *
 *  \param  informationBufferLength
 *          - Set to maximum expected size in bytes of transaction response's informationBuffer.
 *            On request completion, it is set to size of transaction response's informationBuffer.
 *
*/
typedef struct MbimSyncObject
{
    pthread_mutex_t mutex;
    pthread_cond_t  condition;
    uint32_t status;
    uint8_t* informationBuffer;
    uint32_t informationBufferLength;  
} MbimSyncObject;

/**
 * \ingroup litembim
 * 
 * Initialize sync object.
 * 
 * @param[in]  pThis         
 *          - The primary object of this call.
 *       
 * @param[in]  informationBuffer
 *          - Buffer to which the transaction response's informationBuffer will be copied.
 *
 * @param[in]  informationBufferLength
 *          - Maximum expected size in bytes of transaction response's informationBuffer.
 *          
 * @return 0 on success, < 0 on failure. 
*/
int MbimSyncObject_Initialize(
    MbimSyncObject* pThis, 
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength 
);

/**
 * \ingroup litembim
 * 
 * Destroy sync object.
 * 
 * @param[in] pThis         The primary object of this call.
 *       
 * @return 0 on success, < 0 on failure. 
*/
int MbimSyncObject_Destroy(MbimSyncObject* pThis);

/**
 * \ingroup litembim
 * 
 * Lock sync object.
 * 
 * @param[in] pThis         The primary object of this call.
 *       
 * @return 0 on success, < 0 on failure. 
*/
int MbimSyncObject_Lock(MbimSyncObject* pThis);

/**
 * \ingroup litembim
 * 
 * Unlock sync object.
 * 
 * @param[in] pThis         The primary object of this call.
 *       
 * @return 0 on success, < 0 on failure. 
*/
int MbimSyncObject_Unlock(MbimSyncObject* pThis);

/**
 * \ingroup litembim
 * 
 * Block, waiting on sync object.
 * 
 * @param[in] pThis         The primary object of this call.
 *       
 * @return 0 on success, < 0 on failure. 
*/
int MbimSyncObject_Wait(MbimSyncObject* pThis);

/**
 * \ingroup litembim
 * 
 * Block, waiting on sync object for a number of seconds.
 * 
 * @param[in] pThis         The primary object of this call.
 *       
 * @param[in] timeoutSeconds Timeout in seconds before abondoning wait.
 *       
 * @return 0 on success, ETIMEDOUT, or < 0 on failure. 
*/
int MbimSyncObject_TimedWait(MbimSyncObject* pThis, time_t timeoutSeconds);

/**
 * \ingroup litembim
 * 
 * Signal sync object. Waiting thread should wake up.
 * 
 * @param[in] pThis         The primary object of this call.
 *       
 * @return 0 on success, or < 0 on failure. 
*/
int MbimSyncObject_Signal(MbimSyncObject* pThis);

/**
 * \ingroup litembim
 * 
 * This funcion copies MBIM command response's informationBuffer to this object's
 * informationBuffer and signals the waiting thread.
 * 
 * @param[in] status MBIM status of the MBIM transaction.
 * 
 * @param[in] transactionId Transaction ID of associated MBIM transaction.
 * 
 * @param[in] informationBuffer Transaction response's informationBuffer.
 *
 * @param[in] informationBufferLength Number of bytes in transaction response's informationBuffer.
 *
 * @param[in] pDoneCallbackContext Pointer to this sync object.
*/
void MbimSyncObject_DoneCallback(
    uint32_t status, 
    uint32_t transactionId,
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength, 
    void* pDoneCallbackContext
    );


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif //__MBIM_SYNC_OBJECT_H__
