SRC_D  := ./src
OBJ_D  := ./objects
EXAMPLE_D := ./example_files
INC    := ./include
# LIB := -L./lib -l:libgdslib.a
LIB := -L./lib -lgdslib -Wl,-rpath=./lib

CFLAGS=-Wall -Wextra -g -I$(INC)

GQUEUE_HFILES := $(wildcard $(INC)/*queue*.h)
GQUEUE_CFILES := $(wildcard $(SRC_D)/*queue*.c)

LLIST_HFILES  := $(wildcard $(INC)/*list*.h) 
LLIST_CFILES  := $(wildcard $(SRC_D)/*list*.c)

GSTACK_HFILES := $(wildcard $(INC)/*stack*.h)
GSTACK_CFILES := $(wildcard $(SRC_D)/*stack*.c)

NODE_HFILES   := $(wildcard $(INC)/*node*.h)
NODE_CFILES   := $(wildcard $(SRC_D)/*node*.c)

ALLOC_HFILES   := $(wildcard $(INC)/*allocator*.h)
ALLOC_CFILES   := $(wildcard $(SRC_D)/*allocator*.c)


C_FILES := $(GQUEUE_CFILES) $(GSTACK_CFILES) $(LLIST_CFILES) $(NODE_CFILES) $(ALLOC_CFILES)
H_FILES := $(GQUEUE_HFILES) $(GSTACK_HFILES) $(LLIST_HFILES) $(NODE_HFILES) $(ALLOC_HFILES)

EXAMPLE_FILES := $(wildcard $(EXAMPLE_D)/*.c)
ALL := $(H_FILES) $(C_FILES) $(EXAMPLE_FILES)

CC := gcc

all:  stack queue linkedlist alist astack $(EXAMPLE_D)

stack: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gstack.c $(LIB) -o ./bin/stack 

queue: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gqueue.c $(LIB) -o ./bin/queue

linkedlist: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_glinkedlist.c $(LIB) -o ./bin/list

alist: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_garraylist.c $(LIB) -o ./bin/alist

astack: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_garraystack.c $(LIB) -o ./bin/astack

gnode: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gnode.c $(LIB) -o ./bin/node

lib: obj 
	ar rcs ./lib/libgdslib.a $(wildcard ./objects/*.o)
	gcc -shared $(OBJ_D)/*.o -o ./lib/libgdslib.so

obj: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -c $(C_FILES) 
	mv *.o ./objects/

clean :
	rm ./objects/*
	rm ./lib/*
	rm ./bin/*

