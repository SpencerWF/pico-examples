#include "hardware/i2c.h"
#include <pico/binary_info.h>

#ifndef SI7021_DEFINED_H
#define SI7021_DEFINED_H
// Includes
// #include si7021_picopi.

#define SI7021_DEFAULT_ADDRESS 0x40
#define SI7021_I2C_BAUDRATE (100*1000)
#define SI7021_MEASRH_HOLD_CMD                                                 \
  0xE5 /**< Measure Relative Humidity, Hold Master Mode */
#define SI7021_MEASRH_NOHOLD_CMD                                               \
  0xF5 /**< Measure Relative Humidity, No Hold Master Mode */
#define SI7021_MEASTEMP_HOLD_CMD                                               \
  0xE3 /**< Measure Temperature, Hold Master Mode */
#define SI7021_MEASTEMP_NOHOLD_CMD                                             \
  0xF3 /**< Measure Temperature, No Hold Master Mode */
#define SI7021_READPREVTEMP_CMD                                                \
  0xE0 /**< Read Temperature Value from Previous RH Measurement */
#define SI7021_RESET_CMD 0xFE           /**< Reset Command */
#define SI7021_WRITERHT_REG_CMD 0xE6    /**< Write RH/T User Register 1 */
#define SI7021_READRHT_REG_CMD 0xE7     /**< Read RH/T User Register 1 */
#define SI7021_WRITEHEATER_REG_CMD 0x51 /**< Write Heater Control Register */
#define SI7021_READHEATER_REG_CMD 0x11  /**< Read Heater Control Register */
#define SI7021_REG_HTRE_BIT 0x02        /**< Control Register Heater Bit */
#define SI7021_ID1_CMD 0xFA0F           /**< Read Electronic ID 1st Byte */
#define SI7021_ID2_CMD 0xFCC9           /**< Read Electronic ID 2nd Byte */
#define SI7021_FIRMVERS_CMD 0x84B8      /**< Read Firmware Revision */

#define SI7021_REV_1 0xff /**< Sensor revision 1 */
#define SI7021_REV_2 0x20 /**< Sensor revision 2 */

/** An enum to represent sensor types **/
enum si_sensorType {
  SI_Engineering_Samples,
  SI_7013,
  SI_7020,
  SI_7021,
  SI_UNKNOWN,
};

/** An enum to represent the sensor heater heat levels **/
enum si_heatLevel {
  SI_HEATLEVEL_LOWEST = 0x00,
  SI_HEATLEVEL_LOW = 0x01,
  SI_HEATLEVEL_MEDIUM = 0x02,
  SI_HEATLEVEL_HIGH = 0x04,
  SI_HEATLEVEL_HIGHER = 0x08,
  SI_HEATLEVEL_HIGHEST = 0x0F,
};

struct Pico_Si7021{
  i2c_inst_t *inst;
  int i2c_addr;
  uint8_t sernum_a;
  uint8_t sernum_b;
  uint8_t data_out[2];
  uint8_t data_in;
  uint8_t revision;
};

uint8_t si7021_init(struct Pico_Si7021 *self, i2c_inst_t *i2c_i);
uint16_t si7021_read_humidity(struct Pico_Si7021* self);
float si7021_read_temperature(struct Pico_Si7021 *self);
static void si7021_read_revision(struct Pico_Si7021* self);


// /*!
//  *  @brief  Class that stores state and functions for interacting with
//  *          Si7021 Sensor
//  */
// class Adafruit_Si7021 {
// public:
//   Adafruit_Si7021(TwoWire *theWire = &Wire);
//   bool begin();

//   float readTemperature();
//   void reset();
//   void readSerialNumber();
//   float readHumidity();

//   /*!
//    *  @brief  Enable/Disable the sensor heater
//    *  @param h True to enable the heater, False to disable it.
//    */
//   void heater(bool h);
//   bool isHeaterEnabled();

//   /*!
//    *  @brief  Set the sensor heater heat level
//    *  @param level is a si_heatLevel that represents the heat level
//    */
//   void setHeatLevel(uint8_t level);

//   /*!
//    *  @brief  Returns sensor revision established during init
//    *  @return model value
//    */
//   uint8_t getRevision() { return _revision; };
//   si_sensorType getModel();

//   uint32_t sernum_a; /**< Serialnum A */
//   uint32_t sernum_b; /**< Serialnum B */

// private:
//   void _readRevision();
//   si_sensorType _model;
//   uint8_t _revision;
//   uint8_t _readRegister8(uint8_t reg);
//   uint16_t _readRegister16(uint8_t reg);
//   void _writeRegister8(uint8_t reg, uint8_t value);

//   int8_t _i2caddr;
//   TwoWire *_wire;
//   const static int _TRANSACTION_TIMEOUT = 100; // Wire NAK/Busy timeout in ms
// };
#endif