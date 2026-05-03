#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

//uncomment to use CRC lookup table (faster but takes more flash)
#define _MB_USE_CRC_LOOKUP
//uncomment to use dynamic memory alloc to allocate the TX and RX buffers and to allocate the modbus register memory
//#define _MB_USE_MALLOC
//uncomment to enable the use of tickless operation (where there is no systick counter)
#define _MB_TICKLESS

//uncomment when the receiver gets the incoming bytes as full packets instead of byte-by-byte
//ex. with a controller that has Rx_Idle_Interrupt, this feature needs to be enabled
#define _MB_FULL_FRAME_RX

/**this defines the inter frame delay:
this is used by the engine to define the time it needs to wait in case communication with another slave is ongoing
 not to try to interpret the incoming data as da aaddressed to this particular slave
this number needs to be defined in either ms (for tick based op) or ticks (in tickless operation)
this number also depends on the baud rate, it should be in the size of 3.5chars (at least)
 * for 9600: 3.65ms
 * 19200 : 1.82ms
 * > 19200 (can be <=1ms)
 * for backward compatibility reasons, this number shall be fed to the Rx_Silent_Interval
 */
#define _MB_InterFrame_Delay 1

#endif