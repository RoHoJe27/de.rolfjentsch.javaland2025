#!/bin/bash

cd `dirname $0`

/usr/lib64/jvm/jdk-22/bin/javac -h ../jniffm-jni/include \
	-d unused \
	-implicit:none \
	./src/main/java/de/rolfjentsch/javaland/Api.java \
	./src/main/java/de/rolfjentsch/javaland/ApiStatus.java \
	./src/main/java/de/rolfjentsch/javaland/ApiInstrumentData.java \
	./src/main/java/de/rolfjentsch/javaland/jni/JniApi.java

rm -r unused
