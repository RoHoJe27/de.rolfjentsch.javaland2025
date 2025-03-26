/*
 * references.c
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */


#include <stdio.h>
#include "references.h"

/*
 * Diese Source definiert die Klassenobjekte und MethodenIDs um vom C-Code aus auf
 * Java-Objekte zuzugreifen.
 *
 * Die Funktion initMethodReferences initialisiert einmalig die Werte.
 */

/* Class Integer */
jclass integerCls;
jmethodID integerIntValue;
jmethodID integerValueOf;

/* Class Double */
jclass doubleCls;
jmethodID doubleDoubleValue;
jmethodID doubleValueOf;

/* Class JniApi */
jclass jniApiCls;
jmethodID getStringLatin1;
jmethodID getBytesLatin1;

/* class ApiInsturmentData */
jclass apiInstrumentDataCls;
jmethodID apiInstNew;
jmethodID apiInstGetInstId;
jmethodID apiInstSetInstId;
jmethodID apiInstGetInstName;
jmethodID apiInstSetInstName;
jmethodID apiInstGetInstType;
jmethodID apiInstSetInstType;
jmethodID apiInstGetMaxVolume;
jmethodID apiInstSetMaxVolume;

/* class ApiStatus */
jclass apiStatusCls;
jmethodID apiStatusSetCode;
jmethodID apiStatusSetMessage;
jmethodID apiStatusSetInstrumentData;


void
initMethodReferences (JNIEnv * env)
{
	integerCls = (*env)->FindClass (env, "java/lang/Integer");
	integerIntValue = (*env)->GetMethodID (env, integerCls,"intValue", "()I");
	integerValueOf = (*env)->GetStaticMethodID(env, integerCls,"valueOf","(I)Ljava/lang/Integer;");

	doubleCls = (*env)->FindClass (env, "java/lang/Double");
	doubleDoubleValue = (*env)->GetMethodID (env, doubleCls,"doubleValue", "()D");
	doubleValueOf = (*env)->GetStaticMethodID(env, doubleCls,"valueOf","(D)Ljava/lang/Double;");


	jniApiCls = (*env)->FindClass (env, "de/rolfjentsch/javaland/jni/JniApi");
	getStringLatin1 = (*env)->GetMethodID(env,jniApiCls,"getStringLatin1","([B)Ljava/lang/String;");
	getBytesLatin1 = (*env)->GetMethodID(env,jniApiCls,"getBytesLatin1","(Ljava/lang/String;)[B");

	apiInstrumentDataCls = (*env)->FindClass(env,"de/rolfjentsch/javaland/ApiInstrumentData");
	apiInstNew = (*env)->GetMethodID(env,apiInstrumentDataCls,"<init>","()V");
	apiInstGetInstId = (*env)->GetMethodID(env,apiInstrumentDataCls,"getInstrumentId","()Ljava/lang/String;");
	apiInstGetInstName = (*env)->GetMethodID(env,apiInstrumentDataCls,"getName","()Ljava/lang/String;");
	apiInstGetInstType = (*env)->GetMethodID(env,apiInstrumentDataCls,"getTyp","()Ljava/lang/Integer;");
	apiInstGetMaxVolume = (*env)->GetMethodID(env,apiInstrumentDataCls,"getMaxVolume","()Ljava/lang/Double;");
	apiInstSetInstId = (*env)->GetMethodID(env,apiInstrumentDataCls,"setInstrumentId","(Ljava/lang/String;)V");
	apiInstSetInstName = (*env)->GetMethodID(env,apiInstrumentDataCls,"setName","(Ljava/lang/String;)V");
	apiInstSetInstType = (*env)->GetMethodID(env,apiInstrumentDataCls,"setTyp","(Ljava/lang/Integer;)V");
	apiInstSetMaxVolume = (*env)->GetMethodID(env,apiInstrumentDataCls,"setMaxVolume","(Ljava/lang/Double;)V");

	apiStatusCls = (*env)->FindClass(env,"de/rolfjentsch/javaland/ApiStatus");
	apiStatusSetCode = (*env)->GetMethodID(env,apiStatusCls,"setCode","(I)V");
	apiStatusSetMessage = (*env)->GetMethodID(env,apiStatusCls,"setMessage","(Ljava/lang/String;)V");
	apiStatusSetInstrumentData = (*env)->GetMethodID(env,apiStatusCls,"setInstrumentData",
			"(Lde/rolfjentsch/javaland/ApiInstrumentData;)V");
}
