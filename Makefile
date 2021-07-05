MGINC = deps/mongoose
MGSRC = ${MGINC}/mongoose.c
MGOBJ = mongoose.o
CFLAGS = -I${MGINC}

SRC = orkus.c
OBJ = ${SRC:.c=.o} ${MGOBJ}

all: clean orkus

orkus: ${OBJ}
	${CC} ${LDFLAGS} -o orkus ${OBJ}

${OBJ}: ${SRC} ${MGSRC}
	${CC} ${CFLAGS} -c ${SRC} ${MGSRC}

deps:
	rm -rf deps
	mkdir deps &2>/dev/null
	git clone https://github.com/cesanta/mongoose deps/mongoose

clean:
	rm -f ${OBJ} orkus

.PHONY: deps
