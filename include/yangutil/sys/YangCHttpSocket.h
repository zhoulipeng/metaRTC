/*
 * YangCHttpSocket.h
 *
 *  Created on: 2021年12月26日
 *      Author: yang
 */

#ifndef INCLUDE_YANGUTIL_SYS_YANGCHTTPSOCKET_H_
#define INCLUDE_YANGUTIL_SYS_YANGCHTTPSOCKET_H_
#include <stdint.h>

 int32_t yang_http_post(char* rets,char* ip,int32_t port,char* api,uint8_t *p, int32_t plen);


#endif /* INCLUDE_YANGUTIL_SYS_YANGCHTTPSOCKET_H_ */
