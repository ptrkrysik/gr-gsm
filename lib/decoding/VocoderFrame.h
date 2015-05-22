#ifndef _VOCODERFRAME_H
#define _VOCODERFRAME_H

#include "BitVector.h" 
//#include "GSMCommon.h"

class VocoderFrame : public BitVector {

        public:

        VocoderFrame()
                :BitVector(264)
        { fillField(0,0x0d,4); }

        /** Construct by unpacking a char[33]. */
        VocoderFrame(const unsigned char *src)
                :BitVector(264)
        { unpack(src); }

        BitVector payload() { return tail(4); }
        const BitVector payload() const { return tail(4); }

};

class VocoderAMRFrame : public BitVector {

        public:

        VocoderAMRFrame()
                :BitVector(244+8)
        { fillField(0,0x3c,8); /* AMR-NB 12.2 */ }

        /** Construct by unpacking a char[32]. */
        VocoderAMRFrame(const unsigned char *src)
                :BitVector(244+8)
        { unpack(src); }

        BitVector payload() { return tail(8); }
        const BitVector payload() const { return tail(8); }

};

#endif
