/**
******************************************************************************
*
* @file       FlightBatt.h
* @author     Joerg-D. Rothfuchs
* @brief      Implements voltage and current measurement of the flight battery
*             on the Ardupilot Mega MinimOSD using built-in ADC reference.
* @see        The GNU Public License (GPL) Version 3
*
*****************************************************************************/
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, see <http://www.gnu.org/licenses/> or write to the 
* Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


// !!! For using this, you have to solder a little bit on the MinimOSD, see the wiki !!!


#ifndef FLIGHT_BATT_H_
#define FLIGHT_BATT_H_


#define VOLTAGE_A_PIN                   7
#define VOLTAGE_B_PIN                   6
#define CURRENT_PIN                     2
#define TEMPERATURE_PIN                 1

#define MEASURE_PERIOD                  95                      // ms

#define REF_VOLTAGE_STEP                (1100.0/1024.0)             // INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
#define LOW_VOLTAGE                     22                     // filter start value for 3s LiPo in mV

#define CURRENT_VOLTAGE(x, div)         (((x)*REF_VOLTAGE_STEP)/(div))
#define CURRENT_AMPS(x, div)            (((x)*REF_VOLTAGE_STEP)-(current_offset_A/10.0))/(div)


//void flight_batt_init(void);
void flight_batt_read(void);


#endif /* FLIGHT_BATT_H_ */
