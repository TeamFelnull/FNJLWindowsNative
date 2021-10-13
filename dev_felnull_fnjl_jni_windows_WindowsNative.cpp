#include "dev_felnull_fnjl_jni_windows_WindowsNative.h"
#include <shlobj.h>

JNIEXPORT jstring JNICALL Java_dev_felnull_fnjl_jni_windows_WindowsNative_getSpecialFolderPath
        (JNIEnv *env, jclass, jint num) {
    TCHAR path[MAX_PATH];
    SHGetSpecialFolderPath(NULL, path, num, 0);
    return env->NewStringUTF(path);
}
