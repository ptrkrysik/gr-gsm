#ifndef GSM_ENDIAN_H
#define GSM_ENDIAN_H

#if defined(__linux__)
#  include <endian.h>
#elif defined(__APPLE__)
#  include <libkern/OSByteOrder.h>

#  define htobe16(x) OSSwapHostToBigInt16(x)
#  define htobe32(x) OSSwapHostToBigInt32(x)

#  define be16toh(x) OSSwapBigToHostInt16(x)
#  define be32toh(x) OSSwapBigToHostInt32(x)
#endif

#endif /* GSM_ENDIAN_H */
