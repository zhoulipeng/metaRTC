﻿

#ifndef SRC_YANGWEBRTC_YANGMEDIADESC_H_
#define SRC_YANGWEBRTC_YANGMEDIADESC_H_
#include <yangutil/buffer/YangBuffer.h>
#include <yangsdp/YangMediaPayloadType.h>
#include <yangsdp/YangSdpType.h>
#include <yangsdp/YangSSRCInfo.h>

typedef struct
{
    char ip_[32];
    int32_t port_;
    char type_[32];
}YangCandidate;
typedef struct{
    char ice_ufrag[16];
    char ice_pwd[128];
    char ice_options[128];
    char fingerprint_algo[128];
    char fingerprint[128];
    char setup[128];
}YangSessionInfo;

typedef struct{
	int32_t mapid;
	char extmap[128];
}YangExtmap;

yang_declare_struct(YangCandidate)
yang_declare_struct_init(YangCandidate)
yang_declare_struct_destroy(YangCandidate)
yang_declare_struct_insert(YangCandidate)




yang_declare_struct(YangExtmap)
yang_declare_struct_init(YangExtmap)
yang_declare_struct_destroy(YangExtmap)
yang_declare_struct_insert(YangExtmap)

typedef struct{
    YangSessionInfo session_info;
    char type[16];
    int32_t port;

    int32_t rtcp_mux;
    int32_t rtcp_rsize;

    int32_t sendonly;
    int32_t recvonly;
    int32_t sendrecv;
    int32_t inactive;

    char mid[64];
    char msid[64];
    char msid_tracker[64];
    char protos[64];
    YangMediaPayloadTypeVector payload_types;
    YangCandidateVector candidates;
    YangSSRCGroupVector ssrc_groups;
    YangSSRCInfoVector  ssrc_infos;
    YangExtmapVector extmaps;
}YangMediaDesc;

yang_declare_struct(YangMediaDesc)
yang_declare_struct_init(YangMediaDesc)
yang_declare_struct_destroy(YangMediaDesc)
yang_declare_struct_insert(YangMediaDesc)
void yang_init_mediadesc(YangMediaDesc* desc);
void yang_destroy_mediadesc(YangMediaDesc* desc);
int32_t yang_encode_sessionInfo(YangSessionInfo* info,YangBuffer* os);
int32_t yang_encode_mediadesc(YangMediaDesc* desc,YangBuffer* os);
int32_t yang_mediadesc_update_msid(YangMediaDesc* desc,char* id);
int32_t yang_sessioninfo_parse_attribute(YangSessionInfo* info, char* attribute, char* value);
int32_t yang_mediadesc_parse_line(YangMediaDesc* desc,char* line);

#endif /* SRC_YANGWEBRTC_YANGMEDIADESC_H_ */
