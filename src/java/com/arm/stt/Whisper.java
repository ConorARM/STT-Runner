//
// SPDX-FileCopyrightText: Copyright 2024-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
//
// SPDX-License-Identifier: Apache-2.0
//

package com.arm.stt;

/**
 * The Whisper class is used to access the underlying C++ implementation
 * of the Whisper API via the JNI.
 */
public class Whisper {
    // Used to load the 'STT_lib' library on application startup.
    static {
        System.loadLibrary("arm-stt-jni");
    }

    /**
    * Function to load the chosen Whisper model and to init the context
    *
    * @param  modelPath  path to the file on disk
    * @return pointer to the context object
    */
    public native long initContext(String modelPath);

    /**
    * Function to free the previously initialised whisper_context
    *
    * @param  contextPtr  pointer to the context object previously initialised
    */
    public native void freeContext(long contextPtr);

    /**
    * Function to run the entire transcription inference loop
    *
    * @param  contextPtr pointer to the context object previously initialised
    * @param  numThreads number of threads to use
    * @param  audioData  audio data to transcribe
    * @return transcribed string object
    */
    public native String fullTranscribe(long contextPtr, int numThreads, float[] audioData);
}
