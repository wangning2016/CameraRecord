#include "caudiodevice.h"
#include "comdef.h"

CAudioDevice::CAudioDevice(QObject *parent) : QThread(parent)
{
    pcm_st = NULL;
    m_stop = true;
    m_userData = NULL;
}

CAudioDevice::~CAudioDevice()
{
    Stop();
}

bool CAudioDevice::Open(AudioParam audioParam, pf_AVDataCallBack callBack,void* userData)
{
    Stop();
    m_userData = userData;
    m_avCallBack = callBack;
    m_param = audioParam;
    int ret = snd_pcm_open(&pcm_st, m_param.devPath.c_str(), SND_PCM_STREAM_CAPTURE, 0);
    if(ret != 0)
    {
        LOG(QString("snd_pcm_open fail ret=%1").arg(snd_strerror(ret)));
        return false;
    }
    snd_pcm_hw_params_t *param;
    snd_pcm_hw_params_malloc(&param);
    snd_pcm_hw_params_any(pcm_st, param);

    snd_pcm_hw_params_set_access(pcm_st, param, SND_PCM_ACCESS_RW_INTERLEAVED);
    // 设置采样格式，声道数量，采样率
    snd_pcm_hw_params_set_format(pcm_st, param, (snd_pcm_format_t)audioParam.pcmFormat);
    snd_pcm_hw_params_set_channels(pcm_st, param, audioParam.channel);
    unsigned int sample_rate = audioParam.rate;
    int dir;
    snd_pcm_hw_params_set_rate_near(pcm_st, param, &sample_rate, &dir);

    unsigned int buffer_time;
    snd_pcm_hw_params_get_buffer_time_max(param, &buffer_time, 0);
    if (buffer_time > 500000)
    {
        buffer_time = 500000;
    }
    snd_pcm_hw_params_set_buffer_time_near(pcm_st, param, &buffer_time, 0);

    // 设置采样周期，采样一帧音频数据的时间
    int fps = m_param.rate/m_param.nb_samples;
    unsigned int period_time = 1000*1000/fps;
    snd_pcm_hw_params_set_period_time_near(pcm_st, param, &period_time, 0);

    // 3. 将设置初始化到pcm结构
    snd_pcm_hw_params(pcm_st, param);


    // 4. 初始化一个帧缓冲区
    snd_pcm_hw_params_get_period_size(param, &frames, &dir);
    LOG(QString("每帧采样点数：%1\n").arg(frames));

    int size = frames * GetSampleByte(m_param) * m_param.channel;  // 每个采样点2Bytes，size表示一帧大小
    if(size < 0)
    {
        LOG("GetSampByte fail");
        Close();
        return false;
    }
    m_pcm_buf.start = malloc(size);
    m_pcm_buf.length = size;
    if (m_pcm_buf.start == NULL)
    {
        Close();
        return false;
    }
    m_stop = false;
    start();
    return true;
}

void CAudioDevice::Stop()
{
    m_stop = true;
    wait();
}

void CAudioDevice::Close()
{
    if (pcm_st)
    {
        snd_pcm_drain(pcm_st);
        snd_pcm_close(pcm_st);
        pcm_st = NULL;
    }
    if (m_pcm_buf.start)
    {
        free(m_pcm_buf.start);
        m_pcm_buf.start = NULL;
        m_pcm_buf.length = 0;
    }
}

void CAudioDevice::run()
{
    if(m_param.channel == 0 || m_param.nb_samples == 0 || m_param.rate == 0)
    {
        m_stop = true;
        Close();
        return;
    }

    while (!m_stop)
    {
        int ret = snd_pcm_readi(pcm_st, m_pcm_buf.start, frames);
        if (ret == -EPIPE)
        {
            LOG("overrun occurred\n");
            ret = snd_pcm_prepare(pcm_st);
            if (ret < 0)
            {
                LOG("failed to recover form overrun\n");
                break;
            }
        }
        else if (ret < 0)
        {
            LOG(QString("error from read:%1\n").arg(snd_strerror(ret)));
            break;
        }
        else if (ret != (int)frames)
        {
            LOG(QString("short read, read %1 samples\n").arg(ret));
        }
        if(m_avCallBack)
        {
            m_avCallBack((char*)m_pcm_buf.start,m_pcm_buf.length,NULL,&m_param,m_userData);
        }
    }
    m_stop = true;
    Close();
}

int CAudioDevice::GetSampleByte(AudioParam param)
{
    if(param.pcmFormat < 0)
    {
        return param.pcmFormat;
    }
    else if(param.pcmFormat <= SND_PCM_FORMAT_U8)
    {
        return 1;
    }
    else if(param.pcmFormat <= SND_PCM_FORMAT_U16_BE)
    {
        return 2;
    }
    else if(param.pcmFormat <= SND_PCM_FORMAT_U24_BE)
    {
        return 3;
    }
    else if(param.pcmFormat <= SND_PCM_FORMAT_U32_BE)
    {
        return 4;
    }
    else
        return -1;
}
