#ifndef SRC_COLORSENSORMACROS_H_
#define SRC_COLORSENSORMACROS_H_
#define TCS_ADDRESS          (0x29)
#define TCS_COMMAND_BIT      (0x80)
#define TCS_ENABLE           (0x00)
#define TCS_ENABLE_AIEN      (0x10)    /* RGBC Interrupt Enable */
#define TCS_ENABLE_WEN       (0x08)    /* Wait enable - Writing 1 activates the wait timer */
#define TCS_ENABLE_AEN       (0x02)    /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS_ENABLE_PON       (0x01)    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */
#define TCS_ATIME            (0x01)    /* Integration time */
#define TCS_WTIME            (0x03)    /* Wait time (if TCS_ENABLE_WEN is asserted) */
#define TCS_WTIME_2_4MS      (0xFF)    /* WLONG0 = 2.4ms   WLONG1 = 0.029s */
#define TCS_WTIME_204MS      (0xAB)    /* WLONG0 = 204ms   WLONG1 = 2.45s  */
#define TCS_WTIME_614MS      (0x00)    /* WLONG0 = 614ms   WLONG1 = 7.4s   */
#define TCS_AILTL            (0x04)    /* Clear channel lower interrupt threshold */
#define TCS_AILTH            (0x05)
#define TCS_AIHTL            (0x06)    /* Clear channel upper interrupt threshold */
#define TCS_AIHTH            (0x07)
#define TCS_PERS             (0x0C)    /* Persistence register - basic SW filtering mechanism for interrupts */
#define TCS_PERS_NONE        (0b0000)  /* Every RGBC cycle generates an interrupt                                */
#define TCS_PERS_1_CYCLE     (0b0001)  /* 1 clean channel value outside threshold range generates an interrupt   */
#define TCS_PERS_2_CYCLE     (0b0010)  /* 2 clean channel values outside threshold range generates an interrupt  */
#define TCS_PERS_3_CYCLE     (0b0011)  /* 3 clean channel values outside threshold range generates an interrupt  */
#define TCS_PERS_5_CYCLE     (0b0100)  /* 5 clean channel values outside threshold range generates an interrupt  */
#define TCS_PERS_10_CYCLE    (0b0101)  /* 10 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_15_CYCLE    (0b0110)  /* 15 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_20_CYCLE    (0b0111)  /* 20 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_25_CYCLE    (0b1000)  /* 25 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_30_CYCLE    (0b1001)  /* 30 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_35_CYCLE    (0b1010)  /* 35 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_40_CYCLE    (0b1011)  /* 40 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_45_CYCLE    (0b1100)  /* 45 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_50_CYCLE    (0b1101)  /* 50 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_55_CYCLE    (0b1110)  /* 55 clean channel values outside threshold range generates an interrupt */
#define TCS_PERS_60_CYCLE    (0b1111)  /* 60 clean channel values outside threshold range generates an interrupt */
#define TCS_CONFIG           (0x0D)
#define TCS_CONFIG_WLONG     (0x02)    /* Choose between short and long (12x) wait times via TCS_WTIME */
#define TCS_CONTROL          (0x0F)    /* Set the gain level for the sensor */
#define TCS_ID               (0x12)    /* 0x44 = TCS34721/TCS, 0x4D = TCS34723/TCS34727 */
#define TCS_STATUS           (0x13)
#define TCS_STATUS_AINT      (0x10)    /* RGBC Clean channel interrupt */
#define TCS_STATUS_AVALID    (0x01)    /* Indicates that the RGBC channels have completed an integration cycle */
#define TCS_CDATAL           (0x14)    /* Clear channel data */
#define TCS_CDATAH           (0x15)
#define TCS_RDATAL           (0x16)    /* Red channel data */
#define TCS_RDATAH           (0x17)
#define TCS_GDATAL           (0x18)    /* Green channel data */
#define TCS_GDATAH           (0x19)
#define TCS_BDATAL           (0x1A)    /* Blue channel data */
#define TCS_BDATAH           (0x1B)

typedef enum
{
  TCS34725_INTEGRATIONTIME_2_4MS  = 0xFF,   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
  TCS34725_INTEGRATIONTIME_24MS   = 0xF6,   /**<  24ms  - 10 cycles  - Max Count: 10240 */
  TCS34725_INTEGRATIONTIME_50MS   = 0xEB,   /**<  50ms  - 20 cycles  - Max Count: 20480 */
  TCS34725_INTEGRATIONTIME_101MS  = 0xD5,   /**<  101ms - 42 cycles  - Max Count: 43008 */
  TCS34725_INTEGRATIONTIME_154MS  = 0xC0,   /**<  154ms - 64 cycles  - Max Count: 65535 */
  TCS34725_INTEGRATIONTIME_700MS  = 0x00    /**<  700ms - 256 cycles - Max Count: 65535 */
}
tcs34725IntegrationTime_t;

typedef enum
{
  TCS34725_GAIN_1X                = 0x00,   /**<  No gain  */
  TCS34725_GAIN_4X                = 0x01,   /**<  4x gain  */
  TCS34725_GAIN_16X               = 0x02,   /**<  16x gain */
  TCS34725_GAIN_60X               = 0x03    /**<  60x gain */
}
tcs34725Gain_t;

#endif /* SRC_COLORSENSORMACROS_H_ */
