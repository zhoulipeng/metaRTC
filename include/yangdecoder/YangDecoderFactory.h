﻿/*
 * YangDecoderFactory.h
 *
 *  Created on: 2019年9月3日
 *      Author: yang
 */

#ifndef YANGDECODER_INCLUDE_YANGDECODERFACTORY_H_
#define YANGDECODER_INCLUDE_YANGDECODERFACTORY_H_
#include "yangdecoder/YangAudioDecoder.h"
#include "yangdecoder/YangVideoDecoder.h"


class YangDecoderFactory {
public:
	YangDecoderFactory();
	virtual ~YangDecoderFactory();

	YangAudioDecoder* createAudioDecoder(YangAudioCodec paet,YangAudioParam *pcontext);
	YangAudioDecoder* createAudioDecoder(YangAudioParam *pcontext);
	YangVideoDecoder* createVideoDecoder(YangVideoCodec paet,YangVideoInfo *pcontext);
	YangVideoDecoder* createFfmpegVideoDecoder(YangVideoCodec paet,YangVideoInfo *pcontext);
	YangVideoDecoder* createVideoDecoder(YangVideoInfo *pcontext);
};

#endif /* YANGDECODER_INCLUDE_YANGDECODERFACTORY_H_ */
