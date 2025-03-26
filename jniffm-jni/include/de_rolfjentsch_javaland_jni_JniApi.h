/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class de_rolfjentsch_javaland_jni_JniApi */

#ifndef _Included_de_rolfjentsch_javaland_jni_JniApi
#define _Included_de_rolfjentsch_javaland_jni_JniApi
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     de_rolfjentsch_javaland_jni_JniApi
 * Method:    initHandle
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lde/rolfjentsch/javaland/ApiStatus;)I
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_initHandle
  (JNIEnv *, jobject, jstring, jstring, jobject);

/*
 * Class:     de_rolfjentsch_javaland_jni_JniApi
 * Method:    insert
 * Signature: (ILde/rolfjentsch/javaland/ApiInstrumentData;Lde/rolfjentsch/javaland/ApiStatus;)I
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_insert
  (JNIEnv *, jobject, jint, jobject, jobject);

/*
 * Class:     de_rolfjentsch_javaland_jni_JniApi
 * Method:    update
 * Signature: (ILde/rolfjentsch/javaland/ApiInstrumentData;Lde/rolfjentsch/javaland/ApiStatus;)I
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_update
  (JNIEnv *, jobject, jint, jobject, jobject);

/*
 * Class:     de_rolfjentsch_javaland_jni_JniApi
 * Method:    delete
 * Signature: (ILjava/lang/String;Lde/rolfjentsch/javaland/ApiStatus;)I
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_delete
  (JNIEnv *, jobject, jint, jstring, jobject);

/*
 * Class:     de_rolfjentsch_javaland_jni_JniApi
 * Method:    get
 * Signature: (ILjava/lang/String;Lde/rolfjentsch/javaland/ApiStatus;)I
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_get
  (JNIEnv *, jobject, jint, jstring, jobject);

/*
 * Class:     de_rolfjentsch_javaland_jni_JniApi
 * Method:    getVersion
 * Signature: ([I)[I
 */
JNIEXPORT jintArray JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_getVersion
  (JNIEnv *, jobject, jintArray);

#ifdef __cplusplus
}
#endif
#endif
