PREFIX =   $(HOME)/.local/bin
LDLIBS =  `pkg-config --libs gtk+-3.0`
CFLAGS += `pkg-config --cflags gtk+-3.0`
CFLAGS += -Iinclude
SRC    = ./src/dragon.c
OBJ    = $(SRC:.c=.o)
NAME   = dragon

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) --std=c99 -Wall $(DEFINES) $^ -o $(NAME) $(CFLAGS) $(LDLIBS)

install: dragon
	mkdir -p $(PREFIX)
	cp dragon $(PREFIX)
