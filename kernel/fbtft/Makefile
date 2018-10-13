#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=fbtft-eb904
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

KERNELSRC_URL=https://cdn.kernel.org/pub/linux/kernel/v4.x/
KERNELSRC_FILE=linux-$(LINUX_VERSION).tar.xz

include $(INCLUDE_DIR)/package.mk

VIDEO_MENU:=Video Support

ifeq ("$(wildcard $(LINUX_DIR)/drivers/video/fbdev/core/syscopyarea.ko)","") # this fail sometimes because wildcard does not works correctly
  KO_EXIST=0
else
  KO_EXIST=1
endif

#FIXUP:=1

define KernelPackage/fbtft-eb904
  SECTION:=kernel
  CATEGORY:=Kernel modules
  PKGFLAGS:=shared
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=EasyBox 904 display support
# Include kmod-fbcon in case the display should be used for console output
  DEPENDS:=@(TARGET_lantiq_xrx200_DEVICE_lantiq_vgv952cjw33-e-ir) kmod-fb kmod-fb-cfb-copyarea kmod-fb-cfb-fillrect kmod-fb-cfb-imgblt kmod-fb-sys-fops
  FILES:=\
	$(LINUX_DIR)/drivers/video/fbdev/core/syscopyarea.ko \
	$(LINUX_DIR)/drivers/video/fbdev/core/sysfillrect.ko \
	$(LINUX_DIR)/drivers/video/fbdev/core/sysimgblt.ko \
	$(LINUX_DIR)/drivers/video/backlight/backlight.ko \
	$(LINUX_DIR)/drivers/staging/fbtft/fbtft.ko \
	$(PKG_BUILD_DIR)/fb_ili9341_eb904.ko
  ifeq ("$(KO_EXIST)","0")
    DEPENDS:=@(TARGET_lantiq_xrx200_DEVICE_lantiq_vgv952cjw33-e-ir)
    FILES:=\
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/lib/fonts/font.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/fb.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/cfbcopyarea.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/cfbfillrect.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/cfbimgblt.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/fb_sys_fops.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/syscopyarea.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/sysfillrect.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core/sysimgblt.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/backlight/backlight.ko \
	$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/staging/fbtft/fbtft.ko \
	$(PKG_BUILD_DIR)/fb_ili9341_eb904.ko
  
  endif
# Not really needed, because the device is initialized via DTS
#	$(LINUX_DIR)/drivers/staging/fbtft/fbtft_device.ko
  AUTOLOAD:=$(call AutoLoad,06,fb_ili9341_eb904 fbtft)
#  EXTRA_CFLAGS:= \
#	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
#	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG))))
endef

define KernelPackage/fbtft-eb904/description
 Enable support for EasyBox 904 display.
endef



EXTRA_KCONFIG:= \
  	CONFIG_STAGING=y \
	CONFIG_FB_TFT=m \
	CONFIG_BACKLIGHT_LCD_SUPPORT=y \
	CONFIG_LCD_CLASS_DEVICE=n \
	CONFIG_FB_TFT_FBTFT_DEVICE=n \
	\
	CONFIG_FB_TFT_AGM1264K_FL=n \
	CONFIG_FB_TFT_BD663474=n \
	CONFIG_FB_TFT_HX8340BN=n \
	CONFIG_FB_TFT_HX8347D=n \
	CONFIG_FB_TFT_HX8353D=n \
	CONFIG_FB_TFT_HX8357D=n \
	CONFIG_FB_TFT_ILI9163=n \
	CONFIG_FB_TFT_ILI9320=n \
	CONFIG_FB_TFT_ILI9325=n \
	CONFIG_FB_TFT_ILI9340=n \
	CONFIG_FB_TFT_ILI9341=n \
	CONFIG_FB_TFT_ILI9481=n \
	CONFIG_FB_TFT_ILI9486=n \
	CONFIG_FB_TFT_PCD8544=n \
	CONFIG_FB_TFT_RA8875=n \
	CONFIG_FB_TFT_S6D02A1=n \
	CONFIG_FB_TFT_S6D1121=n \
	CONFIG_FB_TFT_SSD1289=n \
	CONFIG_FB_TFT_SSD1306=n \
	CONFIG_FB_TFT_SSD1331=n \
	CONFIG_FB_TFT_SSD1351=n \
	CONFIG_FB_TFT_ST7735R=n \
	CONFIG_FB_TFT_ST7789V=n \
	CONFIG_FB_TFT_TINYLCD=n \
	CONFIG_FB_TFT_TLS8204=n \
	CONFIG_FB_TFT_UC1611=n \
	CONFIG_FB_TFT_UC1701=n \
	CONFIG_FB_TFT_UPD161704=n \
	CONFIG_FB_TFT_WATTEROTT=n \
	CONFIG_FB_FLEX=n \
	CONFIG_FB_TFT_SSD1305=n \
	CONFIG_FB_TFT_SSD1325=n \
	CONFIG_FB_TFT_SH1106=n \
	CONFIG_FONT_SUPPORT=m
	
