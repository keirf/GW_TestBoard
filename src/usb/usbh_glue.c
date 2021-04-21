/*
 * usbh_glue.c
 * 
 * Glue sitting above USBH CDC Host layer.
 * 
 * Written & released by Keir Fraser <keir.xen@gmail.com>
 * 
 * This is free and unencumbered software released into the public domain.
 * See the file COPYING for more details, or visit <http://unlicense.org>.
 */

#include "usbh_cdc.h"

static bool_t cdc_device_connected;

extern USB_OTG_CORE_HANDLE USB_OTG_Core;
USBH_HOST USB_Host;

static void USBH_USR_Init(void)
{
    printk("> %s\n", __FUNCTION__);
}

static void USBH_USR_DeInit(void)
{
    printk("> %s\n", __FUNCTION__);
    cdc_device_connected = FALSE;
}

static void USBH_USR_DeviceAttached(void)
{
    printk("> %s\n", __FUNCTION__);
}

static void USBH_USR_ResetDevice(void)
{
    printk("> %s\n", __FUNCTION__);
}

static void USBH_USR_DeviceDisconnected (void)
{
    printk("> %s\n", __FUNCTION__);
    cdc_device_connected = FALSE;
}

static void USBH_USR_OverCurrentDetected (void)
{
    printk("> %s\n", __FUNCTION__);
}

static void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    printk("> %s\n", __FUNCTION__);
    printk("> Device speed: %s\n",
           (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED) ? "High" :
           (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED) ? "Full" :
           (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED) ? "Low" : "???");
}

static void USBH_USR_DeviceDescAvailable(void *DeviceDesc)
{
    USBH_DevDesc_TypeDef *hs = DeviceDesc;
    printk("> %s\n", __FUNCTION__);
    printk(" VID : %04X\n", hs->idVendor);
    printk(" PID : %04X\n", hs->idProduct);
}

static void USBH_USR_DeviceAddressAssigned(void)
{
    printk("> %s\n", __FUNCTION__);
}

static void USBH_USR_ConfigurationDescAvailable(
    USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
    USBH_InterfaceDesc_TypeDef *id = itfDesc;

    printk("> %s\n", __FUNCTION__);
    printk("> Class connected: %02x (%s)\n",
           id->bInterfaceClass,
           (id->bInterfaceClass == USB_CDC_CLASS) ? "CDC" : "???");
}

static void USBH_USR_ManufacturerString(void *ManufacturerString)
{
    printk(" Manufacturer : %s\n", (char *)ManufacturerString);
}

static void USBH_USR_ProductString(void *ProductString)
{
    printk(" Product : %s\n", (char *)ProductString);
}

static void USBH_USR_SerialNumString(void *SerialNumString)
{
    printk(" Serial Number : %s\n", (char *)SerialNumString);
}

static void USBH_USR_EnumerationDone(void)
{
    printk("> %s\n", __FUNCTION__);
}

static USBH_USR_Status USBH_USR_UserInput(void)
{
    printk("> %s\n", __FUNCTION__);
    return USBH_USR_RESP_OK;
}

static int USBH_USR_UserApplication(void)
{
    printk("> %s\n", __FUNCTION__);
    cdc_device_connected = TRUE;
    /* 1 forces reset, 0 okay */
    return 0;
}

static void USBH_USR_DeviceNotSupported(void)
{
    printk("> %s\n", __FUNCTION__);
}

static void USBH_USR_UnrecoveredError (void)
{
    printk("> %s\n", __FUNCTION__);
    cdc_device_connected = FALSE;
}

static USBH_Usr_cb_TypeDef USR_cb = {
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_DeviceDescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_ConfigurationDescAvailable,
    USBH_USR_ManufacturerString,
    USBH_USR_ProductString,
    USBH_USR_SerialNumString,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    USBH_USR_UserApplication,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
};

void usbh_cdc_init(void)
{
    USBH_Init(&USB_OTG_Core, 
              USB_OTG_FS_CORE_ID,
              &USB_Host,
              &USBH_CDC_cb, 
              &USR_cb);
}

void usbh_cdc_buffer_set(uint8_t *buf)
{
    Cfg_Rx_Buffer = buf;
}

void usbh_cdc_process(void)
{
    USBH_Process(&USB_OTG_Core, &USB_Host);
}

bool_t usbh_cdc_connected(void)
{
    return cdc_device_connected && HCD_IsDeviceConnected(&USB_OTG_Core);
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "Linux"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
