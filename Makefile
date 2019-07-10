##
## Makefile
## File description:
## dragon Makefile 
##

PREFIX =   $(HOME)/.local/bin
LDLIBS =  `pkg-config --libs gtk+-3.0`
CFLAGS += `pkg-config --cflags gtk+-3.0`
CFLAGS += -Iinclude
SRC    = ./src/dragon.c		\
	 ./src/drag.c		\
	 ./src/uri.c		\
	 ./src/button.c
OBJ    = $(SRC:.c=.o)
NAME   = dragon

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) --std=c99 -Wall $(DEFINES) $^ -o $(NAME) $(CFLAGS) $(LDLIBS)

install: $(NAME)
	mkdir -p $(PREFIX)
	cp $(NAME) $(PREFIX)

uninstall:
	rm $(PREFIX)/$(NAME)

clean:
	rm $(NAME)

dist-clean: clean
	rm src/*.o

.PHONY: all $(NAME)
