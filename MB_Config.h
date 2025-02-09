#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

//uncomment to use CRC lookup table (faster but takes more flash)
#define _MB_USE_CRC_LOOKUP
//uncomment to use dynamic memory alloc to allocate the TX and RX buffers and to allocate the modbus register memory
//#define _MB_USE_MALLOC
//uncomment to enable the use of tickless operation (where there is no systick counter)
#define _MB_TICKLESS

#endif