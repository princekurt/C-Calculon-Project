OBJS = calculon.o value.o node.o queue.o stack.o scanner.o convert.o bst.o
OPTS = gcc -Wall -g -std=c99
calculon: $(OBJS)
	$(OPTS) -o calculon $(OBJS) -lm

value.o: value.c value.h
	$(OPTS) -c value.c

node.o: node.c node.h value.h
	$(OPTS) -c node.c
	
calculon.o: calculon.c value.h node.h scanner.h queue.h stack.h
	$(OPTS) -c calculon.c 
	
queue.o: queue.c queue.h value.c node.c node.h value.h
	$(OPTS) -g -c queue.c
	
stack.o: stack.c stack.h value.c node.c node.h value.h 
	$(OPTS) -c stack.c

scanner.o: scanner.c
	$(OPTS) -c scanner.c

convert.o: convert.c convert.h stack.c stack.h queue.c queue.h node.c node.h
	$(OPTS) -c convert.c
	
bst.o: bst.c bst.h node.c value.c
	$(OPTS) -c bst.c
	
clean:
	rm -f $(OBJS)

test:
	./calculon testfile.txt
	./calculon -d testfile.txt
	./calculon testfile2.txt
	./calculon -d testfile2.txt
	./calculon -v
	