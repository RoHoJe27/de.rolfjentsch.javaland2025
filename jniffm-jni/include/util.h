/*
 * util.h
 *
 * Extern Decklarationen für diverse Hilfsfunktionen.
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <jni.h>

/*
 * Baut aus einem Java-String einen C-String.
 * Die Java-Seite sorgt dafür, dass die Bytes im richtigen Zeichensatz kodiert sind.
 *
 * Der Pointer muss per free() freigegeben werden.
 */
extern signed char *toCString (JNIEnv * env, jobject jniApi, jstring javaString);

/*
 * Baut aus einem C-String einen JavaString.
 */
extern jstring toJavaString (JNIEnv * env, jobject jniApi, const char *string);

/*
 * Holt aus einem Java IntegerObject den intValue.
 * Liefert 0 wenn das Object null ist
 */
extern int toInteger (JNIEnv * env, jobject integer);

/*
 * Holt aus einem Java DoubleObject den doubleValue.
 * Liefert 0 wenn das Object null ist
 */
extern double toDouble (JNIEnv * env, jobject integer);

/*
 * Erzeugt aus einem jint ein Integer-Object.
 */
extern jobject toIntegerObject(JNIEnv *env,jint i);
/*
 * Erzeugt aus einem jdouble ein Double-Object.
 */
extern jobject toDoubleObject(JNIEnv *env,jdouble d);



#endif /* UTIL_H_ */
