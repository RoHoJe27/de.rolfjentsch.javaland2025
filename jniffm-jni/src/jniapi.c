/*
 * jniapi.c
 *
 * Implementierung für das JNI-API.
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */

#include <malloc.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "api.h"
#include "de_rolfjentsch_javaland_jni_JniApi.h"
#include "util.h"
#include "references.h"
#define FREE(P) if ( P != NULL ) {free ((void *)P); P = NULL;}

#define MSG_NONE 0
#define MSG_STATUS 1
#define MSG_INSTDATA 2
#define MSG_TIMEOUT 3

pthread_mutex_t msg_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t msg_cond = PTHREAD_COND_INITIALIZER;
int msg_code = 0;
char *msg_text = NULL;
int msg_received = MSG_NONE;
InstrumentData *msg_data = NULL;

static void* stralloccopy(const char *source) {
	if (source == NULL) {
		return NULL;
	}
	int len = strlen(source) + 1;
	void *ret = malloc(len);
	memcpy(ret, source, len);
	return ret;
}


/*
 * Liefert die interne Versionsnummer der Bibliothek.
 */
JNIEXPORT jintArray JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_getVersion(
		JNIEnv *env, jobject o, jintArray outJNIArray) {
	jint major, minor, release;
	getApiVersion(&major, &minor, &release);
	jint outCArray[] = { major, minor, release };
	(*env)->SetIntArrayRegion(env, outJNIArray, 0, 3, outCArray);
	return outJNIArray;
}

/*
 * Callbackfunktion für das Setzen des Status (INSERT,UPDATE,DELETE und READ mit Fehler
 */
static void statusCallback(API_HANDLE handle, int code, char const *msg) {
	pthread_mutex_lock(&msg_lock);
	msg_code = code;
	msg_text = stralloccopy(msg);
	msg_received = MSG_STATUS;
	msg_data = NULL;
	pthread_mutex_unlock(&msg_lock);
	pthread_cond_signal(&msg_cond);
}

/*
 * Callbackfunktion für das erfolgreiche Lesen von Daten.
 */
static void instrumentDataCallback(API_HANDLE handle, InstrumentData *msg) {
	pthread_mutex_lock(&msg_lock);
	msg_code = API_OK;
	msg_text = NULL;
	msg_received = MSG_INSTDATA;
	msg_data = malloc(sizeof (InstrumentData));
	memcpy(msg_data,msg,sizeof (InstrumentData));
	pthread_mutex_unlock(&msg_lock);
	pthread_cond_signal(&msg_cond);
}

/*
 * Besorgt einen Handle und registriert bei Erfolg die Callbacks.
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_initHandle(
		JNIEnv *env, jobject thisObj, jstring jUsername, jstring jPassword, jobject status) {
	initMethodReferences(env);
	signed char *cUsername = toCString(env, thisObj, jUsername);
	signed char *cPassword = toCString(env, thisObj, jPassword);
	jint handle = createHandle(cUsername, cPassword);
	(*env)->CallVoidMethod(env, status, apiStatusSetCode, getLastErrorCode());
	(*env)->CallVoidMethod(env, status, apiStatusSetMessage,
			toJavaString(env,thisObj,getErrorMessage(getLastErrorCode())));
	if (handle > 0) {
		registerStatusCallback(handle, statusCallback);
		registerInstrumentCallback(handle, instrumentDataCallback);
	}
	FREE(cUsername);
	FREE(cPassword);
	return handle;
}

/*
 * Wartet auf die asynchrone Verarbeitung und setzt die Daten in die entsprechende result-Objekt.
 */
static void handleWaitMessage(JNIEnv *env, jobject apiObj, jobject result) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 15;
	int rc = 0;
	/*
	 * Darauf warten, dass die Empfangsthread eine Message eingetragen hat,
	 * oder das Timeout von 15 Sekunden abgelaufen.
	 */
	while ( msg_received == MSG_NONE && rc == 0 )
	{
		rc = pthread_cond_timedwait(&msg_cond,&msg_lock,&ts);
	}
	if ( rc == 0 )
	{
		(*env)->CallVoidMethod(env, result, apiStatusSetCode, msg_code);
		if (msg_text != NULL) {
			/* Die Message enthält einen Text. */
			(*env)->CallVoidMethod(env, result, apiStatusSetMessage,
					toJavaString(env, apiObj, msg_text));
			FREE(msg_text);
		}
		if (msg_data != NULL)
		{
			jobject jInstData = (*env)->CallObjectMethod(env,result,apiInstNew);
			(*env)->CallVoidMethod(env,jInstData,apiInstSetInstId,toJavaString(env, apiObj, msg_data->instId));
			(*env)->CallVoidMethod(env,jInstData,apiInstSetInstName,toJavaString(env, apiObj, msg_data->instName));
			(*env)->CallVoidMethod(env,jInstData,apiInstSetInstType,toIntegerObject(env,msg_data->instType));
			(*env)->CallVoidMethod(env,jInstData,apiInstSetMaxVolume,toDoubleObject(env,msg_data->maxVolume));
			(*env)->CallObjectMethod(env,result,apiStatusSetInstrumentData,jInstData);
		}
	} else {
		(*env)->CallVoidMethod(env, result, apiStatusSetCode, 300);
		(*env)->CallVoidMethod(env, result, apiStatusSetMessage,
							toJavaString(env, apiObj, "Timeout"));
	}
}

