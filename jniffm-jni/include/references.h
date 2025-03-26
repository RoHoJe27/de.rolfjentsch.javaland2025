/*
 * references.h
 *
 * extern Deklarationen für alle auf der JNI-Seite verwendeten Klassen und deren Methoden.
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */

#ifndef REFERENCES_H_
#define REFERENCES_H_
#include <jni.h>

/* Class Integer */
extern jclass integerCls;
extern jmethodID integerIntValue;
extern jmethodID integerValueOf;

/* Class Double */
extern jclass doubleCls;
extern jmethodID doubleDoubleValue;
extern jmethodID doubleValueOf;


/* Class JniApi */
extern jclass jniApiCls;
extern jmethodID getStringLatin1;
extern jmethodID getBytesLatin1;

/* class ApiInsturmentData */
extern jclass apiInstrumentDataCls;
extern jmethodID apiInstNew;
extern jmethodID apiInstGetInstId;
extern jmethodID apiInstSetInstId;
extern jmethodID apiInstGetInstName;
extern jmethodID apiInstSetInstName;
extern jmethodID apiInstGetInstType;
extern jmethodID apiInstSetInstType;
extern jmethodID apiInstGetMaxVolume;
extern jmethodID apiInstSetMaxVolume;

/* class ApiStatus */
extern jclass apiStatusCls;
extern jmethodID apiStatusSetCode;
extern jmethodID apiStatusSetMessage;
extern jmethodID apiStatusSetInstrumentData;


extern void initMethodReferences (JNIEnv * env);


#endif /* REFERENCES_H_ */
