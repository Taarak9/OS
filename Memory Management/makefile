CC = gcc
CCC = $(CC) -c
CFLAGS = -Wall
MFLAG = -lm
TARGET = mem_manager

all:	queue simulator mapper
	$(CC) $(CFLAGS) queue.o proc_simulator.o mem_mapper.o mem_manager.c -o $(TARGET) $(MFLAG)

clean:
	rm *.o *.dat $(TARGET)

queue:		 queue.c
	$(CCC) queue.c

simulator:	proc_simulator.c
	$(CCC) proc_simulator.c

mapper:		mem_mapper.c
	$(CCC) mem_mapper.c
