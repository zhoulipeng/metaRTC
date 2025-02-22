/*
 * YangRtcStream.h
 *
 *  Created on: 2022年1月4日
 *      Author: yang
 */

#ifndef SRC_YANGWEBRTC_YANGRTCSTREAM_H_
#define SRC_YANGWEBRTC_YANGRTCSTREAM_H_
#include <yangutil/sys/YangTime.h>
#include <stdint.h>
#include <yangrtp/YangPublishNackBuffer.h>
#include <yangrtp/YangReceiveNackBuffer.h>
#include <yangrtp/YangRtpFUAPayload2.h>
#include <yangrtp/YangRtpPacket.h>
#include <yangrtp/YangRtpRecvNack.h>
#include <yangrtp/YangRtpSTAPPayload.h>
#define Yang_Rtc_Recv_Buffer 512
#define Yang_Rtc_Recv_Buffer2 511

typedef struct {
	int32_t uid;
	uint16_t last_seq;
	int16_t isAudio;
	// Latest sender report ntp and rtp time.

	int64_t last_sender_report_rtp_time;

	// Prev sender report ntp and rtp time.

	int64_t last_sender_report_rtp_time1;
	//YangNtp last_sender_report_ntp;
	uint64_t last_sender_report_sys_time;

	uint32_t ssrc_;

	YangRtpBuffer *rtpBuffer;
	YangReceiveNackBuffer *rtp_queue;
	YangRtpRecvNack *nack_receiver;
	YangFrame audioFrame;
	YangNtp last_sender_report_ntp;
	YangNtp last_sender_report_ntp1;
} YangRecvTrack;
typedef struct  {
	bool in_use;
	bool end;
	uint16_t sn;
	uint32_t ts;
	int32_t nalu_type;
	int32_t nb;
	Fua2H264DataCache fua2;
	char *payload;
} YangH264PacketCache;
typedef struct  {
	uint16_t header_sn;
	uint16_t lost_sn;
	int32_t hasReceiveStap;
	int32_t uid;
	int32_t hasRequestKeyframe;
	int64_t key_frame_ts;

	YangRtpSTAPData stapData;
	YangBuffer buf;
	YangFrame videoFrame;
	char *video_buffer;
	//YangReceiveCallback *recvcb;
	YangH264PacketCache cache_video_pkts[Yang_Rtc_Recv_Buffer];

} YangH264RecvTrack;

typedef struct  {
	bool in_use;
	bool end;
	uint16_t sn;
	uint32_t ts;
	int32_t nalu_type;
	int32_t nb;
	Fua2H265DataCache fua2;
	char *payload;
} YangH265PacketCache;

typedef struct  {
	uint16_t header_sn;
	uint16_t lost_sn;
	int32_t hasReceiveStap;
	int64_t key_frame_ts;

	int32_t uid;
	uint32_t ssrc;
	uint32_t hasRequestKeyframe;

	char *video_buffer;
	YangRtpSTAPData stapData;
	YangBuffer buf;
	YangFrame videoFrame;

	YangH265PacketCache cache_video_pkts[Yang_Rtc_Recv_Buffer];
} YangH265RecvTrack;

typedef struct  {
	YangRecvTrack recv;
	YangH264RecvTrack *h264Track;
	YangH265RecvTrack *h265Track;
} YangVideoRecvTrack;
typedef struct  {
	uint32_t audioSsrc;
	uint32_t videoSsrc;
	int32_t mw_msgs;
	int16_t realtime;

	YangPublishNackBuffer *audio_queue;
	YangPublishNackBuffer *video_queue;
} YangRtcPublishStream;

typedef struct  {
	uint8_t twcc_fb_count;
	uint8_t request_keyframe;
	uint16_t twcc_enabled;
	int32_t nn_simulate_nack_drop;

	int32_t twcc_id;
	int64_t last_time_send_twcc;

	YangRtpBuffer *rtpBuffer;

	YangRtpPacket rtp;
	YangBuffer buf;
	YangRecvTrack *audio_track;
	YangVideoRecvTrack *video_track;

} YangRtcPlayStream;

#endif /* SRC_YANGWEBRTC_YANGRTCSTREAM_H_ */
