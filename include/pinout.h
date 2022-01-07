#ifndef __pinout_h
#define __pinout_h

// Control Panel

#define KEY_LED_EN_PIN  20
#define KEY_1_PIN       3
#define KEY_2_PIN       2
#define KEY_3_PIN       1
#define KEY_4_PIN       0

#define STATUS_R_PIN    22
#define STATUS_G_PIN    23


// Serial & Parallel Interface

#define VSER_TXD_PIN 13
#define VSER_RXD_PIN 14
#define VSER_CTS_PIN 15

#define SD_CS_PIN 12


// IO Expanders

#define IO1_VSE_12V       0
#define IO1_VSE_5V        1
#define IO1_VSE_3V3       2
#define IO1_EN_12V        3
#define IO1_PRES_VIN1     4
#define IO1_PRES_VIN2     5
#define IO1_TEST_USB_DP   6
#define IO1_TEST_USB_DM   7

#define IO1_TEST_MOT_PWM  8
#define IO1_TEST_RJ_SDAP  9
#define IO1_TEST_RJ_SDAM  10
#define IO1_TEST_RJ_SCLP  11
#define IO1_TEST_RJ_SCLM  12

#define IO2_TEST_A        0
#define IO2_TEST_OLED_CS  1
#define IO2_TEST_OLED_DC  2
#define IO2_TEST_OLED_RST 3
#define IO2_TEST_MISO     4
#define IO2_TEST_SCK      5
#define IO2_TEST_MOSI     6
#define IO2_TEST_SD_CS    7

#define IO2_TEST_B        8
#define IO2_TEST_BL       9
#define IO2_TEST_SW       10
#define IO2_TEST_GR       11
#define IO2_TEST_RD       12
#define IO2_TEST_K3       13
#define IO2_TEST_K2       14
#define IO2_TEST_K1       15


// Test Inputs

#define CURR_SENSE_PIN  A0
#define VS_RHIGH_PIN    A1
#define VS_12V_PIN      A2
#define VS_5V_PIN       A3
#define VS_3V3_PIN      A4
#define TEST_V_PRES_PIN A5
#define VS_PRES_PIN     A6


#endif
