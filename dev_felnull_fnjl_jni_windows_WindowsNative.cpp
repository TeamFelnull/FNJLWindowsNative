#include "dev_felnull_fnjl_jni_windows_WindowsNative.h"
#include <shlobj.h>
#include <tchar.h>
#include <cstring>

//https://www.wabiapp.com/WabiSampleSource/windows/sh_get_special_folder_path.html
JNIEXPORT jstring

JNICALL Java_dev_felnull_fnjln_jni_windows_WindowsNative_getSpecialFolderPath(JNIEnv *env, jclass, jint num) {
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

JNIEXPORT jstring

JNICALL Java_dev_felnull_fnjln_jni_windows_WindowsNative_getSystemFontFaceName(JNIEnv *env, jclass, jint num) {
    LOGFONT iconFont = getIconTitleFont(num);
    return env->NewStringUTF(iconFont.lfFaceName);
}

//https://www.ne.jp/asahi/hishidama/home/tech/java/jni_code.html
//http://yamatyuu.net/computer/program/sdk/common_dialog/GetOpenFileName/index.html
//http://wisdom.sakura.ne.jp/system/winapi/common/common6.html
BOOL
openFileName(HWND hWnd, TCHAR *fileName, int sz, const char *initDir, const char *filter, const char *title,
             const char *defExt, int filterIndex, int flags) {
    OPENFILENAME of;
    ZeroMemory(&of, sizeof(of));
    of.lStructSize = sizeof(of);
    of.hwndOwner = hWnd;
    of.lpstrInitialDir = initDir;
    of.lpstrFile = fileName;
    of.nMaxFile = sz;
    of.lpstrFilter = _TEXT(filter);
    of.lpstrDefExt = defExt;
    of.lpstrTitle = title;
    of.nFilterIndex = filterIndex;
    of.Flags = flags;
    return GetOpenFileName(&of);
}

//https://muimi.com/j/jni/
char *getNoGarbledString(JNIEnv *env, jstring str) {
    jstring enc = env->NewStringUTF("Shift_JIS");
    jclass clazz = env->FindClass("java/lang/String");
    jmethodID getBytes = env->GetMethodID(clazz, "getBytes", "(Ljava/lang/String;)[B");
    auto bytes = (jbyteArray)
            env->CallObjectMethod(str, getBytes, enc);
    jsize len = env->GetArrayLength(bytes);
    char *s = (char *) malloc(len + 1);
    jbyte *bs = env->GetByteArrayElements(bytes, NULL);
    memcpy(s, bs, len);
    env->ReleaseByteArrayElements(bytes, bs, 0);
    s[len] = '\0';
    return s;
}

JNIEXPORT jbyteArray
JNICALL Java_dev_felnull_fnjln_jni_windows_WindowsNative_getOpenFileName
        (JNIEnv *env, jclass, jlong hwndId, jstring jTitle, jstring jInitDir, jstring jInitName, jstring jDefExt,
         jstring jFilter, jint filterIndex, jint flags) {
    HWND hWnd = reinterpret_cast<HWND>(hwndId);

    char *title = getNoGarbledString(env, jTitle);
    char *initDir = getNoGarbledString(env, jInitDir);
    char *initName = getNoGarbledString(env, jInitName);
    char *defExt = getNoGarbledString(env, jDefExt);
    char *filter = getNoGarbledString(env, jFilter);

    TCHAR fileName[MAX_PATH * 256];

    for (int i = 0; i < strlen(initName); ++i) {
        fileName[i] = initName[i];
    }

    if (fileName[0] == NULL)
        fileName[0] = _T('\0');

    openFileName(hWnd, fileName, sizeof(fileName) / sizeof(TCHAR), initDir, filter, title, defExt, filterIndex, flags);

    free(title);
    free(initDir);
    free(initName);
    free(defExt);
    free(filter);

    int ct = 0;
    int nct = 0;
    for (char i: fileName) {
        if (i == NULL)
            nct++;
        else
            nct = 0;
        ct++;
        if (nct >= 2)
            break;
    }

    jbyteArray ret = env->NewByteArray(ct);
    jbyte *b = env->GetByteArrayElements(ret, nullptr);
    for (int i = 0; i < ct; ++i) {
        b[i] = (jbyte) fileName[i];
    }

    env->ReleaseByteArrayElements(ret, b, 0);
    return ret;
}
