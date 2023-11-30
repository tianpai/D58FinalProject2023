/* decap (decapsulation)
 *  decapsulates packets whereelse it is needed
 */
#include "decap.h"
#include "protocol.h"
#include <stdint.h>

uint8_t *packet_decapsulate(uint8_t packet) {
	return packet + sizeof(gre_hdr_t);
}

/* Refer to decap.h for function details */
