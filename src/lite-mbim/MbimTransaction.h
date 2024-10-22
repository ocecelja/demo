/**
 * \ingroup litembim
 *
 * \file MbimTransaction.h
 */
#ifndef __MBIM_TRANSACTION_H__
#define __MBIM_TRANSACTION_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup litembim
 * 
 * This function prototype represents the callback which is called by the MBIM transport 
 * when a MBIM transaction completes.
 * 
 * @param[in] status MBIM status of the MBIM transaction.
 * 
 * @param[in] transactionId Transaction ID of associated MBIM transaction.
 * 
 * @param[in] informationBuffer Transaction response's informationBuffer.
 *
 * @param[in] informationBufferLength Number of bytes in transaction response's informationBuffer.
 *
 * @param[in] pDoneCallbackContext User context supplied when transaction was initialized.
*/
typedef void (*MBIM_DONE_CALLBACK)(
    uint32_t status, 
    uint32_t transactionId, 
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength, 
    void* pDoneCallbackContext
    );


/**
 * \ingroup litembim
 * 
 *  This structure abstracts a MBIM transaction.
 *  It is responsible for tracking a MBIM request to its response.
 *
 *  \param  pPrev
 *          - A link so that this object can be stored in a linked list by the
 *            transport layer. This points to the previous object or NULL if 
 *            this is the first object in the list.
 * 
 *  \param  pNext
 *          - A link so that this object can be stored in a linked list by the
 *            transport layer. This points to the next object or NULL if 
 *            this is the last object in the list.
 * 
 *  \param  transactionId
 *          - MBIM transaction ID. 
 *            Can never be 0.
 *             
 *  \param  pDoneCallback
 *          - User callback for MBIM response.
 *
 *  \param  pDoneCallbackContext
 *          - User context for user callback.
 *
 *  \param  status
 *          - MBIM status of response, typically contained in 
 *            first response fragment.
 */
typedef struct MbimTransaction  
{
    struct MbimTransaction* pPrev;
    struct MbimTransaction* pNext;
    
    uint32_t transactionId;
    MBIM_DONE_CALLBACK pDoneCallback;
    void* pDoneCallbackContext;

    uint32_t status; 

} MbimTransaction;


/**
 * \ingroup litembim
 * 
 * Initialize MBIM transaction.
 * @param[in] pThis         The primary object of this call.
 * @param[in] transactionId Transaction ID obtained from a call to 
 *                          MbimTransport_GetNextTransactionId.
 * @param[in] pDoneCallback   User function to be be called when MBIM
 *                                response is received.    
 * @param[in] pDoneCallbackContext   A user context for callback.    
 *       
 */
void MbimTransaction_Initialize(
    MbimTransaction* pThis,
    uint32_t transactionId,
    MBIM_DONE_CALLBACK pDoneCallback,
    void* pDoneCallbackContext
);


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif //__MBIM_TRANSACTION_H__
