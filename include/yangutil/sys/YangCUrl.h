/*
 * YangCUrl.h
 *
 *  Created on: 2021年12月27日
 *      Author: yang
 */

#ifndef INCLUDE_YANGUTIL_SYS_YANGCURL_H_
#define INCLUDE_YANGUTIL_SYS_YANGCURL_H_

#include <stdint.h>

#define Yang_Websocket_Ws 0
#define Yang_Websocket_Wss 1
#define Yang_Websocket_Http 2
#define Yang_Websocket_Https 3
typedef struct YangUrlData{
	int32_t netType;
	int32_t port;
	char server[30];
	char app[20];
	char stream[20];
}YangUrlData;
//webrtc://host[:port]/app/stream
int32_t yang_srs_url_parse(char* purl, YangUrlData* data);
//http://host:port/path ws://host:port/path wss://host:port/path
int32_t yang_ws_url_parse(char* purl, YangUrlData* data);



#endif /* INCLUDE_YANGUTIL_SYS_YANGCURL_H_ */
