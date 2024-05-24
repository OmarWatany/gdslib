SRC_D  := ./src
OBJ_D  := ./objects
TEST_D := ./test_files
INC    := ./include
LIB := -L./lib -lgdslib

CFLAGS=-Wall -Wextra -g -I$(INC)

GQUEUE_CFILES := $(wildcard $(SRC_D)/*queue*.c)
GSTACK_CFILES := $(wildcard $(SRC_D)/*stack*.c)
LLIST_CFILES  := $(wildcard $(SRC_D)/*list*.c)
NODE_CFILES   := $(wildcard $(SRC_D)/*node*.c)

GQUEUE_HFILES := $(wildcard $(INC)/*queue*.h)
GSTACK_HFILES := $(wildcard $(INC)/*stack*.h)
LLIST_HFILES  := $(wildcard $(INC)/*list*.h) 
NODE_HFILES   := $(wildcard $(INC)/*node*.h)

C_FILES := $(GQUEUE_CFILES) $(GSTACK_CFILES) $(LLIST_CFILES) $(NODE_CFILES)
H_FILES := $(GQUEUE_HFILES) $(GSTACK_HFILES) $(LLIST_HFILES) $(NODE_HFILES)

TEST_FILES := $(wildcard $(TEST_D)/*.c)
ALL := $(H_FILES) $(C_FILES) $(TEST_FILES)

CC := gcc

all:  stack queue linkedlist alist astack 

stack: lib
	$(CC) $(CFLAGS) $(TEST_D)/test_gstack.c $(LIB) -o ./bin/stack 

queue: lib
	$(CC) $(CFLAGS) $(TEST_D)/test_gqueue.c $(LIB) -o ./bin/queue

linkedlist: lib
	$(CC) $(CFLAGS) $(TEST_D)/test_glinkedlist.c $(LIB) -o ./bin/list

alist: lib
	$(CC) $(CFLAGS) $(TEST_D)/test_garraylist.c $(LIB) -o ./bin/alist

astack: lib
	$(CC) $(CFLAGS) $(TEST_D)/test_garraystack.c $(LIB) -o ./bin/astack

gnode: lib
	$(CC) $(CFLAGS) $(TEST_D)/test_gnode.c $(LIB) -o ./bin/node

lib: obj 
	ar rcs ./lib/libgdslib.a $(wildcard ./objects/*.o)
	# gcc -shared $(OBJ_D)/*.o -o ./lib/libgdslib.so

obj: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -c $(C_FILES) 
	mv *.o ./objects/

clean :
	rm ./objects/*
	rm ./lib/*
	rm ./bin/*

