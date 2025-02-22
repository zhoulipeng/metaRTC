/*
 * YangCNackBuffer.h
 *
 *  Created on: 2022年1月3日
 *      Author: yang
 */

#ifndef SRC_YANGRTP_YANGCRECEIVENACKBUFFER_H_
#define SRC_YANGRTP_YANGCRECEIVENACKBUFFER_H_


#include <stdint.h>
typedef struct YangReceiveNackBuffer{
    uint16_t begin;
        // The end iterator for ring buffer.
        // For example, when got 1 elems, the end is 1.
     uint16_t end;
	 uint16_t capacity;
	 uint16_t contexttialized;
	    // Ring bufer.
	 int32_t* queue;
	    // Increase one when uint16 flip back, for get_extended_highest_sequence.
	 uint64_t nn_seq_flip_backs;
	    // Whether initialized, because we use uint16 so we can't use -1.
}YangReceiveNackBuffer;

void yang_init_nackBuffer(YangReceiveNackBuffer* nackbuf,int32_t capacity);
void yang_destroy_nackBuffer(YangReceiveNackBuffer* nackbuf);
int32_t yang_nackbuffer_empty(YangReceiveNackBuffer* nackbuf);
void yang_nackbuffer_notify_nack_list_full(YangReceiveNackBuffer* nackbuf);
void yang_nackbuffer_notify_drop_seq(YangReceiveNackBuffer* nackbuf,uint16_t seq);
int32_t yang_nackbuffer_update(YangReceiveNackBuffer* nackbuf,uint16_t seq);
int32_t yang_nackbuffer_update2(YangReceiveNackBuffer* nackbuf,uint16_t seq, uint16_t* nack_first, uint16_t* nack_last);
void yang_nackbuffer_set(YangReceiveNackBuffer* nackbuf,uint16_t at, uint16_t pkt);
uint32_t yang_nackbuffer_get_extended_highest_sequence(YangReceiveNackBuffer* nackbuf);



#endif /* SRC_YANGRTP_YANGCRECEIVENACKBUFFER_H_ */
