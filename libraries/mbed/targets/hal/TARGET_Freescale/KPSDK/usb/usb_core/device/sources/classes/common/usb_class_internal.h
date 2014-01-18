/**HEADER********************************************************************
* 
* Copyright (c) 2008, 2013 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: usb_class_internal.h$
* $Version : 
* $Date    : 
*
* Comments:
*
* @brief The file contains USB stack class layer api header function.
*
*****************************************************************************/

#ifndef _USB_CLASS_INTERNAL_H
#define _USB_CLASS_INTERNAL_H 1

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_class.h"


/******************************************************************************
 * Global Variables
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/

/******************************************************************************
 * Types
 *****************************************************************************/
/* Strucutre holding USB class object state*/
typedef struct _usb_class_object
{
   uint32_t           usb_fw_handle;
   _usb_device_handle controller_handle;
   void*              arg;
   USB_DEVICE_NOFIFY  class_callback;   
}USB_CLASS_OBJECT_STRUCT,* USB_CLASS_OBJECT_STRUCT_PTR;

/******************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  USB_Class_Init
 *
 * @brief The funtion initializes the Class Module 
 *
 * @param handle             :handle to Identify the controller
 * @param class_callback     :event callback      
 * @param other_req_callback :call back for class/vendor specific requests on 
 *                            CONTROL ENDPOINT
 *
 * @return status       
 *         USB_OK           : When Successfull 
 *         Others           : Errors
 *
 *****************************************************************************/
USB_CLASS_HANDLE USB_Class_Init
(
    _usb_device_handle              handle,            /* [IN] the USB device controller to initialize */                  
    USB_DEVICE_NOFIFY               class_callback,    /*[IN]*/
    USB_REQUEST_NOTIFY              other_req_callback,/*[IN]*/
    void*                           user_arg,          /*[IN]*/
    DESC_REQUEST_NOFIFY_STRUCT_PTR  desc_callback_ptr  /*[IN]*/
); 

/**************************************************************************//*!
 *
 * @name  USB_Class_Deinit
 *
 * @brief The funtion initializes the Class Module 
 *
 * @param handle             :handle to Identify the controller
 * @param class_handle       :class handle      
 *
 * @return status       
 *         USB_OK           : When Successfull 
 *         Others           : Errors
 *
 *****************************************************************************/
uint8_t USB_Class_Deinit
(
    _usb_device_handle              handle, /* [IN] the USB device controller to initialize */                  
    USB_CLASS_HANDLE                class_handle
 );
 /**************************************************************************//*!
 *
 * @name  USB_Class_Send_Data
 *
 * @brief The funtion calls the device to send data upon recieving an IN token 
 *
 * @param handle:               handle to Identify the controller
 * @param ep_num:               The endpoint number     
 * @param buff_ptr:             buffer to send
 * @param size:                 length of transfer
 * 
 * @return status       
 *         USB_OK           : When Successfull 
 *         Others           : Errors
 *
 *****************************************************************************/
uint8_t USB_Class_Send_Data
(
    USB_CLASS_HANDLE                handle,   /*[IN]*/
    uint8_t                         ep_num,   /* [IN] the Endpoint number */                  
    uint8_t*                        buff_ptr, /* [IN] buffer to send */      
    uint32_t                        size      /* [IN] length of the transfer */
); 

/**************************************************************************//*!
 *
 * @name  USB_Class_Get_Desc
 *
 * @brief  This function is called in to get the descriptor as specified in cmd.
 *
 * @param handle:           USB class handle. Received from
 *                          USB_Class_Init      
 * @param cmd:              command for USB discriptor to get.
 * @param in_data:          input to the Application functions.
 * @param out_buf           buffer which will contian the discriptors.
 * @return status:       
 *                        USB_OK : When Successfull       
 *                        Others : When Error
 *
 *****************************************************************************/
uint8_t USB_Class_Get_Desc(
    USB_CLASS_HANDLE                handle,     /*[IN]*/
    int32_t                         cmd,        /*[IN]*/
    uint8_t                         input_data, /*[IN]*/
    uint8_t**                       in_buf      /*[OUT]*/
);

/**************************************************************************//*!
 *
 * @name  USB_Class_Set_Desc
 *
 * @brief  This function is called in to Set the descriptor as specified in cmd.
 *
 * @param handle:           USB class handle. Received from
 *                          USB_Class_Init      
 * @param cmd:              command for USB discriptor to get.
 * @param in_data:          input to the Application functions.
 * @param in_buf           buffer which will contian the discriptors.
 * @return status:       
 *                        USB_OK : When Successfull       
 *                        Others : When Error
 *
 *****************************************************************************/
uint8_t USB_Class_Set_Desc(
    USB_CLASS_HANDLE                handle,     /*[IN]*/
    int32_t                         cmd,        /*[IN]*/
    uint8_t                         input_data, /*[IN]*/
    uint8_t**                       out_buf     /*[IN]*/
);

/**************************************************************************//*!
 *
 * @name   USB_Class_Periodic_Task
 *
 * @brief  The funtion calls for periodic tasks 
 *
 * @param  None
 *
 * @return None
 *
 *****************************************************************************/
extern void USB_Class_Periodic_Task(void);

#ifdef COMPOSITE_CONFIG
/**************************************************************************//*!
 *
 * @name  USB_Class_Get_Class_Handle
 *
 * @brief  This function is called to return class handle.
 *
 * @return value:
 *                        class handle
 *
 *****************************************************************************/
USB_CLASS_HANDLE USB_Class_Get_Class_Handle();

/**************************************************************************//*!
 *
 * @name  USB_Class_Get_Ctrler_Handle
 *
 * @brief  This function is called to return controller handle.
 *
 * @return value:
 *                        controller handle
 *
 *****************************************************************************/
_usb_device_handle USB_Class_Get_Ctrler_Handle(USB_CLASS_HANDLE class_handle);
#endif

#endif

/* EOF */
