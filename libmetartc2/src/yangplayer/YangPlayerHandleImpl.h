
#ifndef YANGMEETING_INCLUDE_YANGPLAYERHANDLE_H_
#define YANGMEETING_INCLUDE_YANGPLAYERHANDLE_H_
#include <yangplayer/YangPlayerHandle.h>
#include <yangplayer/YangPlayerBase.h>
#include <yangplayer/YangPlayReceive.h>
#include <yangstream/YangSynBuffer.h>
#include <yangutil/yangavinfotype.h>
#include <yangutil/sys/YangUrl.h>
#include "YangRtcReceive.h"


class YangPlayerHandleImpl :public YangPlayerHandle{
public:
	YangPlayerHandleImpl(YangContext* pcontext,YangSysMessageI* pmessage);
	virtual ~YangPlayerHandleImpl();
	YangVideoBuffer* getVideoBuffer();
	int play(string url,int32_t localport);

	int32_t playRtc(int32_t puid,std::string localIp,int32_t localPort, std::string server, int32_t pport,std::string app,std::string stream);
	void stopPlay();
protected:

	YangUrlData m_url;
	void initList();
	YangPlayReceive *m_recv;
	YangPlayerBase *m_play;
	YangRtcReceive *m_rtcRecv;

private:
	YangContext* m_context;
	YangVideoDecoderBuffer* m_outVideoBuffer;
	YangAudioEncoderBuffer* m_outAudioBuffer;
	YangSysMessageI* m_message;
};

#endif /* YANGMEETING_INCLUDE_YANGPLAYERHANDLE_H_ */
