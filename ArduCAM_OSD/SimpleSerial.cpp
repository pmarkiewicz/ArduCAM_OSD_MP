#include <FastSerial.h>
#include <Arduino.h>
#include <inttypes.h>
#include "SimpleSerial.h"

static char msg_buffer[128];
static char msg_err[] = "-1";  // in case of msg parse error -1 should be easily visible
const char terminal = '\n';

char* pos1;
char* pos2;

static const char* first()
{
    pos1 = strchr(msg_buffer, ',') + 1;
    pos2 = strchr(pos1, ',');
    
    if (pos1 == NULL || pos2 == NULL)
    {
        return msg_err;
    }
    
    *pos2 = 0;
    
    return pos1;
}

static const char* next()
{
    if (pos2 == NULL)
    {
        return msg_err;
    }
    
    pos1 = pos2 + 1;
    pos2 = strchr(pos1, ',');
    if (pos2 == NULL)
    {
        return msg_err;
    }
    
    *pos2 = 0;
    
    return pos1;
}

static void parse_ap_msg(simple_message_t* msg)
{
    msg->msgid = ap_msg;
    msg->ap_mode.mode = atoi(first());
}

static void parse_power_msg(simple_message_t* msg)
{
    //Serial.print("pp ");
    msg->msgid = power_msg;
    msg->power_msg.vbat_A = atoi(first());
    //Serial.println("ppp");
}

static void parse_climb_msg(simple_message_t* msg)
{
    //Serial.print("cc ");
    msg->msgid = climb_msg;
    msg->climb_msg.climb = atoi(first());
    //Serial.println("ccc");
}

static void parse_horizon_msg(simple_message_t* msg)
{
    msg->msgid = horizon_msg;
    msg->horizon_msg.roll = atoi(first());
    msg->horizon_msg.pitch = atoi(next());
}

static void parse_gps_msg(simple_message_t* msg)
{
    msg->msgid = gps_msg;
    
    // $A,lat,lng,numSV,alt,speed,course,fix,<CRLF>
    msg->gps_msg.lat = atol(first());
    msg->gps_msg.lon = atol(next());
    msg->gps_msg.satellites_visible = (uint8_t)atoi(next());
    msg->gps_msg.alt = atol(next());
    msg->gps_msg.groundspeed = (uint8_t)atoi(next());
    msg->gps_msg.airspeed = (uint8_t)atoi(next());
    msg->gps_msg.heading = atoi(next());
    msg->gps_msg.fix_type = (uint8_t)atoi(next());
    //Serial.println("----");
}

static void parse_mission_msg(simple_message_t* msg)
{
    msg->msgid = mission_msg;
    
    msg->mission_msg.wp_target_bearing = atoi(first());
    msg->mission_msg.wp_dist = atol(next());
    msg->mission_msg.wp_number = atoi(next());
}

static void parse_channels_msg(simple_message_t* msg)
{
//    memset(msg, 0xFF, sizeof(msg));
    msg->msgid = channels_msg;
    
    msg->channels_msg.channel[0] = atoi(first());
    for (int i = 1; i < 5; ++i)
    {
        msg->channels_msg.channel[i] = atoi(next());
    }
    
    msg->channels_msg.rssi = atoi(next());
}

static void parse_reset(simple_message_t* msg)
{
    msg->msgid = reset_msg;
}

bool simple_parse(uint8_t c, simple_message_t* msg)
{
    if (c != '$')
    {
        return false;
    }
    
    uint8_t length = Serial.readBytesUntil(terminal, msg_buffer, sizeof(msg_buffer));  
    if (length == 0 || msg_buffer[length -1] != '\r')
    {
        return false;
    }
        
    msg_buffer[length -1] = 0;
    //Serial.print("|");
    //Serial.print(msg_buffer);
    //Serial.println("|");
    
    switch (msg_buffer[0])
    {
    case 'P':
        parse_ap_msg(msg);
        break;
    case 'I':
        parse_horizon_msg(msg);
        break;
    case 'A':
        parse_gps_msg(msg);
        break;
    case 'M':
        parse_mission_msg(msg);
        break;
    case 'C':
        parse_channels_msg(msg);
        break;
    case 'U':
        parse_power_msg(msg);
        break;
    case 'Z':
        parse_climb_msg(msg);
        break;
    case 'R':
        parse_reset(msg);
        break;
    default:
        return false;
    }
    
    return true;
}

