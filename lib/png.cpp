#include "png.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

namespace mandelbrot {

void save_png(const uint8_t* pixels, int width, int height,
              const char* filename) {
  // Register all formats and codecs
  avformat_network_init();

  // Allocate the AVFormatContext
  AVFormatContext* format_ctx = nullptr;
  avformat_alloc_output_context2(&format_ctx, nullptr, nullptr, filename);
  if (!format_ctx) {
    fprintf(stderr, "Could not create output context\n");
    return;
  }

  // Find the encoder for PNG
  const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_PNG);
  if (!codec) {
    fprintf(stderr, "Codec not found\n");
    avformat_free_context(format_ctx);
    return;
  }

  // Allocate the AVStream
  AVStream* stream = avformat_new_stream(format_ctx, nullptr);
  if (!stream) {
    fprintf(stderr, "Could not allocate stream\n");
    avformat_free_context(format_ctx);
    return;
  }

  // Set up codec context
  AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
  if (!codec_ctx) {
    fprintf(stderr, "Could not allocate codec context\n");
    avformat_free_context(format_ctx);
    return;
  }

  codec_ctx->codec_id = AV_CODEC_ID_PNG;
  codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
  codec_ctx->pix_fmt = AV_PIX_FMT_RGB24;
  codec_ctx->width = width;
  codec_ctx->height = height;
  codec_ctx->time_base = AVRational{1, 25};  // framerate

  if (format_ctx->oformat->flags & AVFMT_GLOBALHEADER)
    codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

  // Open the codec
  if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
    fprintf(stderr, "Could not open codec\n");
    avcodec_free_context(&codec_ctx);
    avformat_free_context(format_ctx);
    return;
  }

  // Set up the output file
  if (!(format_ctx->oformat->flags & AVFMT_NOFILE)) {
    if (avio_open(&format_ctx->pb, filename, AVIO_FLAG_WRITE) < 0) {
      fprintf(stderr, "Could not open output file '%s'\n", filename);
      avcodec_free_context(&codec_ctx);
      avformat_free_context(format_ctx);
      return;
    }
  }

  // Write the stream header
  if (avformat_write_header(format_ctx, nullptr) < 0) {
    fprintf(stderr, "Error occurred when writing header\n");
    avio_close(format_ctx->pb);
    avcodec_free_context(&codec_ctx);
    avformat_free_context(format_ctx);
    return;
  }

  // Create the AVFrame and fill it with pixel data
  AVFrame* frame = av_frame_alloc();
  frame->format = codec_ctx->pix_fmt;
  frame->width = codec_ctx->width;
  frame->height = codec_ctx->height;
  av_image_alloc(frame->data, frame->linesize, frame->width, frame->height,
                 codec_ctx->pix_fmt, 1);

  // Copy pixel data to frame buffer (assuming RGB format)
  for (int y = 0; y < height; ++y) {
    memcpy(frame->data[0] + y * frame->linesize[0], pixels + y * width * 3,
           width * 3);
  }

  frame->pts = 0;

  // Encode the frame
  AVPacket pkt;
  av_init_packet(&pkt);
  pkt.data = nullptr;
  pkt.size = 0;

  int ret = avcodec_send_frame(codec_ctx, frame);
  if (ret < 0) {
    fprintf(stderr, "Error sending frame to encoder\n");
  }

  ret = avcodec_receive_packet(codec_ctx, &pkt);
  if (ret < 0) {
    fprintf(stderr, "Error receiving packet from encoder\n");
  }

  // Write the packet
  av_interleaved_write_frame(format_ctx, &pkt);

  // Free resources
  av_packet_unref(&pkt);
  av_frame_free(&frame);
  avcodec_free_context(&codec_ctx);
  avformat_close_input(&format_ctx);
  avformat_free_context(format_ctx);
  avformat_network_deinit();
}

}  // namespace mandelbrot
