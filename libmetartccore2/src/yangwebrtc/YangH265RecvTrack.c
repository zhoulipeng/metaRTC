﻿#include <yangutil/yangavtype.h>
#include <yangutil/yangavtype_h265.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangEndian.h>
#include <yangavutil/video/YangMeta.h>
#include <yangrtp/YangRtpConstant.h>
#include <yangrtp/YangRtpFUAPayload2.h>
#include <yangrtp/YangRtpHeader.h>
#include <yangrtp/YangRtpRawPayload.h>
#include <yangrtp/YangRtpSTAPPayload.h>
#include <yangwebrtc/YangCH265RecvTrack.h>
#include <yangwebrtc/YangRtcConnection.h>
void yang_h265recv_copy(YangH265RecvTrack *recv, YangRtpPacket *src,
		YangH265PacketCache *pkt);
int32_t yang_h265recv_packet_video(YangRtcContext *context,
		YangH265RecvTrack *recv, const uint16_t start, const uint16_t end);
int32_t yang_h265recv_is_keyframe(YangRtpPacket *pkt);
int32_t yang_h265recv_is_keyframe2(YangH265PacketCache *pkt);
void yang_h265recv_copy(YangH265RecvTrack *recv, YangRtpPacket *src,
		YangH265PacketCache *pkt);
int32_t yang_h265recv_put_frame_video(YangRtcContext *context,
		YangH265RecvTrack *recv, char *p, int64_t timestamp, int32_t nb);
int32_t yang_h265recv_find_next_lost_sn(YangH265RecvTrack *recv,
		uint16_t current_sn, uint16_t *end_sn);
int32_t yang_h265recv_check_frame_complete(YangH265RecvTrack *recv,
		const uint16_t start, const uint16_t end);
void yang_h265recv_clear_cached_video(YangH265RecvTrack *recv);

void yang_init_h265recv(YangRtcContext *context, YangH265RecvTrack *recv,
		YangRtpBuffer *rtpBuffer) {
	recv->ssrc = context->videoTrack.ssrc;
	recv->key_frame_ts = -1;
	recv->lost_sn = 0;
	recv->header_sn = 0;
	recv->hasRequestKeyframe = false;

	recv->hasReceiveStap = -1;

	memset(recv->cache_video_pkts, 0,
			Yang_Rtc_Recv_Buffer * sizeof(YangH265PacketCache));
	recv->video_buffer = (char*) calloc(1, YANG_VIDEO_ENCODE_BUFFER_LEN);
	memset(recv->video_buffer, 0, YANG_VIDEO_ENCODE_BUFFER_LEN);

	memset(&recv->stapData, 0, sizeof(YangRtpSTAPData));

	yang_inti_sampleVector(&recv->stapData.nalus);

	recv->uid = context->streamConf->uid;


	memset(&recv->videoFrame, 0, sizeof(YangFrame));
}
void yang_destroy_h265recv(YangH265RecvTrack *recv) {
	yang_free(recv->video_buffer);
	yang_reset_h2645_stap(&recv->stapData);
	yang_destroy_sampleVector(&recv->stapData.nalus);
}

