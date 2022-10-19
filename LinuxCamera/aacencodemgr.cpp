#include "aacencodemgr.h"

AACEncodeMgr::AACEncodeMgr(QObject *parent) : QObject(parent)
{

}

void AACEncodeMgr::Init(AudioParam param)
{
    int ret =  m_encode.InitAAC(param.channel, param.rate, param.nb_samples*128);
    if(ret < 0)
    {
        LOG("audio_encoder.InitAAC failed\n");
        return;
    }

    int pcm_frame_size = av_get_bytes_per_sample((AVSampleFormat)param.pcmFormat)*param.channel * m_encode.GetFrameSize();
    if(pcm_frame_size <= 0)
    {
        LOG("pcm_frame_size <= 0\n");
        return ;
    }
    m_pcmBuff.start  =  malloc(pcm_frame_size);
    if(!m_pcmBuff.start)
    {
        LOG("malloc(pcm_frame_size)\n");
        return ;
    }
    m_pcmBuff.length = pcm_frame_size;
    m_param = param;

    // 初始化重采样

    ret = m_audioResampler.InitFromS16ToFLTP(param.channel, param.rate, m_encode.GetChannels(), m_encode.GetSampleRate());
    if(ret < 0)
    {
        LOG("audio_resampler.InitFromS16ToFLTP failed\n");
        return;
    }

}

bool AACEncodeMgr::encode(char *buff, int buffsize, char *out, int& outSize)
{
    return 0;
}
