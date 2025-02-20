CC = gcc
CFLAGS = -Wall -Wextra -O2

OBJ_COMMON = main.o input.o rodcutting.o solution.o
OBJ_LRU = cache_lru.o
OBJ_MRU = cache_mru.o

all: rodcut_lru.exe rodcut_mru.exe

rodcut_lru.exe: $(OBJ_COMMON) $(OBJ_LRU)
	$(CC) $(CFLAGS) -o rodcut_lru.exe $(OBJ_COMMON) $(OBJ_LRU)

rodcut_mru.exe: $(OBJ_COMMON) $(OBJ_MRU)
	$(CC) $(CFLAGS) -o rodcut_mru.exe $(OBJ_COMMON) $(OBJ_MRU)

main.o: main.c pieces.h input.h rodcutting.h cache.h
	$(CC) $(CFLAGS) -c main.c

input.o: input.c pieces.h input.h
	$(CC) $(CFLAGS) -c input.c

rodcutting.o: rodcutting.c rodcutting.h pieces.h cache.h solution.h
	$(CC) $(CFLAGS) -c rodcutting.c

solution.o: solution.c solution.h pieces.h
	$(CC) $(CFLAGS) -c solution.c

cache_lru.o: cache_lru.c cache.h
	$(CC) $(CFLAGS) -c cache_lru.c

cache_mru.o: cache_mru.c cache.h
	$(CC) $(CFLAGS) -c cache_mru.c

clean:
	rm -f *.o rodcut_lru rodcut_mru
