
#include <yangsdp/YangRedPayload.h>
#include <yangutil/sys/YangCString.h>
void yang_init_redPayloaddes(YangRedPayload* red,uint8_t pt, char* encode_name, int32_t sample,int32_t channel){
	red->sample=sample;
	red->pt=pt;
	strcpy(red->name,encode_name);
	red->channel=channel;
}