# Enable for framebuffer console support
#  EXTRA_KCONFIG+= \
#	CONFIG_VGA_CONSOLE=n\
#	CONFIG_VT=y\
#	CONFIG_VT_CONSOLE=y\
#	CONFIG_VT_HW_CONSOLE_BINDING=y\
#	CONFIG_FB_CMDLINE=y\
#	CONFIG_CONSOLE_TRANSLATIONS=y\
#	CONFIG_FONT_8x16=y\
#	CONFIG_FONT_8x8=y\
#	CONFIG_FONT_SUPPORT=m\
#	CONFIG_FRAMEBUFFER_CONSOLE=m\
#	CONFIG_HW_CONSOLE=y

ifeq ("$(KO_EXIST)","0")
EXTRA_KCONFIG+= \
	CONFIG_FB_CFB_FILLRECT=m \
	CONFIG_FB_CFB_COPYAREA=m \
	CONFIG_FB_CFB_IMAGEBLIT=m \
	CONFIG_FB_SYS_FILLRECT=m \
	CONFIG_FB_SYS_COPYAREA=m \
	CONFIG_FB_SYS_IMAGEBLIT=m \
	CONFIG_FB_SYS_FOPS=m \
	CONFIG_FB_DEFERRED_IO=y \
	CONFIG_FB_BACKLIGHT=y \
	CONFIG_BACKLIGHT_LCD_SUPPORT=y \
	CONFIG_BACKLIGHT_CLASS_DEVICE=m \
	CONFIG_FONT_SUPPORT=m
endif

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	$(KERNEL_MAKE_FLAGS) \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)



#MAKE_KMOD := $(MAKE) -C "$(LINUX_DIR)" \
#		$(KERNEL_MAKE_FLAGS) \
#               PATH="$(TARGET_PATH)"
                
             

define Download/kernelsource
	URL:=$(KERNELSRC_URL)
	FILE:=$(KERNELSRC_FILE)
	HASH:=skip
	#298e7f3545e33596a5876f6b9c3da7ef475c2692e5fab0df824fbcd7a489cd80
endef
                
define Build/Prepare
	$(call Build/Prepare/Default)
  #ifeq ("$(KO_EXIST)","0")
#		$(eval $(call Download,kernelsource))
#		mkdir -p $(LINUX_DIR)
#		( $(TAR) -kxf $(DL_DIR)/$(KERNELSRC_FILE) --strip=1 -C $(LINUX_DIR) || exit 0; )
 # endif
  ifeq ("$(KO_EXIST)","0")
		$(eval $(call Download,kernelsource))
		mkdir -p $(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)
		( $(TAR) -kxf $(DL_DIR)/$(KERNELSRC_FILE) --strip=1 -C $(PKG_BUILD_DIR)/linux-$(LINUX_VERSION) || exit 0; )
  endif
endef

define Build/Compile
	# Compile the kernel part
	
  ifeq ("$(KO_EXIST)","0")
	$(MAKE) -C "$(LINUX_DIR)" \
		SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/lib/fonts" \
		$(MAKE_OPTS) \
		MODFLAGS="-DMODULE -mlong-calls" \
		modules
  
	$(MAKE) -C "$(LINUX_DIR)" \
		SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video" \
		$(MAKE_OPTS) \
		MODFLAGS="-DMODULE -mlong-calls" \
		modules
		
	#read
		# KBUILD_EXTRA_SYMBOLS=$(LINUX_DIR)/Module.symvers \
  
  
	#$(MAKE) -C "$(LINUX_DIR)" \
	#	SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core" \
	#	$(MAKE_OPTS) \
	#	MODFLAGS="-DMODULE -mlong-calls" \
	#	modules
	#	# KBUILD_EXTRA_SYMBOLS=$(LINUX_DIR)/Module.symvers \
		
	#$(MAKE) -C "$(LINUX_DIR)" \
	#	SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/backlight" \
	#	$(MAKE_OPTS) \
	#	MODFLAGS="-DMODULE -mlong-calls" \
	#	modules
  
	$(MAKE) -C "$(LINUX_DIR)" \
		SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/staging/fbtft" \
		$(MAKE_OPTS) \
		MODFLAGS="-DMODULE -mlong-calls" \
		KBUILD_EXTRA_SYMBOLS=$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/Module.symvers $(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/lib/fonts/Module.symvers \
		modules
	
	#$(MAKE_KMOD) \
	#	SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core" \
	#	$(KCONFIG) \
	#	modules
		
	#$(MAKE_KMOD) \
		SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/staging/fbtft" \
		MODFLAGS="-DMODULE -mlong-calls" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		modules
	#read

	#$(MAKE_KMOD) \
	#	SUBDIRS="$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/video/fbdev/core" \
	#	MODFLAGS="-DMODULE -mlong-calls" \
	#	modules
  endif
	 
	#$(MAKE_KMOD) \
	#	SUBDIRS="$(PKG_BUILD_DIR)" \
	#	MODFLAGS="-DMODULE -mlong-calls" \
	#	modules
	$(MAKE) -C "$(LINUX_DIR)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		$(MAKE_OPTS) \
		MODFLAGS="-DMODULE -mlong-calls" \
		KBUILD_EXTRA_SYMBOLS=$(PKG_BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/staging/fbtft/Module.symvers \
		modules	
		
endef
$(eval $(call KernelPackage,fbtft-eb904))
