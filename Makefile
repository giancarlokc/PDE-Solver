    PROG = trab1
    OBJS   = $(PROG).o
    CC     = gcc -g

%.o: %.c
	$(CC) -c $<

$(PROG):  $(OBJS)
	$(CC) -o $@ $^

clean:
	@rm -f *~ *.bak
