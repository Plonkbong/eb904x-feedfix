/*
 * FB driver for the ILI9341 LCD display controller
 *
 * This display uses 9-bit SPI: Data/Command bit + 8 data bits
 * For platforms that doesn't support 9-bit, the driver is capable
 * of emulating this using 8-bit transfer.
 * This is done by transferring eight 9-bit words in 9 bytes.
 *
 * Copyright (C) 2013 Christian Vogelgsang
 * Based on adafruit22fb.c by Noralf Tronnes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <video/mipi_display.h>

#include "fbtft.h"

#define	LCD_CMD_MMAP_ADDR	0xB6000002
#define	LCD_DAT_MMAP_ADDR	0xB6000000
#define	LCD_CMD_MASK		0x0
#define	LCD_DAT_MASK		0x0
#define	LCD_VAL_SHIFT		8
#define	LCD_SET_CMD( val )	\
			do { \
				lcd_DelayNs( 20 ); \
				*(unsigned short*)LCD_CMD_MMAP_ADDR = ( 0x0 | LCD_CMD_MASK ); \
				lcd_DelayNs( 20 ); \
				*(unsigned short*)LCD_CMD_MMAP_ADDR = ( ( (unsigned short)(val) << LCD_VAL_SHIFT ) | LCD_CMD_MASK ); \
			} while ( 0 )
#define	LCD_SET_DAT( val )	\
			do { \
				lcd_DelayNs( 20 ); \
				*(unsigned short*)LCD_DAT_MMAP_ADDR = ( ( ((unsigned short)(val)&0xff) << LCD_VAL_SHIFT ) | LCD_DAT_MASK ); \
			} while ( 0 )
#define	LCD_GET_DAT( val )	\
			do { \
				unsigned short	__tmp__h, __tmp__l; \
				lcd_DelayNs( 20 ); \
				__tmp__h = *(unsigned short*)LCD_DAT_MMAP_ADDR; \
				lcd_DelayNs( 20 ); \
				__tmp__l = *(unsigned short*)LCD_DAT_MMAP_ADDR; \
				(val) = ( ( ( __tmp__h >> LCD_VAL_SHIFT ) & 0xff ) << 8 ) | ( ( __tmp__l >> LCD_VAL_SHIFT ) & 0xff ); \
} while ( 0 )

#define DRVNAME		"fb_ili9341_eb904"
#define WIDTH		240
#define HEIGHT		320
#define TXBUFLEN	(4 * PAGE_SIZE)
#define DEFAULT_GAMMA	"1F 1A 18 0A 0F 06 45 87 32 0A 07 02 07 05 00\n" \
			"00 25 27 05 10 09 3A 78 4D 05 18 0D 38 3A 1F"
#define UBOOT_GAMMA	"0F 1B 17 0C 0D 08 40 A9 28 06 0D 03 10 03 00\n" \
			"00 24 28 03 12 07 3F 56 57 09 12 0C 2F 3C 0F"

void lcd_DelayNs( unsigned long nsec )
{
	ndelay(nsec);
}

void lcd_DelayUs( unsigned long usec )
{
	udelay(usec);
}

void lcd_WriteCommand( unsigned char iReg )
{
	// printk("%s: Write command 0x%04x\n",__FUNCTION__, iReg);
	LCD_SET_CMD( iReg );
}

void lcd_WriteData( unsigned short iData )
{
	// printk("%s: Write data 0x%04x\n",__FUNCTION__, iData);
	LCD_SET_DAT( iData );
}

unsigned short lcd_ReadData( void )
{
	unsigned short	iData;

	LCD_GET_DAT( iData );

	return iData;
}

unsigned short ili9341_GetControllerID( void )
{
	unsigned short	iParameter1;
	unsigned short	iParameter2;

	lcd_WriteCommand( 0xD3);

	iParameter1 = lcd_ReadData();
	iParameter2 = lcd_ReadData();

	return iParameter2;
}

int ili9341_Probe( void )
{
	unsigned short value = ili9341_GetControllerID();
	printk("%s: Probed ID4: %x\n", __FUNCTION__, value);
	return ( value == 0x9341 );
}

static int init_display_uboot(struct fbtft_par *par)
	{
	// VCI=2.8V
		ili9341_Probe();
	//************* Start Initial Sequence **********//