int32_t yang_h265recv_packet_video(YangRtcContext *context,
		YangH265RecvTrack *recv, const uint16_t start, const uint16_t end) {
	int32_t err = Yang_Ok;
	uint16_t cnt = end - start + 1;
	YangH265PacketCache *header = &recv->cache_video_pkts[yang_cache_index(
			start)];
	int64_t timestamp = header->ts;
	char *p = recv->video_buffer;
	if (yang_h265recv_is_keyframe2(header)) {
		*p++ = 0x1c; // type(4 bits): key frame; code(4bits): avc
		recv->key_frame_ts = -1;
	} else {
		*p++ = 0x2c; // type(4 bits): inter frame; code(4bits): avc
	}
	*p++ = 0x01;
	*p++ = 0x00;
	*p++ = 0x00;
	*p++ = 0x00;

	int32_t nalu_len = 0;
	for (uint16_t i = 0; i < cnt; ++i) {
		uint16_t index = yang_cache_index((start + i));
		YangH265PacketCache *pkt = &recv->cache_video_pkts[index];
		recv->cache_video_pkts[index].in_use = false;

		if (pkt->nalu_type == H265_kFuA) {
			if (pkt->nb > 0) {
				if (pkt->fua2.start) {
					nalu_len = pkt->nb + 1;
					p += 4;
					*p++ = pkt->fua2.nalu_type << 1;
					memcpy(p, pkt->payload, pkt->nb);
					p += pkt->nb;
				} else {
					nalu_len += pkt->nb;
					memcpy(p, pkt->payload, pkt->nb);
					p += pkt->nb;
					if (pkt->fua2.end) {
						//write nalu_len back
						yang_put_be32(p - (4 + nalu_len), nalu_len);
					}
				}
				pkt->fua2.start = pkt->fua2.end = false;
				pkt->fua2.nri = pkt->fua2.nalu_type = (YangHevcNaluType) 0;
				continue;
			}
		}

		if (pkt->nalu_type == H265_kStapA) {

			if (recv->hasReceiveStap > -1 && recv->hasReceiveStap == pkt->sn) {
				recv->hasReceiveStap = -1;
				continue;
			} else {

				YangBuffer stap_buf;
				yang_init_buffer(&stap_buf, pkt->payload, pkt->nb);
				yang_reset_h2645_stap(&recv->stapData);
				yang_decode_h265_stap(&stap_buf, &recv->stapData);


				for (int32_t j = 0; j < (int) recv->stapData.nalus.vlen; ++j) {
					YangSample *sample = yang_get_sampleVector(
							&recv->stapData.nalus, j);
					if (sample->nb > 0) {
						yang_put_be32(p, sample->nb);
						p += 4;
						memcpy(p, sample->bytes, sample->nb);
						p += sample->nb;
					}
				}
				yang_reset_h2645_stap(&recv->stapData);
				continue;
			}

		}
		if (pkt->payload) {

			if (pkt->nb > 0) {
				yang_put_be32(p, pkt->nb);
				p += 4;
				memcpy(p, pkt->payload, pkt->nb);
				p += pkt->nb;

				pkt->payload = NULL;

				continue;
			}
		}
		memset(&recv->cache_video_pkts[i], 0,
				sizeof(YangH265PacketCache));
	}

	if ((err = yang_h265recv_put_frame_video(context, recv, recv->video_buffer,
			timestamp, p - recv->video_buffer)) != Yang_Ok) {
		yang_warn("fail to pack video frame");
	}

	recv->header_sn = end + 1;
	uint16_t tail_sn = 0;
	int32_t sn = yang_h265recv_find_next_lost_sn(recv, recv->header_sn,
			&tail_sn);
	if (-1 == sn) {
		if (yang_h265recv_check_frame_complete(recv, recv->header_sn,
				tail_sn)) {
			err = yang_h265recv_packet_video(context, recv, recv->header_sn,
					tail_sn);
		}
	} else if (-2 == sn) {
		return yang_error_wrap(ERROR_RTC_RTP_MUXER, "video cache is overflow");
	} else {
		recv->lost_sn = sn;
	}

	return err;
}

