/**
******************************************************************************
*
* @file       FlightBatt.ino
* @author     Joerg-D. Rothfuchs
* @brief      Implements voltage and current measurement of the flight battery
* 	       on the Ardupilot Mega MinimOSD using built-in ADC reference.
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

#include <FastSerial.h>

#include "FlightBatt.h"

static float voltage_a = LOW_VOLTAGE * 1.05;	// battery voltage, initialized above the low voltage threshold to pre-load the filter and prevent low voltage events at startup
static float voltage_b = LOW_VOLTAGE * 1.05;	// battery voltage, initialized above the low voltage threshold to pre-load the filter and prevent low voltage events at startup 


void flight_batt_init(void)
{
	analogReference(INTERNAL);			// INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
    voltage_a = battv_A * 1.1;
    voltage_b = battv_B * 1.1; 
}

void flight_batt_read(void)
{
	static float current_amps = 0;			// battery instantaneous currrent draw [A]
	static float current_total = 0;			// totalized battery current [mAh]
	static unsigned long loopTimer = 0;
	uint16_t delta_ms;

	if (loopTimer + MEASURE_PERIOD <= millis()) {
		delta_ms	= millis() - loopTimer;
		loopTimer  	= millis();
		
		voltage_a	= CURRENT_VOLTAGE(analogRead(VOLTAGE_A_PIN), voltage_calibration_A) * 0.2 + voltage_a * 0.8;		// reads battery voltage pin
		//voltage_a	= analogRead(VOLTAGE_A_PIN);
		osd_vbat_A	= voltage_a;
		voltage_b       = CURRENT_VOLTAGE(analogRead(VOLTAGE_B_PIN), voltage_calibration_B) * 0.2 + voltage_b * 0.8;
		//voltage_b       = analogRead(VOLTAGE_B_PIN);
		osd_vbat_B	= voltage_b;

        //temperature     = CURRENT_TEMPERATURE(analogRead(TEMPERATURE_PIN), tempconv, tempconvAdd);
        temperature     = analogRead(TEMPERATURE_PIN) * 0.1 + temperature * 0.9;
                
		if (current_calibration_A > 0.0) {								// Consider Amp sensor disbled when Amp per Volt ratio is zero
			current_amps	= CURRENT_AMPS(analogRead(CURRENT_PIN), current_calibration_A) * 0.1 + current_amps * 0.9; 	// reads battery sensor current pin
			//current_amps	= analogRead(CURRENT_PIN); 	// reads battery sensor current pin
			//current_total	+= current_amps * (float) delta_ms * 0.0002778;				// .0002778 is 1/3600 (conversion to hours)
			osd_curr_A	= current_amps * 100;
			//mah_used	= current_total;
		}
	}
}