/*
 * Führt den Insert durch.
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_insert(
		JNIEnv *env, jobject thisObj, jint handle, jobject instrumentData, jobject status) {
	char const *instId = toCString(env, thisObj, (*env)->CallObjectMethod(env,instrumentData,apiInstGetInstId));
	char const *instName = toCString(env, thisObj, (*env)->CallObjectMethod(env,instrumentData,apiInstGetInstName));
	unsigned int instType = toInteger(env, (*env)->CallObjectMethod(env,instrumentData,apiInstGetInstType));
	double maxVolume = toDouble(env,(*env)->CallObjectMethod(env,instrumentData,apiInstGetMaxVolume));

	pthread_mutex_lock(&msg_lock);
	msg_received = MSG_NONE;


	int result = instrumentCreate(handle, instId, instName, instType,
			maxVolume);
	if (result == API_OK) {
		handleWaitMessage(env,thisObj, status);
	} else {
		(*env)->CallVoidMethod(env, status, apiStatusSetCode,result);
		jstring javaMessage = toJavaString(env,thisObj,getErrorMessage(result));
		(*env)->CallVoidMethod(env, status, apiStatusSetMessage,javaMessage);
	}
	pthread_mutex_unlock(&msg_lock);
	FREE(instId);
	FREE(instName);
	return result;
}

/*
 * Führt das Update durch
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_update(
		JNIEnv *env, jobject thisObj, jint handle, jobject instrumentData, jobject status) {
	char const *instId = toCString(env, thisObj, (*env)->CallObjectMethod(env,instrumentData,apiInstGetInstId));
	jobject instNameObject = (*env)->CallObjectMethod(env,instrumentData,apiInstGetInstName);
	char const *instName = toCString(env, thisObj, instNameObject);
	jobject instTypeObject = (*env)->CallObjectMethod(env,instrumentData,apiInstGetInstType);
	unsigned int instType = toInteger(env,instTypeObject );
	jobject maxVolumeObject = (*env)->CallObjectMethod(env,instrumentData,apiInstGetMaxVolume);
	double maxVolume = toDouble(env,maxVolumeObject);

	pthread_mutex_lock(&msg_lock);
	msg_received = MSG_NONE;

	int result = instrumentUpdate(
			handle, instId,
			instName, instNameObject != NULL,
			instType, instTypeObject != NULL,
			maxVolume, maxVolumeObject != NULL);
	if (result == API_OK) {
		handleWaitMessage(env,thisObj, status);
	} else {
		(*env)->CallVoidMethod(env, status, apiStatusSetCode,result);
		jstring javaMessage = toJavaString(env,thisObj,getErrorMessage(result));
		(*env)->CallVoidMethod(env, status, apiStatusSetMessage,javaMessage);
	}
	pthread_mutex_unlock(&msg_lock);
	FREE(instId);
	FREE(instName);
	return result;
}

/*
 * Führt das Delete durch
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_delete(
		JNIEnv *env, jobject thisObj, jint handle, jstring instIdStr, jobject status) {

	pthread_mutex_lock(&msg_lock);
	msg_received = MSG_NONE;
	char const *instId = toCString(env, thisObj, instIdStr);

	int result = instrumentDelete(
			handle, instId);
	if (result == API_OK) {
		handleWaitMessage(env,thisObj, status);
	} else {
		(*env)->CallVoidMethod(env, status, apiStatusSetCode,result);
		jstring javaMessage = toJavaString(env,thisObj,getErrorMessage(result));
		(*env)->CallVoidMethod(env, status, apiStatusSetMessage,javaMessage);
	}
	pthread_mutex_unlock(&msg_lock);
	FREE(instId);
	return result;
}

/*
 * Führt das get durch.
 */
JNIEXPORT jint JNICALL Java_de_rolfjentsch_javaland_jni_JniApi_get(
		JNIEnv *env, jobject thisObj, jint handle, jstring instIdStr, jobject status) {

	pthread_mutex_lock(&msg_lock);
	msg_received = MSG_NONE;
	char const *instId = toCString(env, thisObj, instIdStr);

	int result = instrumentRead(
			handle, instId);
	if (result == API_OK) {
		handleWaitMessage(env,thisObj, status);
	} else {
		(*env)->CallVoidMethod(env, status, apiStatusSetCode,result);
		jstring javaMessage = toJavaString(env,thisObj,getErrorMessage(result));
		(*env)->CallVoidMethod(env, status, apiStatusSetMessage,javaMessage);
	}
	pthread_mutex_unlock(&msg_lock);
	FREE(instId);
	return result;
}