int32_t yang_h265recv_packet_video_key_frame(YangRtcContext *context,
		YangH265RecvTrack *recv, YangRtpPacket *pkt) {
	int32_t err = Yang_Ok;
	uint16_t index = yang_cache_index(pkt->header.sequence);

	if (pkt->nalu_type == H265_kStapA) {
		YangBuffer stap_buf; //
		yang_init_buffer(&stap_buf, pkt->payload, pkt->nb);
		yang_reset_h2645_stap(&recv->stapData);
		yang_decode_h265_stap(&stap_buf, &recv->stapData);
		YangSample *vps = yang_get_h265_stap_vps(&recv->stapData);
		YangSample *sps = yang_get_h265_stap_sps(&recv->stapData);
		YangSample *pps = yang_get_h265_stap_pps(&recv->stapData);
		if (NULL == vps || NULL == sps || NULL == pps) {
			return yang_error_wrap(ERROR_RTC_RTP_MUXER,
					"no vps or sps or pps in stap-a rtp.vps:%p, sps: %p, pps:%p",
					vps, sps, pps);
		} else {
			int32_t metaLen = 0;
			yang_getConfig_Meta_H265(vps, sps, pps,
					(uint8_t*) recv->video_buffer, &metaLen);
			recv->hasReceiveStap = pkt->header.sequence;
			yang_reset_h2645_stap(&recv->stapData);
			if (metaLen > 0) {
				if ((err = yang_h265recv_put_frame_video(context, recv,
						recv->video_buffer, 0, metaLen)) != Yang_Ok) {
					return err;
				}
			}

		}
	}
	if (-1 == recv->key_frame_ts) {
		recv->key_frame_ts = pkt->header.timestamp;
		recv->header_sn = pkt->header.sequence;
		recv->lost_sn = recv->header_sn + 1;
		// Received key frame and clean cache of old p frame pkts
		yang_h265recv_clear_cached_video(recv);
		//yang_trace("set ts=%lld, header=%hu, lost=%hu", key_frame_ts_, header_sn_, lost_sn_);
	} else if (recv->key_frame_ts != pkt->header.timestamp) {
		//new key frame, clean cache

		recv->key_frame_ts = pkt->header.timestamp;
		recv->header_sn = pkt->header.sequence;
		recv->lost_sn = recv->header_sn + 1;
		yang_h265recv_clear_cached_video(recv);

	}

	recv->cache_video_pkts[index].in_use = true;
	yang_h265recv_copy(recv, pkt, &recv->cache_video_pkts[index]);

	int32_t sn = recv->lost_sn;
	uint16_t tail_sn = 0;
	if (yang_rtp_seq_distance(recv->header_sn, pkt->header.sequence) < 0) {
		// When receive previous pkt in the same frame, update header sn;
		recv->header_sn = pkt->header.sequence;
		sn = yang_h265recv_find_next_lost_sn(recv, recv->header_sn, &tail_sn);
	} else if (recv->lost_sn == pkt->header.sequence) {
		sn = yang_h265recv_find_next_lost_sn(recv, recv->lost_sn, &tail_sn);
	}

	if (-1 == sn) {
		if (yang_h265recv_check_frame_complete(recv, recv->header_sn,
				tail_sn)) {
			if ((err = yang_h265recv_packet_video(context, recv,
					recv->header_sn, tail_sn)) != Yang_Ok) {
				err = yang_error_wrap(err, "fail to packet key frame");
			}
		}
	} else if (-2 == sn) {
		return yang_error_wrap(ERROR_RTC_RTP_MUXER, "video cache is overflow");
	} else {
		recv->lost_sn = (uint16_t) sn;
	}

	return err;
}

void yang_h265recv_clear_cached_video(YangH265RecvTrack *recv) {
	for (size_t i = 0; i < Yang_Rtc_Recv_Buffer; i++) {
		if (recv->cache_video_pkts[i].in_use) {
			memset(&recv->cache_video_pkts[i], 0,
					sizeof(YangH265PacketCache));
		}
	}
}

int32_t yang_h265recv_put_frame_video(YangRtcContext *context,
		YangH265RecvTrack *recv, char *p, int64_t timestamp, int32_t nb) {

	recv->videoFrame.uid = recv->uid;
	recv->videoFrame.payload = (uint8_t*) p;
	recv->videoFrame.nb = nb;
	recv->videoFrame.pts = timestamp;


	yang_recvtrack_receiveVideo(context,&recv->videoFrame);

	return Yang_Ok;
}

