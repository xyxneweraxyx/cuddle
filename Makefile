# Compilation
TARGET := libcuddle.a
CC := ar rc
FLAGS := -Wall -Wextra -g3 -O2 -std=c99

# Sources
SRC := $(shell find . -name '*.c')
OBJ := $(SRC:.c=.o)
HDR := $(shell find . -name '*.h')

all: $(TARGET) clean

$(TARGET): $(SRC) 
	clang -c $(SRC)
	ar rc $(TARGET) *.o

clean:
	rm -f $(shell find . -name '*.o')
	rm -f $(shell find . -name '*.c~')
	rm -f $(shell find . -name '*.h~')
	rm -f $(shell find . -name '*#*')
	rm -f $(shell find . -name '*.pch')
	rm -f $(shell find . -name '*.out')
	rm -f $(shell find . -name '*~')

fclean: clean
	rm -f $(shell find . -name '$(TARGET)')
	rm -f $(shell find . -name 'cuddle')

re: fclean $(TARGET)

print_src:
	$(info $(shell find . -name '*.c'))

epiclang_all:
	epiclang $(SRC) $(HDR)

main: clean
	clang $(SRC) $(FLAGS) -o cuddle