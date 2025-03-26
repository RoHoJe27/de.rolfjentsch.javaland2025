CFLAGS=-I ../include -I ../../include -I ${JAVA_INC} -I ${JAVA_INC_LINUX} -Werror -Wmissing-prototypes -Wstrict-prototypes -fPIC -ggdb
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
DEPDIR := .d

JAVADIR=../../jniffm-java

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

SHLIB=../lib/libjni.so
SRSC=jniapi.c references.c util.c
OBJECTS=${SRSC:.c=.o}
DEPENDS=${patsubst %.c,.d/%.Td,${SRSC}}

all:${SHLIB}

install:all
	mkdir -p ${JAVADIR}/shlib
	cp ${SHLIB} ${JAVADIR}/shlib

${SHLIB}:${OBJECTS}
	gcc -fPIC -shared -ggdb -o ${SHLIB} -L../shlib ${OBJECTS} -lapi -lpthread

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

-include ${DEPENDS} make_env


