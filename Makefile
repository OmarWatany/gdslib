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

C_FILES := $(GQUEUE_CFILES) $(GSTACK_CFILES) $(LLIST_CFILES)
H_FILES := $(GQUEUE_HFILES) $(GSTACK_HFILES) $(LLIST_HFILES)

TEST_FILES := $(wildcard $(TEST_D)/*.c)
OBJ_FILES := $(wildcard $(OBJ_D)/*.o)
ALL := $(H_FILES) $(C_FILES) $(TEST_FILES)

CC := gcc

all: queue stack alist astack linkedlist lib

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

obj: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -c -fPIC  $(C_FILES) 
	mv *.o ./objects/

lib: obj 
	ar rcs ./lib/libgdslib.a $(OBJ_FILES)
	# gcc -shared $(OBJ_D)/*.o -o ./lib/libgdslib.so
	# cp ./lib/libgdslib.so ./bin/

clean :
	rm ./objects/*
	rm ./lib/*
	rm ./bin/*

