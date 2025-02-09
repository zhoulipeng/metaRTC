/*
 * YangSdpHandle.c
 *
 *  Created on: 2022年1月15日
 *      Author: yang
 */

#include <yangsdp/YangSdpHandle.h>
#include <yangsdp/YangRtcSdp.h>
#include <yangutil/sys/YangLog.h>
void yang_parse_sdp(char* str){
	   YangSdp sdp;
	    memset(&sdp,0,sizeof(YangSdp));
	    yang_init_rtcsdp(&sdp);
	    yang_rtcsdp_parse(&sdp,str);
	    char* tmp=(char*)calloc(1024*12,1);
	    YangBuffer buf;
	    yang_init_buffer(&buf,tmp,1024*12);
	    yang_rtcsdp_encode(&sdp,&buf);
	    yang_trace("\n**********************************\n");
	    yang_trace("%s",tmp);
	    yang_destroy_rtcsdp(&sdp);
}
