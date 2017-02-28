#ifdef	__cplusplus
extern "C"
{
#endif
#include <math.h>

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/audio_fifo.h"


#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>


#ifdef __cplusplus
};
#endif
#define OUTPUT_YUV420P 1
int main(int argc,char** argv) {
	av_register_all();
	avdevice_register_all();

	AVFormatContext	*pFormatCtx_Video = NULL;
	AVInputFormat *ifmt = 0;
	AVDictionary *options = NULL;
	AVCodecContext	*pCodecCtx_Video;
	AVCodec			*pCodec_Video;
	AVFifoBuffer	*fifo_video = NULL;
	int VideoIndex;
	SwsContext *img_convert_ctx;
	int frame_size = 0;

	if (argc == 1)
	{
		ifmt = av_find_input_format("gdigrab");
	}else {
		ifmt = av_find_input_format("fbdev");
	}
	av_dict_set(&options, "framerate", "15", NULL);
	//av_dict_set(&options,"offset_x","20",0);
	//The distance from the top edge of the screen or desktop
	//av_dict_set(&options,"offset_y","40",0);
	//Video frame size. The default is to capture the full screen
	av_dict_set(&options, "video_size", "800x400", 0);
	if (avformat_open_input(&pFormatCtx_Video, "desktop", ifmt, &options) != 0)
	{
		printf("Couldn't open input stream.（无法打开视频输入流）\n");
		return 0;
	}
	if (avformat_find_stream_info(pFormatCtx_Video, NULL) < 0)
	{
		printf("Couldn't find stream information.（无法获取视频流信息）\n");
		return 0;
	}


	if (pFormatCtx_Video->streams[0]->codecpar->codec_type != AVMEDIA_TYPE_VIDEO)
	{
		printf("Couldn't find video stream information.（无法获取视频流信息）\n");
		return 0;
	}

	pCodecCtx_Video = avcodec_alloc_context3(NULL);
	if (pCodecCtx_Video == NULL)
	{
		printf("Could not allocate AVCodecContext\n");
		return 0;
	}
	avcodec_parameters_to_context(pCodecCtx_Video, pFormatCtx_Video->streams[0]->codecpar);

	pCodec_Video = avcodec_find_decoder(pCodecCtx_Video->codec_id);
	if (pCodec_Video == NULL)
	{
		printf("Codec not found.（没有找到解码器）\n");
		return 0;
	}
	if (avcodec_open2(pCodecCtx_Video, pCodec_Video, NULL) < 0)
	{
		printf("Could not open codec.（无法打开解码器）\n");
		return 0;
	}

	img_convert_ctx = sws_getContext(pCodecCtx_Video->width, pCodecCtx_Video->height, pCodecCtx_Video->pix_fmt,
		pCodecCtx_Video->width, pCodecCtx_Video->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	frame_size = avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height);
	//申请30帧缓存
	//fifo_video = av_fifo_alloc(30 * avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height));
	AVFrame *pFrame, *pFrameYUV;
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();
	int ret = 0, got_picture = 0;
	
	AVPacket packet;
	/* initialize packet, set data to NULL, let the demuxer fill it */
	av_init_packet(&packet);
	packet.data = NULL;
	packet.size = 0;
	//// Malloc the rgb frame.
	//uint8_t* buffer = (uint8_t*)av_malloc(frame_size * sizeof(uint8_t));

	uint8_t *src_data[4] = { 0 }, *yuv_data[4] = { 0 };
	int src_linesize[4],yuv_linesize[4];
	int yuv_bufsize;
	int width, height;
	enum AVPixelFormat pix_fmt;

	width = pCodecCtx_Video->width;
	height = pCodecCtx_Video->height;
	pix_fmt = pCodecCtx_Video->pix_fmt;
	ret = av_image_alloc(src_data, src_linesize, width, height, AV_PIX_FMT_YUV420P, 1);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate raw video buffer\n");
		return 0;
	}
	ret = av_image_alloc(yuv_data, yuv_linesize, width, height, AV_PIX_FMT_YUV420P, 1);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate raw video buffer\n");
		return 0;
	}
	yuv_bufsize = ret;

	//avpicture_fill((AVPicture*)pFrameYUV, yuv_data, AV_PIX_FMT_YUV420P, pCodecCtx_Video->width, pCodecCtx_Video->height);

#if OUTPUT_YUV420P
	FILE *fp_yuv = fopen("f:/test/output.yuv", "wb+");
#endif 
	for (;;) {
		if (av_read_frame(pFormatCtx_Video, &packet) >= 0) {
			if (packet.stream_index == 0) {
				ret = avcodec_decode_video2(pCodecCtx_Video, pFrame, &got_picture, &packet);
				if (ret < 0) {
					printf("Decode Error.\n");
					return 0;
				}
				if (got_picture) {
					sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, height, yuv_data, yuv_linesize);
					av_image_copy(src_data, src_linesize,
						(const uint8_t **)(yuv_data), yuv_linesize,
						AV_PIX_FMT_YUV420P, width, height);
					

#if OUTPUT_YUV420P    
					fwrite(src_data[0], 1, yuv_bufsize, fp_yuv);
#endif    

				}
			}
		}
		else {
			//Exit Thread  
		}
	}

	sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P   
	fclose(fp_yuv);
#endif   

	av_free(pFrameYUV);
	avcodec_close(pCodecCtx_Video);
	avformat_close_input(&pFormatCtx_Video);
	return 0;
}
void captureStop() {
}
void captureLink() {
	
}