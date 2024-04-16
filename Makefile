CC = gcc
CFLAGS = -Wall -g
OBJ = sha512_masked.o bool_arith.o

# Path to the include files
INCLUDES = -I.

# Define the executable file 
MAIN = sha512_masked

.PHONY: depend clean

all:    $(MAIN)
	@echo  Program compiled

$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJ)

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) $(INCLUDES) -MM $^ >  ./.depend;

include .depend
 
