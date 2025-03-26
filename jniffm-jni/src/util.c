/*
 * util.c
 *
 * Diverse Hilfsfunktionen.
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */
#include <jni.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "references.h"

/*
 * Baut aus einem Java-String einen C-String.
 * Die Java-Seite sorgt dafür, dass die Bytes im richtigen Zeichensatz kodiert sind.
 *
 * Der Pointer muss per free() freigegeben werden.
 */
signed char * toCString (JNIEnv * env, jobject jniApi,  jstring string)
{
	if (NULL == string)
	{
		char *leer = malloc(1);
		*leer = '\0';
		return leer;
	}
	jbyteArray bArray = (*env)->CallObjectMethod(env,jniApi,getBytesLatin1,string);
	jsize length = (*env)->GetArrayLength (env, bArray);
	signed char *cString = malloc (length + 1);
	if (cString == NULL)
	{
		fprintf(stderr,"toCString malloc -> NULL\n");
		return NULL;
	}
	(*env)->GetByteArrayRegion (env, bArray, 0, length, cString);
	cString[length] = '\0';
	return cString;
}

/*
 * Baut aus einem C-String einen JavaString.
 */
jstring toJavaString (JNIEnv * env, jobject jniApi, const char *string)
{
	if (string == NULL)
	{
		return NULL;
	}
	int len = strlen (string);
	jbyteArray arr = (*env)->NewByteArray(env, len);
	(*env)->SetByteArrayRegion (env, arr, 0, len, string);
	return (*env)->CallObjectMethod(env,jniApi,getStringLatin1,arr);
}

/*
 * Holt aus einem Java IntegerObject den intValue.
 * Liefert 0 wenn das Object null ist
 */
int toInteger (JNIEnv * env, jobject integer)
{
	if (NULL == integer)
		return 0;
	return (*env)->CallIntMethod(env,integer,integerIntValue);
}

/*
 * Holt aus einem Java DoubleObject den doubleValue.
 * Liefert 0 wenn das Object null ist
 */
double toDouble (JNIEnv * env, jobject doubleObj)
{
	if (NULL == doubleObj)
		return 0.0;
	return (*env)->CallDoubleMethod(env,doubleObj,doubleDoubleValue);
}

/*
 * Erzeugt aus einem jint ein Integer-Object.
 */
jobject toIntegerObject(JNIEnv *env,jint i)
{
	return (*env)->CallStaticObjectMethod(env,integerCls,integerValueOf,i);
}

/*
 * Erzeugt aus einem jdouble ein Double-Object.
 */
jobject toDoubleObject(JNIEnv *env,jdouble d)
{
	return (*env)->CallStaticObjectMethod(env,doubleCls,doubleValueOf,d);
}
