CC      = gcc
CFLAGS  = -Wall -Wextra -O2
TARGET  = unmacro

SRCS = commands.c debmsg.c disassemble.c displacements.c getdata.c header.c instructions.c i_type.c masks.c operands.c posneg.c symtab.c undata.c unmacro.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
