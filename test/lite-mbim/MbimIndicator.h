/**
 * \ingroup litembim
 *
 * \file MbimIndicator.h
 */
#ifndef __MBIM_INDICATOR_H__
#define __MBIM_INDICATOR_H__

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
 * when a MBIM indication is received.
 * 
 * @param[in] informationBuffer Indication's informationBuffer.
 *
 * @param[in] informationBufferLength Number of bytes in indication's informationBuffer.
 *
 * @param[in] pIndicationCallbackContext User context supplied when indicator was initialized.
*/
typedef void (*MBIM_INDICATION_CALLBACK)(
    uint8_t* deviceServiceId, 
    uint32_t cid, 
    uint8_t* informationBuffer, 
    uint32_t informationBufferLength, 
    void* pIndicationCallbackContext
    );

/**
 * \ingroup litembim
 * 
 *  This structure abstracts a MBIM indication callback.
 *  Users who wish to be notified of MBIM indications can
 *  register one of these objects with the MbimTransport object using
 *  MbimTransport_AttachIndicator/MbimTransport_DetachIndicator calls.
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
 *  \param  pIndicationCallback
 *          - User callback for MBIM indication.
 *
 *  \param  pIndicationCallbackContext
 *          - User context for user callback.
 *
 */
typedef struct MbimIndicator  
{
    struct MbimIndicator* pPrev;
    struct MbimIndicator* pNext;
    
    MBIM_INDICATION_CALLBACK pIndicationCallback;
    void* pIndicationCallbackContext;
} MbimIndicator;


/**
 * \ingroup litembim
 * 
 * Initialize the indicator object.
 * 
 * @param[in] pThis         The primary object of this call.
 * @param[in] pIndicationCallback User function to be be called when MBIM
 *                                indication is received.    
 * @param[in] pIndicationCallbackContext   A user context for callback.    
*/
void MbimIndicator_Initialize(
    MbimIndicator* pThis,
    MBIM_INDICATION_CALLBACK pIndicationCallback,
    void* pIndicationCallbackContext
);


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif //__MBIM_INDICATION_H__
