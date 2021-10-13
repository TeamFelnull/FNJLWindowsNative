#include "dev_felnull_fnjl_jni_windows_WindowsNative.h"
#include <shlobj.h>

//https://www.wabiapp.com/WabiSampleSource/windows/sh_get_special_folder_path.html
JNIEXPORT jstring JNICALL Java_dev_felnull_fnjl_jni_windows_WindowsNative_getSpecialFolderPath
        (JNIEnv *env, jclass, jint num) {
    TCHAR path[MAX_PATH];
    SHGetSpecialFolderPath(NULL, path, num, 0);
    return env->NewStringUTF(path);
}

//http://www.neko.ne.jp/~freewing/software/windows_10_change_system_font/
//https://www.tokovalue.jp/NonClientMetrics_U.htm
LOGFONT getIconTitleFont(int num) {
    if (num == 0) {
        LOGFONT font;
        ZeroMemory(&font, sizeof(LOGFONT));
        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &font, 0);
        return font;
    } else {
        NONCLIENTMETRICS metrics;
        ZeroMemory(&metrics, sizeof(NONCLIENTMETRICS));
        metrics.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);
        if (num == 1)
            return metrics.lfMessageFont;
        else if (num == 2)
            return metrics.lfCaptionFont;
        else if (num == 3)
            return metrics.lfMenuFont;
        else if (num == 4)
            return metrics.lfSmCaptionFont;
        return metrics.lfStatusFont;
    }
}

JNIEXPORT jstring JNICALL Java_dev_felnull_fnjl_jni_windows_WindowsNative_getSystemFontFaceName
        (JNIEnv *env, jclass, jint num) {
    LOGFONT iconFont = getIconTitleFont(num);
    return env->NewStringUTF(iconFont.lfFaceName);
}