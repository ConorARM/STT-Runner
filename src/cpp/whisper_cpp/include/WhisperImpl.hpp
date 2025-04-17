//
// SPDX-FileCopyrightText: Copyright 2024-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
//
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "whisper.h"
#include "STT.hpp"
#include <string>

#ifndef STT_WHISPER_IMPL_HPP
#define STT_WHISPER_IMPL_HPP

/**
* @brief Whisper Implementation of our STT API
*
*/
class WhisperImpl : public STT<WhisperImpl> {
private:
    /**
     * Function to retrieve the total number of text segments
     * @param contextPtr whisper_context pointer
     * @return segment count
     */
    int GetTextSegmentCount(whisper_context* contextPtr)
    {
        return whisper_full_n_segments(contextPtr);
    }

    /**
     * Function to retrieve the text segment at a given index
     * @param contextPtr whisper_context pointer
     * @param index index of the text segment
     * @return text segment
     */
    const char * GetTextSegment(whisper_context* contextPtr, int index)
    {
        const char *text = whisper_full_get_segment_text(contextPtr, index);
        return text;
    }

    /**
     * The transcription inference loop, inspired by an existing whisper.cpp example
     * @param contextPtr whisper_context pointer
     * @param numThreads number of threads to use
     * @param audioDataPtr pointer to audio data to transcribe
     * @param audioDataLength length of the audio data array
     */
    void Transcribe(whisper_context* contextPtr, const int numThreads, const float* audioDataPtr, const int audioDataLength)
    {
        whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
        params.print_realtime = true;
        params.print_progress = false;
        params.print_timestamps = true;
        params.print_special = false;
        params.translate = false;
        params.language = "en";
        params.n_threads = numThreads;
        params.offset_ms = 0;
        params.no_context = true;
        params.single_segment = false;

        whisper_full(contextPtr, params, &audioDataPtr[0], audioDataLength);
        whisper_reset_timings(contextPtr);
        whisper_print_timings(contextPtr);
    }

public:
    WhisperImpl() = default;

    /**
    * Function to load the chosen STT model and to init the context
    * @param pathToModel path to the model location
    * @return whisper_context pointer
    */
    whisper_context* InitContext(const char *pathToModel)
    {
        whisper_context *context = whisper_init_from_file_with_params(pathToModel, whisper_context_default_params());
        return context;
    }

    /**
     * Function to free the previously initialised whisper_context
     * @param contextPtr whisper_context pointer
     */
    void FreeContext(whisper_context* contextPtr)
    {
        whisper_free(contextPtr);
    }

    /**
    * The full transcription inference loop, and retrieval of all text segments
    * Taken from whisper.cpp/examples/whisper.android/lib/src/main/jni/whisper/jni.c and slightly modified
    * @param contextPtr whisper_context pointer
    * @param  numThreads number of threads to use
    * @param  audioDataPtr  pointer to audio data to transcribe
    * @param audioDataLength length of the audio data array
    */
    std::string FullTranscribe(whisper_context* contextPtr, const int numThreads, const float* audioDataPtr, const int audioDataLength)
    {
        Transcribe(contextPtr, numThreads, audioDataPtr, audioDataLength);

        int count = GetTextSegmentCount(contextPtr);

        std::string transcribed;
        for(int i = 0; i < count; i++)
        {
            transcribed += GetTextSegment(contextPtr, i);
        }
        return transcribed;
    }
};

#endif //STT_WHISPER_IMPL_HPP
