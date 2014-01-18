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
* $FileName: usb_msc.h$
* $Version : 
* $Date    : 
*
* Comments:
*
* @brief The file contains USB stack MSC class layer api header function.
*
*****************************************************************************/

#ifndef _USB_MSC_H
#define _USB_MSC_H 1

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_device_stack_interface.h"
	
#include "usb_class.h"

#include "usb_class_msc.h"

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/

/* If Implementing Disk Drive then configure the macro below as TRUE,
   otherwise keep it False(say for Hard Disk)*/
#define IMPLEMENTING_DISK_DRIVE         (0) /*1: TRUE; 0:FALSE*/
#define MSD_RECV_BUFFER_SIZE            (512)
#define MSD_SEND_BUFFER_SIZE            (512)



/* Class specific request Codes */
#define BULK_ONLY_MASS_STORAGE_RESET          (0xFF)
#define GET_MAX_LUN                           (0xFE)
#define PUT_REQUESTS                          (0xFD)
#define GET_REQUESTS                          (0xFC)

/* Events to the Application */ /* 0 to 4 are reserved for class events */

/* other macros */
#define  USB_DCBWSIGNATURE       USB_LONG_BE_TO_HOST_CONST(0x55534243)
#define  USB_DCSWSIGNATURE       USB_LONG_BE_TO_HOST_CONST(0x55534253)
#define  USB_CBW_DIRECTION_BIT   (0x80)
#define  USB_CBW_DIRECTION_SHIFT (7)
#define  MSC_CBW_LENGTH          (31)
#define  MSC_CSW_LENGTH          (13)

#define COMMAND_PASSED                (0x00)
#define COMMAND_FAILED                (0x01)
#define PHASE_ERROR                   (0x02)


/* MACROS FOR COMMANDS SUPPORTED */
#define INQUIRY_COMMAND                    (0x12)
#define READ_10_COMMAND                    (0x28)
#define READ_12_COMMAND                    (0xA8)
#define REQUEST_SENSE_COMMAND              (0x03)
#define TEST_UNIT_READY_COMMAND            (0x00)
#define WRITE_10_COMMAND                   (0x2A)
#define WRITE_12_COMMAND                   (0xAA)
#define PREVENT_ALLOW_MEDIUM_REM_COMMAND   (0x1E)
#define FORMAT_UNIT_COMMAND                (0x04)
#define READ_CAPACITY_10_COMMAND           (0x25)
#define READ_FORMAT_CAPACITIES_COMMAND     (0x23)
#define MODE_SENSE_10_COMMAND              (0x5A) 
#define MODE_SENSE_6_COMMAND               (0x1A)
#define MODE_SELECT_10_COMMAND             (0x55)
#define MODE_SELECT_6_COMMAND              (0x15)
#define SEND_DIAGNOSTIC_COMMAND            (0x1D)
#define VERIFY_COMMAND                     (0x2F)
#define START_STOP_UNIT_COMMAND            (0x1B)
 
#define MAX_MSC_DEVICE                     (0x01)
#define MAX_MSC_SUPPORTED_INTERFACES       (0x01)
 
/*****************************************************************************
 * Local Functions
 *****************************************************************************/
void USB_Service_Bulk_In(PTR_USB_EVENT_STRUCT event,void* arg);
void USB_Service_Bulk_Out(PTR_USB_EVENT_STRUCT event,void* arg);
void USB_Class_MSC_Event(uint8_t event, void* val,void* arg);
uint8_t USB_MSC_Requests(USB_SETUP_STRUCT * setup_packet, uint8_t * *data, uint32_t *size, void* arg);

/******************************************************************************
 * Types
 *****************************************************************************/
typedef enum
 {
    USB_MSD_EP_COUNT = 1,
    USB_MSD_INTERFACE_COUNT,
	USB_MSD_CLASS_INFO,
 }USB_MSD_DESC_INFO_T;

typedef struct _usb_msc_cbw   /* Command Block Wrapper -- 31 bytes */
{
    uint32_t             signature;        /*0-3  : dCBWSignature*/
    uint32_t             tag;              /*4-7  : dCBWTag*/
    uint32_t             data_length;      /*8-11 : dCBWDataTransferLength*/
    uint8_t              flag;             /*12   : bmCBWFlags*/
    uint8_t              lun;              /*13   : bCBWLUN(bits 3 to 0)*/
    uint8_t              cb_length;        /*14   : bCBWCBLength*/
    uint8_t              command_block[16];/*15-30 : CBWCB*/
}CBW, *PTR_CBW ;

