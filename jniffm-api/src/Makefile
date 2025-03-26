CFLAGS=-I ../include -Werror -Wmissing-prototypes -Wstrict-prototypes -fPIC -ggdb
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
DEPDIR := .d

JNIDIR=../../jniffm-jni
JAVADIR=../../jniffm-java

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

SHLIB=../lib/libapi.so
SRSC=api.c
HEADER=../include/api.h
OBJECTS=${SRSC:.c=.o}
DEPENDS=${patsubst %.c,.d/%.Td,${SRSC}}

all:${SHLIB}

install:all
	mkdir -p ${JNIDIR}/shlib ${JNIDIR}/include ${JAVADIR}/shlib ${JAVADIR}/include
	cp ${SHLIB} ${JNIDIR}/shlib
	cp ${SHLIB} ${JAVADIR}/shlib
	cp ${HEADER} ${JNIDIR}/include
	cp ${HEADER} ${JAVADIR}/include

${SHLIB}:${OBJECTS}
	gcc -fPIC -shared -ggdb -o ${SHLIB} ${OBJECTS} -lpthread

clean:
	rm ${SHLIB} ${OBJECTS}

$(shell mkdir -p $(DEPDIR) >/dev/null)


%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.i : %.c
	$(CC) -E -P $(CFLAGS) $<

$(DEPDIR)/%.Td: ;
.PRECIOUS: $(DEPDIR)/%.Td

-include ${DEPENDS}


