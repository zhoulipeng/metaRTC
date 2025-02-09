/*
 * YangCRtcpRR.h
 *
 *  Created on: 2022年1月2日
 *      Author: yang
 */

#ifndef SRC_YANGRTP_YANGCRTCPRR_H_
#define SRC_YANGRTP_YANGCRTCPRR_H_

#include <yangrtp/YangRtcpCommon.h>
#include <yangutil/buffer/YangBuffer.h>
void yang_init_rtcpRR(YangRtcpCommon* comm);
void yang_destroy_rtcpRR(YangRtcpCommon* comm);
int32_t yang_decode_rtcpRR(YangRtcpCommon* comm,YangBuffer *buffer);
int32_t yang_encode_rtcpRR(YangRtcpCommon* comm,YangBuffer *buffer);
uint64_t yang_rtcpRR_nb_bytes(YangRtcpCommon* comm);
void yang_rtcpRR_set_sender_ntp(YangRtcpCommon* comm,uint64_t ntp);
void yang_set_sender_ntp(YangRtcpCommon* comm,uint64_t ntp);



#endif /* SRC_YANGRTP_YANGCRTCPRR_H_ */
