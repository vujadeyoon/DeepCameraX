#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdio>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/delegates/gpu/delegate.h"
#include "torch/script.h"



#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, "Test11", __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Test12", __VA_ARGS__)


template <typename T>
void log(const char* m, T t) {
    std::ostringstream os;
    os << t << std::endl;
    ALOGI("%s %s", m, os.str().c_str());
}


// Example: load a tflite model using TF Lite C++ API
// Credit to https://github.com/ValYouW/crossplatform-tflite-object-detecion
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_tflitecxx_MainActivity_loadModelJNI(
        JNIEnv* env,
        jobject /* this */,
        jobject assetManager,
        jstring fileName
        ) {

    char* buffer = nullptr;
    long size = 0;
    const char* modelpath = env->GetStringUTFChars(fileName, 0);

    if (!(env->IsSameObject(assetManager, NULL))) {
        AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
        AAsset *asset = AAssetManager_open(mgr, modelpath, AASSET_MODE_UNKNOWN);
        assert(asset != nullptr);

        size = AAsset_getLength(asset);
        buffer = (char *) malloc(sizeof(char) * size);
        AAsset_read(asset, buffer, size);
        AAsset_close(asset);
    }

    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromBuffer(buffer, size);

    std::string status_error_1 = "ERROR_1";
    std::string status_error_2 = "ERROR_2";
    std::string status_error_3 = "ERROR_3";
    std::string str_opencv = CV_VERSION;

    bool is_gpu = true;
    assert(model != nullptr);
    if (!model) return env->NewStringUTF(status_error_1.c_str()); // temp

    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter);

    // NEW: Prepare GPU delegate.
    auto* delegate = TfLiteGpuDelegateV2Create(/*default options=*/nullptr);
    if (interpreter->ModifyGraphWithDelegate(delegate) != kTfLiteOk) return env->NewStringUTF(status_error_2.c_str());

    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    int rows = 120;
    int cols = 160;

    int num_test = 100;
    for (int i = 0; i < num_test; i++) {
      // Load an image
      cv::Mat mat_input(rows, cols, CV_32FC1);
      double mean = 0.0;
      double stddev = 1.0 / 3.0 + ((double) i / (double) num_test);
      randn(mat_input, cv::Scalar(mean), cv::Scalar(stddev));

      // Memcpy
      memcpy(interpreter -> typed_input_tensor<float>(0), mat_input.data, mat_input.total() * mat_input.elemSize());

      // Run inference
      if (interpreter->Invoke() != kTfLiteOk) return env->NewStringUTF(status_error_3.c_str());
    }

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();

    float time_inference = (float) std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / (float) num_test;

    free(buffer);

    // NEW: Prepare GPU delegate.
    TfLiteGpuDelegateV2Delete(delegate);

    torch::Tensor t = torch::randn({1, 3, 224, 224});
    log("input tensor:", t);

    __android_log_print(
            ANDROID_LOG_INFO,
            "Test13",
            " -- CUFOTapAttack::android_main(), ui == %d \n", 100);

    return env->NewStringUTF(std::to_string(time_inference).c_str());
    // return env->NewStringUTF(str_opencv.c_str());
}