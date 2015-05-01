    PROG = pdeSolver
    OBJS   = trab1.o
    CC     = gcc -g

%.o: %.c
	$(CC) -c $<

$(PROG):  $(OBJS)
	$(CC) -o $@ $^

clean:
	@rm -f *~ *.bak *.o $(PROG)
