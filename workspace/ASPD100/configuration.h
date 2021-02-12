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
 * Default gui settings
 */
#define DESCRIPTION_SCROLL_SPEED  0         // 0: Slow 1: Fast - default to slow

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

/*
 * Footprints configuration
 */
#define PUSH_TIME_0201		500
#define RETRACT_TIME_0201	490

#define PUSH_TIME_0402		600
#define RETRACT_TIME_0402	590

#define PUSH_TIME_0603		800
#define RETRACT_TIME_0603	790

#define PUSH_TIME_0805		900
#define RETRACT_TIME_0805	890
