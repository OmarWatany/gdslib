SRC_D  := ./src
OBJ_D  := ./objects
EXAMPLE_D := ./example_files
INC    := ./include
# RAY_LIB := -L./lib -lgdslib
RAY_LIB := -L./lib -lgdslib -Wl,-rpath=./lib 

LIB := $(RAY_LIB) -lm 

CFLAGS=-Wall -Wextra -g -I$(INC) -fPIC -Wno-unused-function 
# CFLAGS= -g -I$(INC) -fPIC 

GQUEUE_HFILES := $(wildcard $(INC)/*queue*.h)
GQUEUE_CFILES := $(wildcard $(SRC_D)/*queue*.c)

LLIST_HFILES  := $(wildcard $(INC)/*list*.h) 
LLIST_CFILES  := $(wildcard $(SRC_D)/*list*.c)

GSTACK_HFILES := $(wildcard $(INC)/*stack*.h)
GSTACK_CFILES := $(wildcard $(SRC_D)/*stack*.c)

RING_HFILES   := $(wildcard $(INC)/*ring*.h)
RING_CFILES   := $(wildcard $(SRC_D)/*ring*.c)

GTREE_HFILES := $(wildcard $(INC)/*gtree*.h)
GTREE_CFILES := $(wildcard $(SRC_D)/*gtree*.c)

NODE_HFILES   := $(wildcard $(INC)/*node*.h)
NODE_CFILES   := $(wildcard $(SRC_D)/*node*.c)

ALLOC_HFILES   := $(wildcard $(INC)/*allocator*.h)
ALLOC_CFILES   := $(wildcard $(SRC_D)/*allocator*.c)


C_FILES := $(GQUEUE_CFILES) $(GSTACK_CFILES) $(LLIST_CFILES) $(GTREE_CFILES) $(NODE_CFILES) $(ALLOC_CFILES) $(RING_CFILES)
H_FILES := $(GQUEUE_HFILES) $(GSTACK_HFILES) $(LLIST_HFILES) $(GTREE_HFILES) $(NODE_HFILES) $(ALLOC_HFILES) $(RING_HFILES)

EXAMPLE_FILES := $(wildcard $(EXAMPLE_D)/*.c)
ALL := $(H_FILES) $(C_FILES) $(EXAMPLE_FILES)

CC := gcc

all:  stack ring queue linkedlist alist astack gtree pqueue $(EXAMPLE_D)

stack: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gstack.c $(LIB) -o ./bin/stack 

ring : lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gringbuffer.c $(LIB) -o ./bin/ring 

queue: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gqueue.c $(LIB) -o ./bin/queue

pqueue: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gpqueue.c $(LIB) -o ./bin/pqueue

linkedlist: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_glinkedlist.c $(LIB) -o ./bin/list

alist: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_garraylist.c $(LIB) -o ./bin/alist

astack: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_garraystack.c $(LIB) -o ./bin/astack

gnode: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gnode.c $(LIB) -o ./bin/node

gtree: lib
	$(CC) $(CFLAGS) $(EXAMPLE_D)/example_gtree.c $(LIB) -o ./bin/tree

lib: obj 
	ar rcs ./lib/libgdslib.a $(wildcard ./objects/*.o)
	$(CC) -shared $(OBJ_D)/*.o -o ./lib/libgdslib.so

obj: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -c $(C_FILES) 
	mv *.o ./objects/

run : all
	@valgrind ./bin/ring

clean :
	rm ./objects/*
	rm ./lib/*
	rm ./bin/*

