/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define CH1_AD (BSP_IO_PORT_00_PIN_00)
#define CH2_AD (BSP_IO_PORT_00_PIN_01)
#define SW_TIME1 (BSP_IO_PORT_00_PIN_02)
#define EN (BSP_IO_PORT_00_PIN_13)
#define IO_0 (BSP_IO_PORT_00_PIN_14)
#define SW_TEMPDN (BSP_IO_PORT_00_PIN_15)
#define RXD0 (BSP_IO_PORT_01_PIN_00)
#define TXD0 (BSP_IO_PORT_01_PIN_01)
#define CH1_SIN (BSP_IO_PORT_01_PIN_02)
#define CH2_SIN (BSP_IO_PORT_01_PIN_03)
#define SW_SELECT (BSP_IO_PORT_01_PIN_04)
#define COM3 (BSP_IO_PORT_01_PIN_11)
#define SLEEP (BSP_IO_PORT_01_PIN_12)
#define MD (BSP_IO_PORT_02_PIN_01)
#define SW_PWR (BSP_IO_PORT_02_PIN_06)
#define CH1_OUT1 (BSP_IO_PORT_02_PIN_07)
#define CH1_OUT2 (BSP_IO_PORT_02_PIN_08)
#define COM1 (BSP_IO_PORT_02_PIN_12)
#define COM2 (BSP_IO_PORT_02_PIN_13)
#define FND_A (BSP_IO_PORT_03_PIN_01)
#define FND_B (BSP_IO_PORT_03_PIN_02)
#define FND_C (BSP_IO_PORT_04_PIN_00)
#define SW_TIME2 (BSP_IO_PORT_04_PIN_01)
#define FND_D (BSP_IO_PORT_04_PIN_07)
#define FND_E (BSP_IO_PORT_04_PIN_08)
#define SW_TEMPUP (BSP_IO_PORT_04_PIN_09)
#define FND_F (BSP_IO_PORT_05_PIN_00)
#define FND_G (BSP_IO_PORT_09_PIN_13)
#define CH2_OUT1 (BSP_IO_PORT_09_PIN_14)
#define CH2_OUT2 (BSP_IO_PORT_09_PIN_15)
extern const ioport_cfg_t g_bsp_pin_cfg; /* R7FA2E1A73CNE.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* BSP_PIN_CFG_H_ */
