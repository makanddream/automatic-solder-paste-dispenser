#pragma once
/**
 * Configuration.h
 * Define here your default pre settings for the ASPD100
 * 
 */

//===========================================================================
//============================= Default Settings ============================
//===========================================================================
/**
 * Default configuration
 */
#define FIRST_START          1        // Default TODO
#define DEFAULT_MODE         1        // Default mode is automatic solder paste dispenser

/**
 * Default screen settings
 */
#define CONSTRAST_VALUE          50        // Default constrast value is 50%

/**
 * How many seconds/minutes we wait until going to sleep/shutdown.
 * Values -> SLEEP_TIME * 10; i.e. 5*10 = 50 Seconds! 
 */
#define SLEEP_TIME                5         // x10 Seconds
#define SHUTDOWN_TIME            10         // Minutes

/**
 * Default configuration for the automatic solder paste dispenser mode
 */
#define FIRST_START          1        // Default constrast value is 50%

/**
 * Default configuration for the vacuum pick-up mode
 */
#define FIRST_START          1        // Default constrast value is 50%
