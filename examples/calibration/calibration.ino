/*******************************************************************************************************************************************************************************************
*  Adafruit BNO055 Sensor Calibration sample
*  Refer to http://www.adafruit.com/datasheets/BST_BNO055_DS000_12.pdf
*  Section 3.10 on how to calibrate your sensor:
*   -Run this sketch
*   -To calibrate the Gyroscope, just let the sensor sit for a couple of seconds until GYR Calibration shows 1
*   -To calibrate the Magnetometer, move your sensor in random figure 8 patterns until MAG Calibration shows 1
*   -If you want to calibrate the acceleration sensor, move the sensor to 6 stable positions slowly, 3 of those should be in the XY/XZ/YZ plane
*   -Once you are happy with the readings you get, copy the c_data = .... line from serial.console to the setup() of your sketch and write it to the sensor with "bno.setCalibData(c_data);"
********************************************************************************************************************************************************************************************/

#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup()
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Calibration"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin(bno.OPERATION_MODE_NDOF)) //if you want to calibrate using another mode, set it here. OPERATION_MODE_COMPASS for a precise tilt compensated compass (Section 3.3.2 / 3.3.3)
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  
  /* You will need the next two commented lines in your own sketch, _immediatly_ after bno.begin() to use a predefined calibration
  const byte c_data[22] = {0, 0, 0, 0, 0, 0, 172, 250, 112, 255, 52, 253, 0, 0, 253, 255, 255, 255, 232, 3, 240, 2}; //replace this line with the serial output of this sketch
  bno.setCalibData(c_data);*/
  
  delay(1000);
  bno.setExtCrystalUse(true);
}

void loop()
{
  sensors_event_t event;
  bno.getEvent(&event);

  /* Prints the euler angles for reference */
  Serial.print("X: "); //"heading"
  Serial.print(event.orientation.x, 4);
  Serial.print(" Y: ");
  Serial.print(event.orientation.y, 4);
  Serial.print(" Z: ");
  Serial.print(event.orientation.z, 4);
  Serial.println();

  /* Shows the calibration states for all 3 sensors + system. Optimally, in NDOF mode you want all these to read "1" 
    before you assume good calibration (Note: Some sensors are off in other modes)*/
  bool bMAG = bno.getCalibState(bno.CALIB_STAT_MAG);
  Serial.print("MAG Calibration: ");
  Serial.println(bMAG);
  bool bGYR = bno.getCalibState(bno.CALIB_STAT_GYR);
  Serial.print("GYR Calibration: ");
  Serial.println(bGYR);
  bool bACC = bno.getCalibState(bno.CALIB_STAT_ACC);
  Serial.print("ACC Calibration: ");
  Serial.println(bACC);
  bool b = bno.getCalibState(bno.CALIB_STAT_SYS);
  Serial.print("SYS Calibration: ");
  Serial.println(b);

  byte buffer[22];
  bno.getCalibData(buffer);
  /* Prints the actual line to paste into your sketch (see setup() above) */
  Serial.print("byte c_data[22] = {");
  for (int i = 0; i < 22; i++)
  {
    Serial.print(buffer[i]); 
    if(i!=21) Serial.print(", ");
  }
  Serial.println("};");
  
  delay(500);
}