int32_t yang_h265recv_check_frame_complete(YangH265RecvTrack *recv,
		const uint16_t start, const uint16_t end) {
	uint16_t cnt = (end - start + 1);
	uint16_t fu_s_c = 0;
	uint16_t fu_e_c = 0;
	for (uint16_t i = 0; i < cnt; ++i) {
		int32_t index = yang_cache_index((start + i));
		YangH265PacketCache *pkt = &recv->cache_video_pkts[index];
		if (pkt->nalu_type == H265_kFuA) {
			if (pkt->fua2.start) {
				++fu_s_c;
			}

			if (pkt->fua2.end) {
				++fu_e_c;
			}
		}
	}

	return fu_s_c == fu_e_c;
}

int32_t yang_h265recv_find_next_lost_sn(YangH265RecvTrack *recv,
		uint16_t current_sn, uint16_t *end_sn) {
	uint32_t last_ts =
			recv->cache_video_pkts[yang_cache_index(recv->header_sn)].ts;
	for (int32_t i = 0; i < Yang_Rtc_Recv_Buffer; ++i) {
		uint16_t lost_sn = current_sn + i;
		int32_t index = yang_cache_index(lost_sn);

		if (!recv->cache_video_pkts[index].in_use) {
			return lost_sn;
		}
		//check time first, avoid two small frame mixed case decode fail
		if (last_ts != recv->cache_video_pkts[index].ts) {
			*end_sn = lost_sn - 1;
			return -1;
		}

		if (recv->cache_video_pkts[index].end) {
			*end_sn = lost_sn;
			return -1;
		}
	}

	yang_error(
			"the cache is mess. the packet count of video frame is more than %u",
			Yang_Rtc_Recv_Buffer);
	return -2;
}

int32_t yang_h265recv_is_keyframe(YangRtpPacket *pkt) {
	// It's normal H264 video rtp packet
	if (pkt->nalu_type == H265_kStapA) {

		YangHevcNaluType nalu_type = (YangHevcNaluType) (YANG_HEVC_NALU_TYPE(
				*(pkt->payload + 3)));
		if (nalu_type == YANG_NAL_UNIT_VPS || nalu_type == YANG_NAL_UNIT_SPS
				|| nalu_type == YANG_NAL_UNIT_VPS) {
			return true;
		}
	} else if (pkt->nalu_type == H265_kFuA) {
		YangHevcNaluType v = (YangHevcNaluType) (YANG_HEVC_NALU_TYPE(
				(uint8_t )(*(pkt->payload + 2))));

		if (v >= YANG_NAL_UNIT_CODED_SLICE_BLA
				&& v <= YANG_NAL_UNIT_CODED_SLICE_CRA)
			return true;

	} else {                   //32
		if ((pkt->nalu_type == YANG_NAL_UNIT_VPS)
				|| (pkt->nalu_type == YANG_NAL_UNIT_SPS)
				|| (pkt->nalu_type == YANG_NAL_UNIT_PPS)) {
			return true;
		}
	}
	return false;
}

int32_t yang_h265recv_is_keyframe2(YangH265PacketCache *pkt) {
	// It's normal H264 video rtp packet
	if (pkt->nalu_type == H265_kStapA) {

		YangHevcNaluType nalu_type = (YangHevcNaluType) (YANG_HEVC_NALU_TYPE(
				*(pkt->payload + 3)));
		if (nalu_type == YANG_NAL_UNIT_VPS || nalu_type == YANG_NAL_UNIT_SPS
				|| nalu_type == YANG_NAL_UNIT_VPS) {
			return true;
		}
	} else if (pkt->nalu_type == H265_kFuA) {
		uint8_t v = (uint8_t) (*pkt->payload);
		if (YangAvcNaluTypeIDR == (YangAvcNaluType) (v & kNalTypeMask))
			return true;

	} else {
		if ((pkt->nalu_type == YANG_NAL_UNIT_VPS)
				|| (pkt->nalu_type == YANG_NAL_UNIT_SPS)
				|| (pkt->nalu_type == YANG_NAL_UNIT_PPS)) {
			return true;
		}
	}
	return false;
}