typedef struct _usb_msc_csw   /* Command Status Wrapper -- 13 bytes */
{
    uint32_t             signature; /*0-3  : dCSWSignature*/
    uint32_t             tag;       /*4-7 : dCSWTag*/
    uint32_t             residue;   /*8-11 : dCSWDataResidue*/
    uint8_t              csw_status; /*12 : bCSWStatus*/
}CSW, *PTR_CSW;

typedef struct _lba_info_struct
{
    uint32_t             starting_lba;/* LBA to start transfering with */
    uint32_t             lba_transfer_num;/* number of LBAs to transfer */    
}LBA_INFO_STRUCT, * PTR_LBA_INFO_STRUCT;

typedef struct _msc_thirteen_case_check
{
    _usb_device_handle   handle;
    uint32_t             host_expected_data_len;
    uint32_t             device_expected_data_len;
    uint8_t*             csw_status_ptr;
    uint32_t*            csw_residue_ptr;
    uint8_t*             buffer_ptr;
    bool                 lba_txrx_select;
    LBA_INFO_STRUCT      lba_info;
    uint8_t              host_expected_direction;
    uint8_t              device_expected_direction;
}MSC_THIRTEEN_CASE_STRUCT, *PTR_MSC_THIRTEEN_CASE_STRUCT;



/* MSD Device Structure */
typedef struct _msc_variable_struct
{
    _usb_device_handle                         controller_handle;
	uint32_t                                   user_handle;
    MSD_HANDLE                                 msc_handle;
    USB_CLASS_HANDLE                           class_handle;
    USB_ENDPOINTS*                             ep_desc_data;
    USB_APPLICATION_CALLBACK_STRUCT            msc_application_callback;
    USB_VENDOR_REQ_CALLBACK_STRUCT             vendor_req_callback;            
    USB_CLASS_SPECIFIC_HANDLER_CALLBACK_STRUCT class_specific_callback;
    DESC_REQUEST_NOFIFY_STRUCT_PTR             desc_callback_ptr;  
    /* contains the endpoint info */
    /* Memory Allocation for endpoint done at App layer. Only App
       Knows how many endpoints to allocate.
      */
    void*                                      scsi_object_ptr;
    USB_CLASS_MSC_ENDPOINT_DATA                msc_endpoint_data;
    USB_CLASS_MSC_ENDPOINT*                    ep;
    /* macro configuired by user*/
    /* flag to track bulk out data processing after CBW if needed*/
    bool                                       out_flag; 
    /* flag to track bulk in data processing before CSW if needed*/
    bool                                       in_flag;
    /* flag to track if there is need to stall BULK IN ENDPOINT because of BULK COMMAND*/
    bool                                       in_stall_flag;
    /* flag to track if there is need to stall BULK OUT ENDPOIN because of BULK COMMAND */
    bool                                       out_stall_flag;
    /* flag to validate CBW */
    bool                                       cbw_valid_flag;
    bool                                       re_stall_flag;
    /* global structure for command status wrapper */
    PTR_CSW                                    csw_ptr;
    /* global structure for command block wrapper */
    PTR_CBW                                    cbw_ptr;    
    DEVICE_LBA_INFO_STRUCT                     device_info;
    MSD_BUFF_INFO                              msd_buff;
    /* following two macros are used to manage transfers involving read/write 
       on APPLICATION MASS STORAGE DEVICE */
    uint32_t                                   transfer_remaining;
    uint32_t                                   current_offset;
    uint32_t                                   bulk_in_endpoint_packet_size;
    uint32_t                                   usb_max_suported_interfaces;
    uint8_t                                    bulk_in_endpoint;
    uint8_t                                    bulk_out_endpoint;
    /* LUN can have value only from 0 to 15 decimal */
    uint8_t                                    lun;
}MSC_DEVICE_STRUCT, * MSC_DEVICE_STRUCT_PTR; 

/**************************************************************************//*!
 *
 * @name  USB_Class_MSC_Send_Data
 *
 * @brief 
 *
 * @param msc_handle   :   handle returned from USB_Class_MSC_Init
 * @param ep_num          :   endpoint num 
 * @param app_buff        :   buffer to send
 * @param size            :   length of the transfer   
 *
 * @return status       
 *         USB_OK           : When Successfull 
 *         Others           : Errors
 *****************************************************************************/
