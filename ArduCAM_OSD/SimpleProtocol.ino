#include <FastSerial.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "SimpleSerial.h"

#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}
void(* resetFunc) (void) = 0; //declare reset function @ address 0

// true when we have received at least 1 MAVLink packet
static bool mavlink_active;
static uint8_t crlf_count = 0;

static int packet_drops = 0;
static int parse_error = 0;

void request_mavlink_rates()
{
}

void read_mavlink(){
	simple_message_t msg;
	
	//apm_mav_type = 1;  // plane
	//motor_armed = 1;
	//osd_battery_remaining_A = 100;

	//grabing data 
	while(Serial.available() > 0) { 
		uint8_t c = Serial.read();

		/* allow CLI to be started by hitting enter 3 times, if no
		heartbeat packets have been received */
		if (mavlink_active == 0 && millis() < 20000 && millis() > 5000) {
			if (c == '\n' || c == '\r') {
				crlf_count++;
			} else {
				crlf_count = 0;
			}
			if (crlf_count == 3) {
				uploadFont();
			}
		}

		if (simple_parse(c, &msg))
		{
			mavlink_active = 1;
			mavbeat = 1;  
			lastMAVBeat = millis(); 
			
			switch(msg.msgid) {
			case ap_msg:
				osd_mode = msg.ap_mode.mode;
				osd_nav_mode = 0;        
				break;
			case gps_msg:
				osd_lat = msg.gps_msg.lat / 10000000.0f;
				osd_lon = msg.gps_msg.lon / 10000000.0f;
				osd_fix_type = msg.gps_msg.fix_type;
				osd_alt = msg.gps_msg.alt;
				osd_satellites_visible = msg.gps_msg.satellites_visible;
				osd_groundspeed = msg.gps_msg.groundspeed;
				osd_airspeed = msg.gps_msg.airspeed;
				osd_heading = msg.gps_msg.heading;
				//Serial.println("===");
				break;
			case horizon_msg:
				osd_pitch = msg.horizon_msg.pitch;
				osd_roll = msg.horizon_msg.roll;
				//osd_yaw = 0;
				break;
			case mission_msg:
				wp_target_bearing = msg.mission_msg.wp_target_bearing;
				wp_dist = msg.mission_msg.wp_dist;
				wp_number = msg.mission_msg.wp_number;
				break;
			case channels_msg:
				osd_throttle = (msg.channels_msg.channel[0] - 1000) / 10;
				chan5_raw = msg.channels_msg.channel[1];
				chan6_raw = msg.channels_msg.channel[2];
				chan7_raw = msg.channels_msg.channel[3];
				chan8_raw = msg.channels_msg.channel[4];
				osd_rssi = msg.channels_msg.rssi;
				break;
			case power_msg:
#ifndef FLIGHT_BATT_ON_MINIMOSD
				osd_vbat_A = msg.power_msg.vbat_A / 100.0f; //Battery voltage, in millivolts (1 = 1 millivolt)
				//osd_curr_A = 0;
                                //osd_vbat_B = 0;
				//osd_battery_remaining_A = 0;
				//Serial.println("====");
#endif
                                //temperature = msg.power_msg.vbat_A * 10;
				break;
			case climb_msg:
				osd_climb = msg.climb_msg.climb / 10;
				//Serial.println("=====");
				break;
                        case reset_msg:
                                //Reset_AVR();
                                delay(500);
                                resetFunc();  //call reset
                                break;
			default:
				//Do nothing
				break;
			}
		}

	}
}