int32_t yang_h265recv_on_rtp(YangRtcContext *context, YangH265RecvTrack *recv,
		YangRtpPacket *src) {
	int32_t err = Yang_Ok;
	if (!recv->hasRequestKeyframe) {
		yang_send_rtcp_fb_pli(context, context->videoTrack.ssrc);


	}
	src->nalu_type = YANG_HEVC_NALU_TYPE(src->payload[0]);


	if (yang_h265recv_is_keyframe(src)) {
		recv->hasRequestKeyframe = true;
		return yang_h265recv_packet_video_key_frame(context, recv, src);
	}

//    if(src->nalu_type<=YangAvcNaluTypeCodedSliceExt){
//        if(src->header.marker) {
//            packet_video_Raw(src);
//            return Yang_Ok;
//        }

	//   }
	// store in cache

	int32_t index = yang_cache_index(src->header.sequence);
	recv->cache_video_pkts[index].in_use = true;
	yang_h265recv_copy(recv, src, &recv->cache_video_pkts[index]);

	// check whether to recovery lost packet and can construct a video frame
	if (recv->lost_sn == src->header.sequence) {
		uint16_t tail_sn = 0;
		int32_t sn = yang_h265recv_find_next_lost_sn(recv, recv->lost_sn,
				&tail_sn);
		if (-1 == sn) {
			if (yang_h265recv_check_frame_complete(recv, recv->header_sn,
					tail_sn)) {
				if ((err = yang_h265recv_packet_video(context, recv,
						recv->header_sn, tail_sn)) != Yang_Ok) {
					err = yang_error_wrap(err, "fail to pack video frame");
				}
			}
		} else if (-2 == sn) {
			return yang_error_wrap(ERROR_RTC_RTP_MUXER,
					"video cache is overflow");
		} else {
			recv->lost_sn = (uint16_t) sn;
		}
	}

	return err;
}

void yang_h265recv_copy(YangH265RecvTrack *recv, YangRtpPacket *src,
		YangH265PacketCache *pkt) {
	pkt->sn = src->header.sequence;
	pkt->end = src->header.marker;

	// For performance issue, do not copy the unused field.

	pkt->ts = src->header.timestamp;
	pkt->nalu_type = src->nalu_type;

	pkt->payload = src->payload;
	pkt->nb = src->nb;

	if (pkt->nalu_type == H265_kFuA) {
		yang_init_buffer(&recv->buf, src->payload, src->nb);
		yang_decode_h265_fua2(&recv->buf, &pkt->fua2);
		pkt->payload = recv->buf.head;
		pkt->nb = yang_buffer_left(&recv->buf);
	}
}

int32_t yang_h265recv_packet_video_Raw(YangRtcContext *context,
		YangH265RecvTrack *recv, YangRtpPacket *pkt) {
	int err = Yang_Ok;
	char *p = recv->video_buffer;
	int64_t timestamp = pkt->header.timestamp;
	*p++ = 0x2c;	// type(4 bits): inter frame; code(4bits): avc
	*p++ = 0x01;
	*p++ = 0x00;
	*p++ = 0x00;
	*p++ = 0x00;
	yang_put_be32(p, pkt->nb);
	p += 4;
	memcpy(p, pkt->payload, pkt->nb);
	p += pkt->nb;
	if ((err = yang_h265recv_put_frame_video(context, recv, recv->video_buffer,
			timestamp, p - recv->video_buffer)) != Yang_Ok) {
		yang_warn("fail to pack video frame");
	}

	recv->header_sn = pkt->header.sequence + 1;
	uint16_t tail_sn = 0;
	int32_t sn = yang_h265recv_find_next_lost_sn(recv, recv->header_sn,
			&tail_sn);
	if (-1 == sn) {
		if (yang_h265recv_check_frame_complete(recv, recv->header_sn,
				tail_sn)) {
			err = yang_h265recv_packet_video(context, recv, recv->header_sn,
					tail_sn);
		}
	} else if (-2 == sn) {
		return yang_error_wrap(ERROR_RTC_RTP_MUXER, "video cache is overflow");
	} else {
		recv->lost_sn = sn;
	}

	return err;
}
