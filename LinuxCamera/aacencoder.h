#ifndef AACENCODER_H
#define AACENCODER_H
#include <vector>
#include "comdef.h"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
class CAACEncoder
{
public:
    CAACEncoder();
    ~CAACEncoder();
    int InitAAC(int channels, int sample_rate, int bit_rate);

    void DeInit();  // 释放资源
    AVPacket *Encode(AVFrame *frame, int stream_index, int64_t pts, int64_t time_base);
    int Encode(AVFrame *farme, int stream_index, int64_t pts, int64_t time_base,
               std::vector<AVPacket *> &packets);
    int GetFrameSize(); // 获取一帧数据 每个通道需要多少个采样点
    int GetSampleFormat();  // 编码器需要的采样格式
    AVCodecContext *GetCodecContext();
    int GetChannels();
    int GetSampleRate();
private:
    int m_channels = 2;
    int m_sampleRate = 44100;
    int m_bitRate = 128*1024;
    int64_t m_pts = 0;
    AVCodecContext * m_codecCtx = NULL;
};

#endif // AACENCODER_H
