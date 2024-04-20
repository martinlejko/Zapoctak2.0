CC = clang
CFLAGS = -std=c++20 -Wall

SRCS = main.cpp \
       tgaimage.cpp \
       parser.cpp \
       datatypes.cpp \
       model.cpp \
       render.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = simple-software-renderer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

