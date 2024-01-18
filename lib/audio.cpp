#include <iostream>
#include <portaudio.h>
#include <sndfile.h>
#include <thread>

#define FRAMES_PER_BUFFER (512)

struct callback_data_s
{
    SNDFILE *file;
    SF_INFO info;
};

int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    float *out;
    callback_data_s *p_data = static_cast<callback_data_s *>(userData);
    sf_count_t num_read;

    out = static_cast<float *>(output);
    p_data = static_cast<callback_data_s *>(userData);

    /* clear output buffer */
    memset(out, 0, sizeof(float) * frameCount * p_data->info.channels);

    /* read directly into the output buffer */
    num_read = sf_read_float(p_data->file, out, frameCount * p_data->info.channels);

    /* If we couldn't read a full frameCount of samples, we've reached EOF */
    if (num_read < frameCount)
    {
        if (p_data->info.frames > 0 && num_read < frameCount)
        {
            // If looping is enabled, rewind the file to the beginning
            if (sf_seek(p_data->file, 0, SEEK_SET) == -1)
            {
                std::cerr << "Error seeking to the beginning of the file." << std::endl;
                return paComplete;
            }
        }
        else
        {
            return paComplete;
        }
    }

    return paContinue;
}

void playMusic(const char *filePath)
{
    PaStream *stream;
    PaError error;
    callback_data_s data;

    data.file = sf_open(filePath, SFM_READ, &data.info);
    if (sf_error(data.file) != SF_ERR_NO_ERROR)
    {
        std::cerr << sf_strerror(data.file) << std::endl;
        std::cerr << "File: " << filePath << std::endl;
        return;
    }

    error = Pa_Initialize();
    if (error != paNoError)
    {
        std::cerr << "Problem initializing PortAudio" << std::endl;
        return;
    }

    error = Pa_OpenDefaultStream(&stream, 0 /* no input */,
                                 data.info.channels /* stereo out */,
                                 paFloat32 /* floating point */,
                                 data.info.samplerate, FRAMES_PER_BUFFER, callback, &data);
    if (error != paNoError)
    {
        std::cerr << "Problem opening Default Stream" << std::endl;
        Pa_Terminate();
        return;
    }

    error = Pa_StartStream(stream);
    if (error != paNoError)
    {
        std::cerr << "Problem starting Stream" << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return;
    }

    // Calculate the duration in seconds
    double audioDuration = static_cast<double>(data.info.frames) / data.info.samplerate;

    PaTime startTime = Pa_GetStreamTime(stream);

    // Run until the stream is no longer active or the specified duration has elapsed
    while (Pa_IsStreamActive(stream) && (Pa_GetStreamTime(stream) - startTime) < audioDuration)
    {
        Pa_Sleep(100); // Sleep for a short duration to avoid busy-waiting
    }

    sf_close(data.file);
    Pa_CloseStream(stream);
    Pa_Terminate();
}

void playingMusic(const char *filePath)
{
    if (PLAY_MUSIC_ON)
    {
        std::thread newThread(playMusic, filePath); //Process en parallèle 
        newThread.detach();        // Detach the thread to allow it to run independently
        newThread.native_handle(); // Get the native handle of the thread (optional)
    }
}