CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0` -Wall -rdynamic
CLIBS=`pkg-config --libs gtk+-3.0`

fastfingers: fastfingers.o cards.o fflib.o home.o practice.o
	$(CC) $(CFLAGS) -o fastfingers fastfingers.o cards.o home.o practice.o fflib.o $(CLIBS)

fastfingers.o: fastfingers.c fflib.c practice.c cards.c home.c fflib.h cards.h home.h
	$(CC) -c fastfingers.c $(CFLAGS)

fflib.o: fflib.c fflib.h
	$(CC) -c fflib.c $(CFLAGS)

home.o: home.c home.h
	$(CC) -c home.c $(CFLAGS)

cards.o: cards.c cards.h
	$(CC) -c cards.c $(CFLAGS)

practice.o: practice.c practice.h
	$(CC) -c practice.c $(CFLAGS)

clean:
	rm fastfingers  *.o *~
