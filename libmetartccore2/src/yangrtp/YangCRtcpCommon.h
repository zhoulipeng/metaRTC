/*
 * YangCRtcpCommon.h
 *
 *  Created on: 2022年1月2日
 *      Author: yang
 */

#ifndef SRC_YANGRTP_YANGCRTCPCOMMON_H_
#define SRC_YANGRTP_YANGCRTCPCOMMON_H_


#include <yangrtp/YangRtcp.h>
#include <yangrtp/YangRtpConstant.h>
#include <yangutil/buffer/YangBuffer.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

void yang_init_rtcpCommon(YangRtcpCommon* comm);
void yang_destroy_rtcpCommon(YangRtcpCommon* comm);
int32_t yang_decode_rtcpCommon(YangRtcpCommon* comm,YangBuffer *buffer);
int32_t yang_encode_rtcpCommon(YangRtcpCommon* comm,YangBuffer *buffer);
uint64_t yang_get_rtcpCommon_nb_bytes(YangRtcpCommon* comm);
int32_t yang_decode_header_rtcpCommon(YangRtcpCommon* comm,YangBuffer *buffer);
int32_t yang_encode_header_rtcpCommon(YangRtcpCommon* comm,YangBuffer *buffer);



#endif /* SRC_YANGRTP_YANGCRTCPCOMMON_H_ */
