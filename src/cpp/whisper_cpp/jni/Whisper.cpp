//
// SPDX-FileCopyrightText: Copyright 2024-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
//
// SPDX-License-Identifier: Apache-2.0
//

#include <jni.h>
#include "WhisperImpl.hpp"
#include "STT.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// Instantiating a Whisper type STT implementation
STT<WhisperImpl> stt;

/**
 * Initialize whisper context
 *
 * @param env JNI environment
 * @param modelPath path to the model file
 * @return context
 */
JNIEXPORT jlong JNICALL Java_com_arm_stt_Whisper_initContext
  (JNIEnv* env, jobject, jstring modelPath)
{
      whisper_context *context = nullptr;
      const char *model_path_chars = env->GetStringUTFChars(modelPath, nullptr);
      context = stt.InitContext<whisper_context>(model_path_chars);
      env->ReleaseStringUTFChars(modelPath, model_path_chars);
      return reinterpret_cast<jlong>(context);
}

/**
 * Free the resources associated with whisper context
 * @param contextPtr pointer to whisper context
 */
JNIEXPORT void JNICALL Java_com_arm_stt_Whisper_freeContext
  (JNIEnv*, jobject, jlong contextPtr)
{
    auto *context = reinterpret_cast<struct whisper_context *>(contextPtr);
    stt.FreeContext<whisper_context>(context);
}

/**
 * Full transcribe function to
 * @param env JNI environment
 * @param contextPtr pointer to whisper context
 * @param numThreads number of threads to use
 * @param audioData audio data to transcribe
 * @return full transcription
 */
JNIEXPORT jstring JNICALL Java_com_arm_stt_Whisper_fullTranscribe
  (JNIEnv *env, jobject, jlong contextPtr,
         jint numThreads, jfloatArray audioData)
{
    auto *context = reinterpret_cast<struct whisper_context *>(contextPtr);
    jfloat *audio_data_arr = env->GetFloatArrayElements(audioData, nullptr);
    const jsize audio_data_length = env->GetArrayLength(audioData);

    const std::string transcribed = stt.FullTranscribe(context, numThreads, audio_data_arr, audio_data_length);

    env->ReleaseFloatArrayElements(audioData, audio_data_arr, JNI_ABORT);
    return env->NewStringUTF(transcribed.c_str());
}

#ifdef __cplusplus
}
#endif