//		lcd_WriteCommand(0xCB);
//		lcd_WriteData(0x392C);
//		lcd_WriteData(0x0034);
//		lcd_WriteData(0x0200);
		write_reg(par, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02, 0x00);

//		lcd_WriteCommand(0xCF);
//		lcd_WriteData(0x00C1);
//		lcd_WriteData(0X3000);
		write_reg(par, 0xCF, 0x00, 0xC1, 0x30, 0x00);

//		lcd_WriteCommand(0xE8);
//		lcd_WriteData(0x8510);
//		lcd_WriteData(0x7900);
		write_reg(par, 0xE8, 0x85, 0x10, 0x79, 0x00);

//		lcd_WriteCommand(0xEA);
//		lcd_WriteData(0x0000);
		write_reg(par, 0xEA, 0x00, 0x00);

//		lcd_WriteCommand(0xED);
//		lcd_WriteData(0x6403);
//		lcd_WriteData(0X1281);
		write_reg(par, 0xED, 0x64, 0x03, 0x12, 0x81);

//		lcd_WriteCommand(0xF7);
//		lcd_WriteData(0x2000);
		write_reg(par, 0xF7, 0x20, 0x00);

//		lcd_WriteCommand(0xC0);    //Power control
//		lcd_WriteData(0x2100);   //VRH[5:0]
		write_reg(par, 0xC0, 0x21, 0x00);

//		lcd_WriteCommand(0xC1);    //Power control
//		lcd_WriteData(0x1200);   //SAP[2:0];BT[3:0]
		write_reg(par, 0xC1, 0x12, 0x00);

//		lcd_WriteCommand(0xC5);    //VCM control
//		lcd_WriteData(0x243F);
		write_reg(par, 0xC5, 0x24, 0x3F);

//		lcd_WriteCommand(0xC7);    //VCM control2
//		lcd_WriteData(0xC200);
		write_reg(par, 0xC7, 0xC2, 0x00);

//		lcd_WriteCommand(0xb1);	   // Frame rate
//		lcd_WriteData(0x0016);
		write_reg(par, 0xB1, 0x00, 0x16);

//		lcd_WriteCommand(0x36);    // Memory Access Control
//		if ( lcd_GetOrientation() == LCD_ORIENTATION_LANDSCAPE)
//		  lcd_WriteData(0x4800);//08 48
//		else
//		  lcd_WriteData(0x3800);

//		lcd_WriteCommand(0x3A);
//		lcd_WriteData(0x5500);
		write_reg(par, 0x3A, 0x55, 0x00);

//		lcd_WriteCommand(0xF2);    // 3Gamma Function Disable
//		lcd_WriteData(0x0000);
		write_reg(par, 0xF2, 0x00, 0x00);

//		lcd_WriteCommand(0x26);    //Gamma curve selected
//		lcd_WriteData(0x0100);
		write_reg(par, 0x26, 0x01, 0x00);

//		lcd_WriteCommand(0xE0);    //Set Gamma
//		lcd_WriteData(0x0F1B);
//		lcd_WriteData(0x170C);
//		lcd_WriteData(0x0D08);
//		lcd_WriteData(0x40A9);
//		lcd_WriteData(0x2806);
//		lcd_WriteData(0x0D03);
//		lcd_WriteData(0x1003);
//		lcd_WriteData(0x0000);
		write_reg(par, 0xE0,
					   0x0F, 0x1B,
					   0x17, 0x0C,
					   0x0D, 0x08,
					   0x40, 0xA9,
					   0x28, 0x06,
					   0x0D, 0x03,
					   0x10, 0x03,
					   0x00, 0x00
				 );

//		lcd_WriteCommand(0XE1);    //Set Gamma
//		lcd_WriteData(0x0024);
//		lcd_WriteData(0x2803);
//		lcd_WriteData(0x1207);
//		lcd_WriteData(0x3F56);
//		lcd_WriteData(0x5709);
//		lcd_WriteData(0x120C);
//		lcd_WriteData(0x2F3C);
//		lcd_WriteData(0x0F00);
		write_reg(par, 0xE1,
					   0x00, 0x24,
					   0x28, 0x03,
					   0x12, 0x07,
					   0x3F, 0x56,
					   0x57, 0x09,
					   0x12, 0x0C,
					   0x2F, 0x3C,
					   0x0F, 0x00
				 );