extern uint8_t USB_Class_MSC_Send_Data
(
    MSD_HANDLE          msc_handle,
    uint8_t              ep_num,
    uint8_t *           buff_ptr,      /* [IN] buffer to send */      
    uint32_t             size           /* [IN] length of the transfer */
);

/**************************************************************************//*!
 *
 * @name  USB_MSC_LBA_Transfer
 *
 * @brief 
 *
 * @param MSC_DEVICE_STRUCT_PTR   
 * @param direction       :   transfer direction
 * @param lba_info_ptr    :   buffer to send
 *
 * @return status       
 *         USB_OK           : When Successfull 
 *         Others           : Errors
 *****************************************************************************/
extern uint8_t USB_MSC_LBA_Transfer
(
    MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
    bool direction,
    PTR_LBA_INFO_STRUCT lba_info_ptr
);

#define USB_MSC_Bulk_Send_Data(a,b,c)  USB_Class_MSC_Send_Data(a,msc_obj_ptr->bulk_in_endpoint,b,c)
//#define USB_MSC_Bulk_Send_Data(a,b,c)  USB_Class_MSC_Send_Data(a,1,b,c)
#define USB_MSC_Bulk_Recv_Data(a,b,c)  usb_device_recv_data(a,msc_obj_ptr->bulk_out_endpoint,b,c)

/* Sub Class Functions */

 /**************************************************************************//*!
 *
 * @name  USB_MSC_SCSI_Init
 *
 * @brief The funtion initializes the SCSI parameters and callbacks
 *
 * @param msc_obj_ptr         MSD class object pointer.
 * @param cb:                 event callback
 * @param storage_disk        Ramdisk Memory pointer.
 * @param lb                  logical block
 * @param lab_len             lenght of each logical blocks
 *
 * @return status
 *         USB_OK           : When Successfull
 *         Others           : Errors
 *
 *****************************************************************************/
extern uint8_t USB_MSC_SCSI_Init
(
     MSC_DEVICE_STRUCT_PTR  msc_obj_ptr,
     USB_CLASS_SPECIFIC_HANDLER_CALLBACK_STRUCT_PTR cb,
     PTR_DEVICE_LBA_INFO_STRUCT device_info_ptr,
     uint32_t implementing_disk_drive
);

/**************************************************************************//*!
 *
 * @name  USB_MSC_SCSI_Deinit
 *
 * @brief The funtion deinitializes the SCSI parameters and callbacks
 *
 * @param msc_obj_ptr         MSD class object pointer.
 * @return status
 *         USB_OK           : When Successfull
 *         Others           : Errors
 *
 *****************************************************************************/
extern uint8_t USB_MSC_SCSI_Deinit
(
     MSC_DEVICE_STRUCT_PTR  msc_obj_ptr 
);

/**************************************************************************//*!
 *
 * @name  msc_inquiry_command
 *
 * @brief It requests that information regarding parameters of the Device be
 *        sent to the Host Computer
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/

extern uint8_t msc_inquiry_command (MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                   PTR_CBW cbw_ptr, 
                                   uint32_t* csw_residue_ptr, 
                                   uint8_t* csw_status_ptr);
/**************************************************************************//*!
 *
 * @name  msc_read_command
 *
 * @brief command requests that device transfer data to the host computer
 *        (read (10) command)
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_read_command (MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                PTR_CBW cbw_ptr, 
                                uint32_t* csw_residue_ptr, 
                                uint8_t* csw_status_ptr); 
 /**************************************************************************//*!
 *
 * @name  msc_request_sense_command
 *
 * @brief command instructs the Device to transfer sense data to host computer
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return NONE
 *
 *****************************************************************************/
extern uint8_t msc_request_sense_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                        PTR_CBW cbw_ptr, 
                                        uint32_t* csw_residue_ptr, 
                                        uint8_t* csw_status_ptr); 
