#ifndef CONFIG_H
#define CONFIG_H

// Network Settings
#define WIFI_SSID       "Python_Hotspot"
#define WIFI_PASS       "password123"
#define SERVER_IP       "192.168.4.1"
#define SERVER_PORT     5555

// Hardware Pins
#define FLASH_ACK D5
#define FLASH_CONF D8

// Server commands
#define CMD_DOWN 1
#define CMD_CONFIG 2
#define CMD_FAIL 0

// File Paths
#define LOG_FILE        "/logs.txt"
#define CONFIG_FILE     "/config.json"

#endif