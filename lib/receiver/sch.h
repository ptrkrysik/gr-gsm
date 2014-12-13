
#ifndef __SCH_H__
#define __SCH_H__ 1

#include <grgsm/api.h>

#ifdef __cplusplus
extern "C"
{
#endif

  GSM_API int decode_sch(const unsigned char *buf, int * t1_o, int * t2_o, int * t3_o, int * ncc, int * bcc);

#ifdef __cplusplus
}
#endif

#endif

