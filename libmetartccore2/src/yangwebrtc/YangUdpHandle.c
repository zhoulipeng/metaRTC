﻿#include <yangutil/sys/YangLog.h>
#include <yangutil/yang_unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <yangwebrtc/YangUdpHandle.h>
int32_t yang_init_rtcudp(YangUdpHandle *udp, char *pserverIp,
		int32_t plocalPort) {
	if (udp == NULL)
		return 1;
	udp->lclPort = plocalPort;
	udp->serverfd = -1;
	udp->serverPort = 8000;

	udp->lcl_addr.sin_family = AF_INET;
	udp->lcl_addr.sin_port = htons(udp->lclPort);
	// lcl_addr.sin_addr.s_addr=inet_addr(ip);
	udp->lcl_addr.sin_addr.s_addr = INADDR_ANY;
	udp->serverfd = socket(AF_INET, SOCK_DGRAM, 0);

	udp->remote_addr.sin_family = AF_INET;
	udp->remote_addr.sin_port = htons(udp->serverPort);
	udp->remote_addr.sin_addr.s_addr = inet_addr(pserverIp);
	return Yang_Ok;

}
void yang_destroy_rtcudp(YangUdpHandle *udp) {
	if (udp == NULL)
		return;
	yang_stop_rtcudp(udp);
	if (udp->serverfd > 0) {

		close(udp->serverfd);

		udp->serverfd = -1;
	}
}
int32_t yang_rtc_sendData(YangUdpHandle *udp, char *data, int32_t nb) {
	if (udp == NULL || udp->isStart == 0||data==NULL)	return 1;
	int32_t n = 0;

	n = sendto(udp->serverfd, data, nb, 0, (struct sockaddr*) &udp->remote_addr,
			sizeof(struct sockaddr));

	return n > 0 ? 0 : 1;
}

void* yang_run_rtcudp_thread(void *obj) {
	YangUdpHandle *udp = (YangUdpHandle*) obj;
	udp->isStart = 1;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 200000;  // 200 ms
	setsockopt(udp->serverfd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv,
			sizeof(struct timeval));

	printf("\n udp server is starting..localPort==%d", udp->lclPort);

	if (bind(udp->serverfd, (struct sockaddr*) &udp->lcl_addr,
			sizeof(struct sockaddr_in)) < 0) {

		perror("Udp server bind error");
		exit(1);
	}
	char buffer[2048] = { 0 };
	udp->isLoop = 1;

	int32_t len = 0;
	if (udp->startStunTimer)
		udp->startStunTimer(udp->user);
	socklen_t src_len = sizeof(struct sockaddr_in);
	while (udp->isLoop) {
		struct sockaddr_in src;
		memset(&src, 0, src_len);
		memset(buffer, 0, 2048);
		if ((len = recvfrom(udp->serverfd, buffer, 2048, 0,
				(struct sockaddr*) &src, &src_len)) > 0) {
			if (udp->receive)
				udp->receive(buffer, len, udp->user);
		}
	}
	udp->isStart = 0;
	close(udp->serverfd);
	udp->serverfd = -1;

	return NULL;
}
void yang_start_rtcudp(YangUdpHandle *udp) {
	if (udp == NULL)
		return;
	if (pthread_create(&udp->threadId, 0, yang_run_rtcudp_thread, udp)) {
		yang_error("YangThread::start could not start thread");

	}
}
void yang_stop_rtcudp(YangUdpHandle *udp) {
	if (udp == NULL)
		return;
	udp->isLoop = 0;
	while (udp->isStart)
		yang_usleep(1000);
}
