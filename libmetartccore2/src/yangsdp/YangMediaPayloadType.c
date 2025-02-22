#include <yangsdp/YangMediaPayloadType.h>
#include <yangutil/yangtype.h>
#include <yangutil/sys/YangCString.h>
#include <string.h>
#define kCRLF  "\r\n"
yang_impl_struct_init(YangMediaPayloadType)
yang_impl_struct_destroy(YangMediaPayloadType)
yang_impl_struct_insert(YangMediaPayloadType)
int32_t yang_encode_mediapayloadtype(YangMediaPayloadType *payload,
		YangBuffer *os) {
	int32_t err = Yang_Ok;
	char intstr[32];
	memset(intstr, 0, sizeof(intstr));
	yang_itoa(payload->payload_type, intstr, 10);
	yang_write_cstring(os, "a=rtpmap:");
	yang_write_cstring(os, intstr);
	yang_write_cstring(os, " ");
	yang_write_cstring(os, payload->encoding_name);
	yang_write_cstring(os, "/");
	memset(intstr, 0, sizeof(intstr));
	yang_itoa(payload->clock_rate, intstr, 10);
	yang_write_cstring(os, intstr);

	if (strlen(payload->encoding_param)) {
		yang_write_cstring(os, "/");
		yang_write_cstring(os, payload->encoding_param);

	}
	yang_write_cstring(os, kCRLF);



	for (int32_t i = 0; i < payload->rtcp_fb.vlen; i++) {
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(payload->payload_type, intstr, 10);
		//yang_write_cstring(os, intstr);
		yang_write_cstring(os, "a=rtcp-fb:");
		yang_write_cstring(os, intstr);
		yang_write_cstring(os, " ");
		yang_write_cstring(os, payload->rtcp_fb.payload[i]);
		yang_write_cstring(os, kCRLF);
	}
	if (strlen(payload->format_specific_param)) {
		yang_write_cstring(os, "a=fmtp:");
		memset(intstr, 0, sizeof(intstr));
		yang_itoa(payload->payload_type, intstr, 10);
		yang_write_cstring(os, intstr);
		yang_write_cstring(os, " ");
		yang_write_cstring(os, payload->format_specific_param);
		yang_write_cstring(os, kCRLF);

		// TODO: FIXME: Remove the test code bellow.
		// << ";x-google-max-bitrate=6000;x-google-min-bitrate=5100;x-google-start-bitrate=5000"

	}

	return err;
}

