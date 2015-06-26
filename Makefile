    LIB = -lm
    PROG = pdeSolver
    OBJS   = trab1.o
    CC     = gcc -O3 -g

%.o: %.c
	$(CC) -c $< 

$(PROG):  $(OBJS)
	$(CC) -o $@ $^ $(LIB)

clean:
	@rm -f *~ *.bak *.o $(PROG) out.data solution.txt

all: $(PROG)
	$(CC) -o $@ $^ $(LIB)