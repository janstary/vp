BINS	= vp
MANS	= vp.1
OBJS	= vowel.o group.o vplot.o color.o svg.o ps.o
TEST	= male-back.txt female-front.txt diphthongs.txt

PREFIX	= ${HOME}
BINDIR	= ${PREFIX}/bin/
MANDIR	= ${PREFIX}/man/man1/
CFLAGS	= -Wall -pedantic

all: ${BINS}

vp: ${OBJS} vp.c
	${CC} ${CFLAGS} -o vp ${OBJS} vp.c

vowel.o: vowel.c vowel.h
group.o: group.c group.h
vplot.o: vplot.c vplot.h
color.o: color.c color.h
svg.o: svg.c svg.h
ps.o: ps.c ps.h

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

test: ${BINS} ${TEST}
	for i in ${TEST} ; do ./vp $$i ; done
	for i in ${TEST} ; do ./vp $$i $${i%.txt}.svg; done

lint: ${MANS}
	mandoc -Tlint -Wstyle ${MANS}

install: ${BINS} ${MANS}
	install -d -m 755 ${BINDIR} && install -m 755 ${BINS} ${BINDIR}
	install -d -m 755 ${MANDIR} && install -m 644 ${MANS} ${MANDIR}

uninstall:
	( cd ${BINDIR} && rm -f ${BINS} )
	( cd ${MANDIR} && rm -f ${MANS} )

clean:
	rm -f ${BINS} ${OBJS} *.{ps,pdf,svg,png} *~

