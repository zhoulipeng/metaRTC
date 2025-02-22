﻿#include "YangPlayerHandleImpl.h"
#include <yangutil/sys/YangLog.h>
#include <yangstream/YangStreamFactory.h>
#include <string>
#include <string.h>
#include <yangutil/yang_unistd.h>


YangPlayerHandle* YangPlayerHandle::createPlayerHandle(YangContext* pcontext,YangSysMessageI* pmessage){
	return new YangPlayerHandleImpl(pcontext,pmessage);
}
YangPlayerHandleImpl::YangPlayerHandleImpl(YangContext* pcontext,YangSysMessageI* pmessage) {
	m_context=pcontext;
	m_message=pmessage;
	m_recv = NULL;
	m_play = NULL;
	m_rtcRecv=NULL;
	m_outVideoBuffer = NULL;
	m_outAudioBuffer = NULL;
	m_url.netType=0;
	m_url.port=1935;
}

YangPlayerHandleImpl::~YangPlayerHandleImpl() {
	if(m_rtcRecv) m_rtcRecv->disConnect();
	yang_delete(m_recv);
	yang_delete(m_play);
	yang_delete(m_rtcRecv);
	yang_delete(m_outVideoBuffer);
	yang_delete(m_outAudioBuffer);

}

void YangPlayerHandleImpl::stopPlay(){
	if(m_rtcRecv) {
		m_rtcRecv->disConnect();
	}
	if(m_recv){
		m_recv->disConnect();
	}
	if(m_play) m_play->stopAll();
	yang_stop(m_rtcRecv);
	yang_stop_thread(m_rtcRecv);
	yang_delete(m_rtcRecv);

	//yang_delete(m_play);
    yang_stop(m_recv);
    yang_stop_thread(m_recv);
    yang_delete(m_recv);

    yang_delete(m_play);

}
int YangPlayerHandleImpl::play(string url,int32_t localPort) {
	//m_url.app="";
	memset(m_url.server,0,sizeof(m_url.server));
	//m_url.server="";
	//m_url.stream="";
	m_url.port=0;
	if(yang_srs_url_parse((char*)url.c_str(),&m_url)) return 1;

	stopPlay();
	printf("\nnetType==%d,server=%s,port=%d,app=%s,stream=%s\n",m_url.netType,m_url.server,m_url.port,m_url.app,m_url.stream);
	m_context->avinfo.sys.transType=m_url.netType;
	if(m_context->streams.m_playBuffer) m_context->streams.m_playBuffer->setTranstype(m_url.netType);
    if(m_url.netType ==Yang_Webrtc){

        return playRtc(0,m_url.server,localPort,m_url.server,1985,m_url.app,m_url.stream);

    }
    return 1;
    /**
	if (!m_play)	{
		m_play = new YangPlayerBase();

        m_context->avinfo.audio.sample=44100;
        m_context->avinfo.audio.channel=2;
        m_context->avinfo.audio.audioDecoderType=0;
        m_context->avinfo.audio.usingMono=0;
        m_context->avinfo.audio.aIndex=-1;
        m_play->init(m_context);
	}
	initList();
	m_play->startAudioDecoder(m_outAudioBuffer);
	m_play->startVideoDecoder(m_outVideoBuffer);
	m_play->startAudioPlay(m_context);
	//m_play->startVideoPlay();


	if (!m_recv) {

		m_recv = new YangPlayReceive(m_context);	//sf.createStreamBase(m_url.netType,0,m_context);
		m_recv->setBuffer(m_outAudioBuffer, m_outVideoBuffer);
	}
	if(m_recv->init(m_url.netType, m_url.server, m_url.port, m_url.stream)){
		printf("\n connect server failure!");
		return 1;
	}

	m_recv->start();
	return Yang_Ok;
	**/

}

int32_t YangPlayerHandleImpl::playRtc(int32_t puid,std::string localIp,int32_t localPort, std::string server, int32_t pport,std::string app,std::string stream){

	stopPlay();
	if (!m_play)	{
		m_play = new YangPlayerBase();

		m_context->avinfo.audio.sample=48000;
		m_context->avinfo.audio.channel=2;
		m_context->avinfo.audio.audioDecoderType=Yang_AED_OPUS;//3;
		m_context->avinfo.audio.usingMono=0;
        m_context->avinfo.audio.aIndex=-1;
		m_play->init(m_context);
	}
	initList();
	m_play->startAudioDecoder(m_outAudioBuffer);
	m_play->startVideoDecoder(m_outVideoBuffer);

	m_play->startAudioPlay(m_context);


	if(m_rtcRecv==NULL) {
		m_rtcRecv=new YangRtcReceive(m_context,m_message);
		m_rtcRecv->setBuffer(m_outAudioBuffer, m_outVideoBuffer);
		m_rtcRecv->init(puid,localIp,localPort,server,pport,app,stream);
	}

	 m_rtcRecv->start();

	 return Yang_Ok;
}

YangVideoBuffer* YangPlayerHandleImpl::getVideoBuffer(){
	if(m_play) return m_play->m_ydb->getOutVideoBuffer();
	return NULL;
}

void YangPlayerHandleImpl::initList() {
	if (m_outAudioBuffer == NULL) {
		m_outAudioBuffer = new YangAudioEncoderBuffer(10);
	}
	if (m_outVideoBuffer == NULL)
		m_outVideoBuffer = new YangVideoDecoderBuffer();

}
