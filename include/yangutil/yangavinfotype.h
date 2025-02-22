﻿/*
 * yangtype.h
 *
 *  Created on: 2020年10月14日
 *      Author: yang
 */

#ifndef YANGUTIL_YANGAVINFOTYPE_H_
#define YANGUTIL_YANGAVINFOTYPE_H_
#include <yangutil/yangtype.h>
#include <yangutil/yangavtype.h>

//#include <yangutil/sys/YangCertificate.h>

/**
 struct YangEncoderParam {
 int32_t width;
 int32_t height;
 int32_t fps;
 int32_t bitrate;
 int32_t keyint_max;
 int32_t level_idc;
 int32_t profile;
 };
 **/
typedef struct YangAudioInfo {
	int32_t sample;
	int32_t frameSize;
	int32_t bitrate;
	int32_t channel;

	int32_t usingMono;
	int32_t hasAec;
	int32_t echoPath;

	int32_t aecBufferFrames;
	int32_t audioCacheSize;
	int32_t audioCacheNum;
	int32_t audioPlayCacheNum;

	int32_t hasAudioHeader;
	int32_t audioEncoderType;
	int audioDecoderType;
	int32_t audioPlayType;

	int32_t aIndex;
	int32_t aSubIndex;
}YangAudioInfo;

typedef struct YangVideoInfo {
	int32_t width; //= 800
	int32_t height; //= 600
	int32_t outWidth;
	int32_t outHeight;
	int32_t rate; // 512
	int32_t frame; //25
	//int32_t bitcount; // 16
	int32_t bitDepth;

	int32_t videoCacheNum;
	int32_t evideoCacheNum;
	int32_t videoPlayCacheNum;

	enum YangYuvType videoCaptureFormat;
	enum YangYuvType videoEncoderFormat;
	enum YangYuvType videoDecoderFormat;

	int32_t videoEncoderType;
	int32_t videoDecoderType;
	int32_t videoEncHwType;
	int32_t videoDecHwType;
	int32_t vIndex;
}YangVideoInfo;
typedef struct YangVideoEncInfo {
	int32_t preset;
	int32_t level_idc;
	int32_t profile;
	int32_t keyint_max;
	int32_t enc_threads;
	int32_t gop;
	bool createMeta;


}YangVideoEncInfo;
typedef struct YangSysInfo {
	int32_t isMultCamera;
	int32_t transType;
	int32_t usingDataServer;
	int32_t rtmpPort;
	int32_t srtPort;
	int32_t rtcPort;
	int32_t rtcLocalPort;
	int32_t httpPort;
	int32_t dataPort;
	int32_t hasLogFile;
	int32_t logLevel;
	int32_t cameraCount;

	char cameraIndexs[50];
	char rtmpServerIP[20];
	char srtServerIP[20];
	char rtcServerIP[20];
	char httpServerIP[20];
	char dataServerIP[20];
}YangSysInfo;

typedef struct YangRtcInfo {
	int32_t sendTwcc;
	int32_t mixAvqueue;
	int32_t audioQueueCount;
	int32_t videoQueueCount;

}YangRtcInfo;

typedef struct YangAVInfo{
	YangAudioInfo audio;
	YangVideoInfo video;
	YangVideoEncInfo enc;
	YangSysInfo sys;
	YangRtcInfo rtc;
}YangAVInfo;

#ifdef __cplusplus
#include <yangstream/YangStreamManager.h>
class YangContext {
public:
	YangContext();
	virtual ~YangContext();
	void init(char *filename);
	void init();
	//YangCertificate* getCertificate();
	virtual void initExt(void *filename);
	virtual void initExt();
public:
	YangAVInfo avinfo;
#if Yang_HaveVr
        char bgFilename[256];
#endif

	YangStreamManager streams;
private:
	//YangCertificate* m_certificate;

};
extern "C"{
void yang_init_avinfo(YangAVInfo* avinfo);
}
#else
void yang_init_avinfo(YangAVInfo* avinfo);
#endif
//void yang_init_context(YangContext* context,char* filename);
#endif /* YANGUTIL_YANGTYPE_H_ */
