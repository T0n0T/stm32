# 固件特定编译选项
firmware_c_args = [
  '-DUSE_HAL_DRIVER',
  '-DSTM32L431xx',
]

# 包含目录
firmware_incdirs = include_directories(
  'CubeMX/Core/Inc',
  'CMSIS/Include',
  'CMSIS/Device/ST/STM32L4xx/Include',
  'STM32L4xx_HAL_Driver/Inc',
  'STM32L4xx_HAL_Driver/Inc/Legacy',
)

# 源文件列表初始化
firmware_sources = files(
  'CubeMX/Core/Src/main.c',
  'CubeMX/Core/Src/stm32l4xx_hal_msp.c',
  'CMSIS/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c',
  'CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l431xx.s',
)

# 添加 HAL 驱动源文件
firmware_sources += files(
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_can.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_comp.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dcmi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dfsdm.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dfsdm_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma2d.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dsi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_firewall.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gfxmmu.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hash.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hash_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hcd.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_irda.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lcd.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lptim.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_ltdc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_ltdc_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_mmc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_mmc_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nand.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nor.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_ospi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pka.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pssi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smbus.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smbus_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sram.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_swpmi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tsc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart_ex.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_wwdg.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_adc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_comp.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_crc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_crs.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dac.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dma.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dma2d.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_exti.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_fmc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_i2c.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_lptim.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_lpuart.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_opamp.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_pka.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_pwr.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rcc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rng.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rtc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_sdmmc.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_spi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_swpmi.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_tim.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usart.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c',
  'STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_utils.c', 
)

# 条件包含 USB Device 库
if get_option('STM32_USB_Device')
  firmware_incdirs += include_directories(
    'STM32_USB_Device_Library/Core/Inc',
    'STM32_USB_Device_Library/Class/HID/Inc',
    'STM32_USB_Device_Library/Class/MSC/Inc',
    'STM32_USB_Device_Library/Class/VIDEO/Inc',
    'STM32_USB_Device_Library/Class/MTP/Inc',
    'STM32_USB_Device_Library/Class/DFU/Inc',
    'STM32_USB_Device_Library/Class/CDC_ECM/Inc',
    'STM32_USB_Device_Library/Class/Template/Inc',
    'STM32_USB_Device_Library/Class/CCID/Inc',
    'STM32_USB_Device_Library/Class/CDC_RNDIS/Inc',
    'STM32_USB_Device_Library/Class/AUDIO/Inc',
    'STM32_USB_Device_Library/Class/CompositeBuilder/Inc',
    'STM32_USB_Device_Library/Class/BillBoard/Inc',
    'STM32_USB_Device_Library/Class/CDC/Inc',
    'STM32_USB_Device_Library/Class/Printer/Inc',
    'STM32_USB_Device_Library/Class/CustomHID/Inc',
  )
  firmware_sources += files(
    'STM32_USB_Device_Library/Core/Src/usbd_core.c',
    'STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c',
    'STM32_USB_Device_Library/Core/Src/usbd_ioreq.c',
    # Template 文件通常需要用户实现，但根据列表添加
    # 'STM32_USB_Device_Library/Core/Src/usbd_conf_template.c',
    # 'STM32_USB_Device_Library/Core/Src/usbd_desc_template.c',
    'STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c',
    'STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.c',
    'STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.c',
    'STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.c',
    'STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.c',
    # 'STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_storage_template.c', # Template
    'STM32_USB_Device_Library/Class/VIDEO/Src/usbd_video.c',
    # 'STM32_USB_Device_Library/Class/VIDEO/Src/usbd_video_if_template.c', # Template
    'STM32_USB_Device_Library/Class/MTP/Src/usbd_mtp.c',
    'STM32_USB_Device_Library/Class/MTP/Src/usbd_mtp_opt.c',
    'STM32_USB_Device_Library/Class/MTP/Src/usbd_mtp_storage.c', # 可能需要用户实现
    # 'STM32_USB_Device_Library/Class/MTP/Src/usbd_mtp_if_template.c', # Template
    'STM32_USB_Device_Library/Class/DFU/Src/usbd_dfu.c',
    # 'STM32_USB_Device_Library/Class/DFU/Src/usbd_dfu_media_template.c', # Template
    'STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.c',
    # 'STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.c', # Template
    # 'STM32_USB_Device_Library/Class/Template/Src/usbd_template.c', # Template
    'STM32_USB_Device_Library/Class/CCID/Src/usbd_ccid.c',
    'STM32_USB_Device_Library/Class/CCID/Src/usbd_ccid_cmd.c',
    # 'STM32_USB_Device_Library/Class/CCID/Src/usbd_ccid_if_template.c', # Template
    # 'STM32_USB_Device_Library/Class/CCID/Src/usbd_ccid_sc_if_template.c', # Template
    # 'STM32_USB_Device_Library/Class/CCID/Src/usbd_ccid_smartcard_template.c', # Template
    'STM32_USB_Device_Library/Class/CDC_RNDIS/Src/usbd_cdc_rndis.c',
    # 'STM32_USB_Device_Library/Class/CDC_RNDIS/Src/usbd_cdc_rndis_if_template.c', # Template
    'STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.c',
    # 'STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio_if_template.c', # Template
    'STM32_USB_Device_Library/Class/CompositeBuilder/Src/usbd_composite_builder.c',
    'STM32_USB_Device_Library/Class/BillBoard/Src/usbd_billboard.c',
    'STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c',
    # 'STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc_if_template.c', # Template
    'STM32_USB_Device_Library/Class/Printer/Src/usbd_printer.c',
    # 'STM32_USB_Device_Library/Class/Printer/Src/usbd_printer_if_template.c', # Template
    'STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid.c',
    # 'STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid_if_template.c', # Template
  )
