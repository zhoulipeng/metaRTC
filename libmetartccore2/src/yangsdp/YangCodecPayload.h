#ifndef SRC_YANGSDP_YANGCODECPAYLOAD_H_
#define SRC_YANGSDP_YANGCODECPAYLOAD_H_
#include <stdint.h>


#include <yangsdp/YangMediaPayloadType.h>

typedef struct{
	int32_t capacity;
	int32_t vlen;
	char** groups;
}YangFbsVector;
typedef struct{
	   char type[16];
	    uint8_t pt;
	    // for publish, equals to PT of itself;
	    // for subscribe, is the PT of publisher;
	    uint8_t pt_of_publisher;
	    char name[32];
	    int32_t sample;

	    YangFbsVector rtcp_fbs;
}YangCodecPayload;

#endif /* SRC_YANGSDP_YANGCODECPAYLOAD_H_ */
