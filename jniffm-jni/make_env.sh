#!/bin/bash
JAVAC=`type -p javac`
if [ X"$JAVAC" = X ]
then
	echo javac nicht gefunden >&2
	exit 1
fi

JAVAC=`readlink -f $JAVAC`
JAVABIN=`dirname $JAVAC`
JAVA_HOME=`dirname $JAVABIN`
JAVA_INC=$JAVA_HOME/include
JAVA_INC_LINUX=$JAVA_INC/linux

echo JAVA_HOME = $JAVA_HOME > make_env
echo JAVA_INC = $JAVA_INC >> make_env
echo JAVA_INC_LINUX = $JAVA_INC_LINUX >> make_env