/**************************************************************************//*!
 *
 * @name  msc_test_unit_ready_command
 *
 * @brief It provides a means to check if the device is ready
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_test_unit_ready_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                          PTR_CBW cbw_ptr, 
                                          uint32_t* csw_residue_ptr, 
                                          uint8_t* csw_status_ptr); 
/**************************************************************************//*!
 *
 * @name  msc_verify_command
 *
 * @brief requests that device verifies the data on medium
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_verify_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                 PTR_CBW cbw_ptr,
                                 uint32_t* csw_residue_ptr,
                                 uint8_t* csw_status_ptr);                                         
/**************************************************************************//*!
 *
 * @name  msc_mode_sense_command
 *
 * @brief command provides a means for a Device to report parameters to Host
 *        Computer.It is a complementary command to the MODE SELECT command.
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_mode_sense_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                     PTR_CBW cbw_ptr, 
                                     uint32_t* csw_residue_ptr, 
                                     uint8_t* csw_status_ptr);
/**************************************************************************//*!
 *
 * @name  msc_mode_select_command
 *
 * @brief command provides a means for a Device to report parameters to Host
 *        Computer.It is a complementary command to the MODE SENSE command.
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_mode_select_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                      PTR_CBW cbw_ptr, 
                                      uint32_t* csw_residue_ptr, 
                                      uint8_t* csw_status_ptr);                                     
/**************************************************************************//*!
 *
 * @name  msc_read_capacity_command
 *
 * @brief command provides a means for the host computer to request information
 *        regarding the capacity of the installed medium of the device.
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_read_capacity_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                        PTR_CBW cbw_ptr, 
                                        uint32_t* csw_residue_ptr, 
                                        uint8_t* csw_status_ptr); 
/**************************************************************************//*!
 *
 * @name  msc_format_unit_command
 *
 * @brief host sends the FORMAT UNIT command to physically format a diskette
 *        according to selected options
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_format_unit_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                      PTR_CBW cbw_ptr, 
                                      uint32_t* csw_residue_ptr, 
                                      uint8_t* csw_status_ptr); 
/**************************************************************************//*!
 *
 * @name  msc_write_command
 *
 * @brief command requests that the Device write the data transferred by the
 *        Host Computer to the medium.
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_write_command (MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                 PTR_CBW cbw_ptr, 
                                 uint32_t* csw_residue_ptr, 
                                 uint8_t* csw_status_ptr); 
/**************************************************************************//*!
 *
 * @name  msc_start_stop_unit_command
 *
 * @brief command instructs device to enable or disable media access operations
 *
 * @param controller_ID:        To identify the controller 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_start_stop_unit_command
(
    MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
    PTR_CBW cbw_ptr,
    uint32_t* csw_residue_ptr,
    uint8_t* csw_status_ptr
);
                                 
/**************************************************************************//*!
 *
 * @name  msc_prevent_allow_medium_removal
 *
 * @brief command tells the UFI device to enable or disable the removal of the
 *        medium in the logical unit.
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_prevent_allow_medium_removal(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                               PTR_CBW cbw_ptr, 
                                               uint32_t* csw_residue_ptr, 
                                               uint8_t* csw_status_ptr); 
/**************************************************************************//*!
 *
 * @name  msc_read_format_capacity_command
 *
 * @brief allows the host to request a list of the possible capacities that
 *        can be formatted on the currently installed medium
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_read_format_capacity_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                               PTR_CBW cbw_ptr, 
                                               uint32_t* csw_residue_ptr, 
                                               uint8_t* csw_status_ptr);  
                                               
/**************************************************************************//*!
 *
 * @name  msc_send_diagnostic_command
 *
 * @brief requests the device to perform self test
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_send_diagnostic_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                          PTR_CBW cbw_ptr, 
                                          uint32_t* csw_residue_ptr, 
                                          uint8_t* csw_status_ptr);     
                                                                                  
/**************************************************************************//*!
 *
 * @name  msc_unsupported_command
 *
 * @brief Responds appropriately to unsupported commands
 *
 * @param msc_obj_ptr 
 * @param cbw_ptr        : pointer to Command Block Wrapper sent by host
 * @param csw_residue_ptr: pointer to dCSWDataResidue of Command Status Wrapper
 * @param csw_status_ptr : pointer to bCSWStatus  of Command Status Wrapper
 *
 * @return error
 *
 *****************************************************************************/
extern uint8_t msc_unsupported_command(MSC_DEVICE_STRUCT_PTR msc_obj_ptr,
                                      PTR_CBW cbw_ptr, 
                                      uint32_t* csw_residue_ptr, 
                                      uint8_t* csw_status_ptr);

/* EOF */
#endif

/* EOF */
