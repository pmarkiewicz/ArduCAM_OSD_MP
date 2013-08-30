#ifndef _SIMPLESERIAL__H
#define _SIMPLESERIAL__H

/***
	From minimOSD code
        if (osd_mode == 0) mode_str = "manu"; //Manual
        else if (osd_mode == 1) mode_str = "circ"; //CIRCLE
        else if (osd_mode == 2) mode_str = "stab"; //Stabilize
        else if (osd_mode == 5) mode_str = "fbwa"; //FLY_BY_WIRE_A
        else if (osd_mode == 6) mode_str = "fbwb"; //FLY_BY_WIRE_B
        else if (osd_mode == 10) mode_str = "auto"; //AUTO
        else if (osd_mode == 11) mode_str = "retl"; //Return to Launch
        else if (osd_mode == 12) mode_str = "loit"; //Loiter
        else if (osd_mode == 15) mode_str = "guid"; //GUIDED 
***/

enum message_type
{
    ap_msg = 0,
    gps_msg = 1,
    horizon_msg = 2,
    mission_msg = 3,
    channels_msg = 4,
    power_msg = 5,
    climb_msg = 6,
    reset_msg
};

enum osd_mode
{
	mode_manual = 0,
	mode_stable = 2,
	mode_waypoint = 5,
	mode_rth = 11,
	mode_rth_hold = 15
};

typedef struct _ap_msg_t
{
    uint8_t mode;
};

typedef struct _gps_msg_t
{
    int32_t lat; ///< Latitude in 1E7 degrees
    int32_t lon; ///< Longitude in 1E7 degrees
    int32_t alt; ///< Altitude in 1E3 meters (millimeters) above MSL
    uint8_t fix_type; ///< 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
    uint8_t satellites_visible; ///< Number of satellites visible. If unknown, set to 255
    uint16_t groundspeed;
    uint16_t airspeed;
    uint16_t heading;
};

typedef struct _horizon_msg_t
{
    uint16_t pitch;
    uint16_t roll;
};

typedef struct _mission_msg_t
{
    uint16_t wp_target_bearing;
    uint32_t wp_dist;
    uint8_t wp_number;
};

typedef struct _channels_msg_t
{
    uint16_t channel[8];
    uint8_t rssi;
};

typedef struct _power_msg_t
{
    uint16_t vbat_A;
};

typedef struct _climb_msg_t
{
    int16_t climb;
};

typedef struct _reset_msg_t
{
};

struct simple_message_t
{
	int msgid;
	union
	{
	    _ap_msg_t ap_mode;
            _gps_msg_t gps_msg;
            _horizon_msg_t horizon_msg;
            _mission_msg_t mission_msg;
            _channels_msg_t channels_msg;
            _power_msg_t power_msg;
            _climb_msg_t climb_msg;
            _reset_msg_t reset_msg;
	};
};

bool simple_parse(uint8_t c, simple_message_t* msg);

#endif

