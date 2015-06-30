    LIB = -lm -llikwid -L/home/soft/likwid/lib
    INC = -I/home/soft/likwid/include
    PROG = pdeSolver
    OBJS   = trab1.o
    CC     = gcc -O0 -g

%.o: %.c
	$(CC) -c $(INC) $< 

$(PROG):  $(OBJS)
	$(CC) -o $@ $^ $(LIB)

clean:
	@rm -f *~ *.bak *.o $(PROG) out.data solution.txt

all: $(PROG)
	$(CC) -o $@ $^ $(LIB)
