/*
 * YangCRtcpPli.h
 *
 *  Created on: 2022年1月2日
 *      Author: yang
 */

#ifndef SRC_YANGRTP_YANGCRTCPPLI_H_
#define SRC_YANGRTP_YANGCRTCPPLI_H_

#include <yangrtp/YangRtcpPsfbCommon.h>
void yang_init_rtcpPli(YangRtcpCommon* comm);
void yang_destroy_rtcpPli(YangRtcpCommon* comm);
int32_t yang_encode_rtcpPli(YangRtcpCommon* comm,YangBuffer* buffer);
int32_t yang_decode_rtcpPli(YangRtcpCommon* comm,YangBuffer* buffer);
uint64_t yang_rtcpPli_nb_bytes(YangRtcpCommon* comm);




#endif /* SRC_YANGRTP_YANGCRTCPPLI_H_ */
