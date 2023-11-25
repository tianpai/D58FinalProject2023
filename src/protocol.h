/* Created by Tianpai Zhang
 * Date: Nov 13, 2023
 */

/* Every function or struct should be defined within 
 * the namespace of protocol.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* write anything after this line */

#ifdef _LINUX_ 
#include <stdint.h>
#endif  /* -- _LINUX_ -- */

/* Using standard GRE packet header; made C field from 1-bit to 16 bit,
 * removed reserved 0s and version field as they are 15 bits altogether
 * and is always set to 0, according to the standard. Decided to put
 * the 15 bit into good use and made C from unsigned type to uint16_t.
 * Also removed reserved 1 as it is not used for our purpose.
 */
struct gre_hdr {
  uint16_t c;
  uint16_t protocol;
  uint16_t checksum;
} __attribute__ ((packed)) ;

/* anything should before this line */
#endif /* -- PROTOCOL_H -- */
