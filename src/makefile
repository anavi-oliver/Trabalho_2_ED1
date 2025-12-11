
PROJ_NAME = ted
CC = gcc

CFLAGS = -g -Wall -Wextra -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration

LDFLAGS = -lm

SRC_DIRS := $(shell find . -type d)
SOURCES := $(shell find . -name '*.c')
OBJECTS := $(SOURCES:.c=.o)

INCLUDES := $(patsubst %,-I%,$(SRC_DIRS))

all: ted

ted: $(OBJECTS)
	$(CC) -o $(PROJ_NAME) $(OBJECTS) $(LDFLAGS)
	@echo "Executável '$(PROJ_NAME)' criado com sucesso!"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	find . -name '*.o' -delete
	rm -f $(PROJ_NAME)
	@echo "Limpeza concluida."