//		lcd_WriteCommand(0x11);    //Exit Sleep
		write_reg(par, 0x11);
		mdelay(120);
//		lcd_WriteCommand(0x29);    //Display on
		write_reg(par, 0x29);

		return 0;
	}

static int init_display(struct fbtft_par *par)
{
	par->fbtftops.reset(par);

	ili9341_Probe();

	/* startup sequence for MI0283QT-9A */
	write_reg(par, MIPI_DCS_SOFT_RESET);
	mdelay(5);
	write_reg(par, MIPI_DCS_SET_DISPLAY_OFF);
	/* --------------------------------------------------------- */
	write_reg(par, 0xCF, 0x00, 0x83, 0x30);
	write_reg(par, 0xED, 0x64, 0x03, 0x12, 0x81);
	write_reg(par, 0xE8, 0x85, 0x01, 0x79);
	write_reg(par, 0xCB, 0x39, 0X2C, 0x00, 0x34, 0x02);
	write_reg(par, 0xF7, 0x20);
	write_reg(par, 0xEA, 0x00, 0x00);
	/* ------------power control-------------------------------- */
//	write_reg(par, 0xC0, 0x26);
//	write_reg(par, 0xC1, 0x11);
	write_reg(par, 0xC0, 0x21); //VRH[5:0]
	write_reg(par, 0xC1, 0x12); //SAP[2:0];BT[3:0]

	/* ------------VCOM --------- */
//	write_reg(par, 0xC5, 0x35, 0x3E);
//	write_reg(par, 0xC7, 0xBE);
	write_reg(par, 0xC5, 0x24, 0x3F); //VCM control
	write_reg(par, 0xC7, 0xC2);       //VCM control2
	/* ------------memory access control------------------------ */
	write_reg(par, MIPI_DCS_SET_PIXEL_FORMAT, 0x55); /* 16bit pixel */
	/* ------------frame rate----------------------------------- */
	// write_reg(par, 0xB1, 0x00, 0x1B);
	write_reg(par, 0xB1, 0x00, 0x16); // uboot
	/* ------------Gamma---------------------------------------- */
	/* write_reg(par, 0xF2, 0x08); */ /* Gamma Function Disable */
	write_reg(par, MIPI_DCS_SET_GAMMA_CURVE, 0x01);
	/* ------------display-------------------------------------- */
	write_reg(par, 0xB7, 0x07); /* entry mode set */
	/* ------------additional values---------------------------- */
	write_reg(par, 0x13);       /* normal display mode on */
	write_reg(par, 0x38);       /* idle mode off */
	write_reg(par, 0x20);       /* inversion mode off */
	/* --------------------------------------------------------- */
	write_reg(par, 0xB6, 0x0A, 0x82, 0x27, 0x00);
	write_reg(par, MIPI_DCS_EXIT_SLEEP_MODE);
	mdelay(100);
	write_reg(par, MIPI_DCS_SET_DISPLAY_ON);
	mdelay(20);

	return 0;
}

