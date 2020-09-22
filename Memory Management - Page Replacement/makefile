CC = gcc
CCC = $(CC) -c
CFLAGS = -Wall
MFLAG = -lm
TARGET = mem_manager

all:	queue simulator page_replacement mapper manager
	$(CC) $(CFLAGS) queue.o proc_simulator.o page_replacement.o mem_mapper.o mem_manager.o -o $(TARGET) $(MFLAG)

clean:
	rm *.o *.dat $(TARGET)

queue:	queue.c
	$(CCC) queue.c

simulator:	proc_simulator.c
	$(CCC) proc_simulator.c

page_replacement:	page_replacement.c
	$(CCC) page_replacement.c

mapper:		mem_mapper.c
	$(CCC) mem_mapper.c

manager:	mem_manager.c
	$(CCC) mem_manager.c
