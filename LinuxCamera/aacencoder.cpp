#include "aacencoder.h"

CAACEncoder::CAACEncoder()
{

}

CAACEncoder::~CAACEncoder()
{
    if(m_codecCtx)
    {
        DeInit();
    }
}

int CAACEncoder::InitAAC(int channels, int sample_rate, int bit_rate)
{
    m_channels = channels;
    m_sampleRate = sample_rate;
    m_bitRate = bit_rate;

    AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if(!codec)
    {
        LOG("avcodec_find_encoder AV_CODEC_ID_AAC failed\n");
        return -1;
    }
    m_codecCtx = avcodec_alloc_context3(codec);
    if(!m_codecCtx)
    {
        LOG("avcodec_alloc_context3 AV_CODEC_ID_AAC failed\n");
        return -1;
    }

    m_codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    m_codecCtx->bit_rate = m_bitRate;
    m_codecCtx->sample_rate = m_sampleRate;
    m_codecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    m_codecCtx->channels = m_channels;
    m_codecCtx->channel_layout = av_get_default_channel_layout(m_codecCtx->channels);

    int ret = avcodec_open2(m_codecCtx, NULL, NULL);
    if(ret != 0)
    {
        char errbuf[1024] = {0};
        av_strerror(ret, errbuf, sizeof(errbuf) - 1);
        LOG(QString("avcodec_open2 failed:%1\n").arg(errbuf));
        return -1;
    }
    LOG("InitAAC success\n");
    return 0;
}

void CAACEncoder::DeInit()
{
    if(m_codecCtx)
    {
        avcodec_free_context(&m_codecCtx);  // codec_ctx_被设置为NULL
    }
}

AVPacket *CAACEncoder::Encode(AVFrame *frame, int stream_index, int64_t pts, int64_t time_base)
{
    if(!m_codecCtx)
    {
        LOG("codec_ctx_ null\n");
        return NULL;
    }
    pts = av_rescale_q(pts, AVRational{1, (int)time_base}, m_codecCtx->time_base);
    if(frame)
    {
        frame->pts = pts;
    }
    int ret = avcodec_send_frame(m_codecCtx, frame);
    if(ret != 0)
    {
        char errbuf[1024] = {0};
        av_strerror(ret, errbuf, sizeof(errbuf) - 1);
        LOG(QString("avcodec_send_frame failed:%s\n").arg( errbuf));
        return NULL;
    }
    AVPacket *packet = av_packet_alloc();
    ret = avcodec_receive_packet(m_codecCtx, packet);
    if(ret != 0)
    {
        char errbuf[1024] = {0};
        av_strerror(ret, errbuf, sizeof(errbuf) - 1);
        LOG(QString("aac avcodec_receive_packet failed:%1\n").arg( errbuf));
        av_packet_free(&packet);
        return NULL;
    }
    packet->stream_index = stream_index;
    return packet;
}

int CAACEncoder::Encode(AVFrame *frame, int stream_index, int64_t pts, int64_t time_base,
                         std::vector<AVPacket *> &packets)
{
    if(!m_codecCtx)
    {
        printf("codec_ctx_ null\n");
        return NULL;
    }
    pts = av_rescale_q(pts, AVRational{1, (int)time_base}, m_codecCtx->time_base);
    if(frame)
    {
        frame->pts = pts;
    }
    int ret = avcodec_send_frame(m_codecCtx, frame);
    if(ret != 0)
    {
        char errbuf[1024] = {0};
        av_strerror(ret, errbuf, sizeof(errbuf) - 1);
        printf("avcodec_send_frame failed:%s\n", errbuf);
        return NULL;
    }
    while(1)
    {
        AVPacket *packet = av_packet_alloc();
        ret = avcodec_receive_packet(m_codecCtx, packet);
        packet->stream_index = stream_index;
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            ret = 0;
            av_packet_free(&packet);
            break;
        }
        else if (ret < 0)
        {
            char errbuf[1024] = {0};
            av_strerror(ret, errbuf, sizeof(errbuf) - 1);
            printf("aac avcodec_receive_packet failed:%s\n", errbuf);
            av_packet_free(&packet);
            ret = -1;
        }
        packets.push_back(packet);
    }
    return ret;
}

int CAACEncoder::GetFrameSize()
{
    if(m_codecCtx)
        return m_codecCtx->frame_size;
    return 0;
}

int CAACEncoder::GetSampleFormat()
{
    if(m_codecCtx)
        return m_codecCtx->sample_fmt;

    return -1;  // AV_SAMPLE_FMT_NONE
}

AVCodecContext *CAACEncoder::GetCodecContext()
{
    return m_codecCtx;
}

int CAACEncoder::GetChannels()
{
    if(m_codecCtx)
        return m_codecCtx->channels;

    return -1;
}


int CAACEncoder::GetSampleRate()
{
    if(m_codecCtx)
        return m_codecCtx->sample_rate;

    return -1;
}
