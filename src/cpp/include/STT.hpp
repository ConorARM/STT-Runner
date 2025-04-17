//
// SPDX-FileCopyrightText: Copyright 2024-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
//
// SPDX-License-Identifier: Apache-2.0
//

#ifndef STT_STT_HPP
#define STT_STT_HPP

#include <string>

/**
 * Interface to STT(Speech to text)
 * Contains generic STT functions
 * Should invoke whatever implementation this is initialised with
 */
template<typename T>
class STT {
private:
public:
    /**
     * Function to load the chosen STT model and to init the context
     * @tparam P stt context type
     * @param pathToModel path to the model location
     * @return stt context pointer
     */
    template<typename P>
    P *InitContext(const char *pathToModel)
    {
        return ((T *) this)->InitContext(pathToModel);
    }

    /**
     * Function to free the previously initialised stt context
     * @tparam P stt context type
     * @param contextPtr stt context pointer
     */
    template<typename P>
    void FreeContext(P* contextPtr)
    {
        ((T *) this)->FreeContext(contextPtr);
    }

    /**
     * The entire transcription inference loop
     * @tparam P stt context type
     * @param contextPtr stt context pointer
     * @param numThreads number of threads to use
     * @param audioData  audio data to transcribe
     * @param audioDataLength length of the Audio data supplied
     */
    template<typename P>
    std::string FullTranscribe(P* contextPtr, int numThreads, float* audioData, int audioDataLength)
    {
        return ((T *) this)->FullTranscribe(contextPtr, numThreads, audioData, audioDataLength);
    }
};
#endif //STT_STT_HPP