static void set_addr_win(struct fbtft_par *par, int xs, int ys, int xe, int ye)
{
	write_reg(par, MIPI_DCS_SET_COLUMN_ADDRESS,
		  (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF);

	write_reg(par, MIPI_DCS_SET_PAGE_ADDRESS,
		  (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF);

	write_reg(par, MIPI_DCS_WRITE_MEMORY_START);
}

#define MEM_Y   BIT(7) /* MY row address order */
#define MEM_X   BIT(6) /* MX column address order */
#define MEM_V   BIT(5) /* MV row / column exchange */
#define MEM_L   BIT(4) /* ML vertical refresh order */
#define MEM_H   BIT(2) /* MH horizontal refresh order */
#define MEM_BGR (3) /* RGB-BGR Order */
static int set_var(struct fbtft_par *par)
{
	switch (par->info->var.rotate) {
	case 0:
		write_reg(par, MIPI_DCS_SET_ADDRESS_MODE,
			  MEM_X | (par->bgr << MEM_BGR));
		break;
	case 270:
		write_reg(par, MIPI_DCS_SET_ADDRESS_MODE,
			  MEM_V | MEM_L | (par->bgr << MEM_BGR));
		break;
	case 180:
		write_reg(par, MIPI_DCS_SET_ADDRESS_MODE,
			  MEM_Y | (par->bgr << MEM_BGR));
		break;
	case 90:
		write_reg(par, MIPI_DCS_SET_ADDRESS_MODE,
			  MEM_Y | MEM_X | MEM_V | (par->bgr << MEM_BGR));
		break;
	}
	return 0;
}

/*
 * Gamma string format:
 *  Positive: Par1 Par2 [...] Par15
 *  Negative: Par1 Par2 [...] Par15
 */
#define CURVE(num, idx)  curves[num * par->gamma.num_values + idx]
static int set_gamma(struct fbtft_par *par, u32 *curves)
{
	int i;
	for (i = 0; i < par->gamma.num_curves; i++)
		write_reg(par, 0xE0 + i,
			  CURVE(i, 0), CURVE(i, 1), CURVE(i, 2),
			  CURVE(i, 3), CURVE(i, 4), CURVE(i, 5),
			  CURVE(i, 6), CURVE(i, 7), CURVE(i, 8),
			  CURVE(i, 9), CURVE(i, 10), CURVE(i, 11),
			  CURVE(i, 12), CURVE(i, 13), CURVE(i, 14));
	return 0;
}

#undef CURVE

int fbtft_write_8_wr_ebu(struct fbtft_par *par, void *buf, size_t len)
{
	u8 data;
	fbtft_par_dbg_hex(DEBUG_WRITE, par, par->info->device, u8, buf, len, "%s(len=%d): ", __func__, len);

	while (len--) {
		data = *(u8 *)buf;

		/* Set data */
		lcd_WriteData(data);
		buf++;
	}

	return 0;
}

static int verify_gpios_ebu(struct fbtft_par *par)
{
	fbtft_par_dbg(DEBUG_VERIFY_GPIOS, par, "%s()\n", __func__);

	if (!par->pdev)
		return 0;

	return 0;
}

void fbtft_write_reg8_bus8_ebu(struct fbtft_par *par, int len, ...)
{
	va_list args;
	int i, ret;
	int offset = 0;
	u8 *buf = (u8 *)par->buf;

	if (unlikely(par->debug & DEBUG_WRITE_REGISTER)) {
		va_start(args, len);
		for (i = 0; i < len; i++) {
			buf[i] = (u8)va_arg(args, unsigned int);
		}
		va_end(args);
		fbtft_par_dbg_hex(DEBUG_WRITE_REGISTER, par, par->info->device, u8, buf, len, "%s: ", __func__);
	}

	va_start(args, len);

	*buf = ((u8)va_arg(args, unsigned int));
	lcd_WriteCommand((unsigned char)*buf);
	len--;

	if (len) {
		i = len;
		while (i--) {
			*buf++ = ((u8)va_arg(args, unsigned int));
		}
		ret = par->fbtftops.write(par, par->buf, len * (sizeof(u8) + offset));
		if (ret < 0) {
			va_end(args);
			dev_err(par->info->device, "%s: write() failed and returned %d\n", __func__, ret);
			return;
		}
	}
	va_end(args);
}


static struct fbtft_display display = {
	.regwidth = 8,
	.width = WIDTH,
	.height = HEIGHT,
	.txbuflen = TXBUFLEN,
	.gamma_num = 2,
	.gamma_len = 15,
	.gamma = UBOOT_GAMMA,
	.fbtftops = {
		.init_display = init_display,
		.set_addr_win = set_addr_win,
		.set_var = set_var,
		.set_gamma = set_gamma,
		.verify_gpios = verify_gpios_ebu,
		.write = fbtft_write_8_wr_ebu,
		.write_register = fbtft_write_reg8_bus8_ebu,
	},
};

FBTFT_REGISTER_DRIVER(DRVNAME, "ilitek,ili9341_eb904", &display);

MODULE_ALIAS("platform:" DRVNAME);
MODULE_ALIAS("platform:ili9341_eb904");

MODULE_DESCRIPTION("FB driver for the ILI9341 LCD display controller");
MODULE_AUTHOR("Christian Vogelgsang");
MODULE_LICENSE("GPL");
