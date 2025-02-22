/*
 * YangCRtcConnection.h
 *
 *  Created on: 2022年1月2日
 *      Author: yang
 */

#ifndef SRC_YANGWEBRTC_YANGCRTCCONNECTION_H_
#define SRC_YANGWEBRTC_YANGCRTCCONNECTION_H_

#include <yangrtp/YangReceiveNackBuffer.h>
#include <yangrtp/YangRtpPacket.h>
#include <yangrtp/YangRtpRecvNack.h>
#include <yangutil/sys/YangTime.h>
#include <yangwebrtc/YangRtcContext.h>

int32_t yang_send_nackpacket(YangRtcContext* context,char* data,int32_t nb);
void yang_do_request_keyframe(YangRtcContext* context,uint32_t ssrc);
int32_t yang_send_rtcp_fb_pli(YangRtcContext* context,uint32_t ssrc);
int32_t yang_send_rtcp_rr(YangRtcContext* context,uint32_t ssrc, YangReceiveNackBuffer *rtp_queue,
		const uint64_t last_send_systime,  YangNtp* last_send_ntp);
int32_t yang_send_rtcp_xr_rrtr(YangRtcContext* context,uint32_t ssrc);
int32_t yang_check_send_nacks(YangRtcContext* context,YangRtpRecvNack *nack, uint32_t ssrc,
		uint32_t *sent_nacks, uint32_t *timeout_nacks) ;
#endif /* SRC_YANGWEBRTC_YANGCRTCCONNECTION_H_ */