endif

# 条件包含 USB Host 库
if get_option('STM32_USB_Host')
  firmware_incdirs += include_directories(
    'STM32_USB_Host_Library/Core/Inc',
    'STM32_USB_Host_Library/Class/HID/Inc',
    'STM32_USB_Host_Library/Class/MSC/Inc',
    'STM32_USB_Host_Library/Class/MTP/Inc',
    'STM32_USB_Host_Library/Class/Template/Inc',
    'STM32_USB_Host_Library/Class/AUDIO/Inc',
    'STM32_USB_Host_Library/Class/CDC/Inc',
  )
  firmware_sources += files(
    'STM32_USB_Host_Library/Core/Src/usbh_core.c',
    'STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c',
    'STM32_USB_Host_Library/Core/Src/usbh_ioreq.c',
    'STM32_USB_Host_Library/Core/Src/usbh_pipes.c',
    # 'STM32_USB_Host_Library/Core/Src/usbh_conf_template.c', # Template
    'STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c',
    'STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.c',
    'STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.c',
    'STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.c',
    'STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c',
    'STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c',
    'STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c',
    'STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp.c',
    'STM32_USB_Host_Library/Class/MTP/Src/usbh_mtp_ptp.c',
    # 'STM32_USB_Host_Library/Class/Template/Src/usbh_template.c', # Template
    'STM32_USB_Host_Library/Class/AUDIO/Src/usbh_audio.c',
    'STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.c',
  )
endif

# 编译为静态库
firmware_lib = static_library(
  'firmware',
  sources: firmware_sources,
  include_directories: firmware_incdirs,
  c_args: c_args, # 使用更新后的 C 参数
  pic: false, # 对于嵌入式目标通常不需要 PIC
)

# 创建 firmware 依赖项，供根 meson.build 使用
firmware_dep = declare_dependency(
  link_with: firmware_lib,
  include_directories: firmware_incdirs
)

# # 导出依赖项
meson.override_dependency('firmware', firmware_dep)