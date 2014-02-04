#ifndef __GSMTVOID_SYSTEM_H__
#define __GSMTVOID_SYSTEM_H__ 1

#define DEBUGF(a...)	{ \
	fprintf(stderr, "%s:%d ", __FILE__, __LINE__); \
	fprintf(stderr, a); \
} while (0)

#endif

