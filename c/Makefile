CFLAGS = -std=c++17 -g -gdwarf-4 -Ilib
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC_SOURCES = $(filter-out src/main.c, $(wildcard src/*.c))
SRC_OBJECTS = $(SRC_SOURCES:.c=.o)

MAIN_SOURCE = src/main.c
MAIN_OBJECT = src/main.o

LIB_SOURCES = $(wildcard lib/*.c)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

OBJECTS = $(MAIN_OBJECT) $(SRC_OBJECTS) $(LIB_OBJECTS)

VulkanTest: $(OBJECTS)
	set -xe
	g++ $(CFLAGS) -o VulkanTest $(OBJECTS) $(LDFLAGS)


$(MAIN_OBJECT) : $(MAIN_SOURCE)
	g++ $(CFLAGS) -c $(MAIN_SOURCE) -o $(MAIN_OBJECT)

src/%.o: src/%.c
	g++ $(CFLAGS) -c $< -o $@

lib/%.o: lib/%.c
	g++ $(CFLAGS) -c $< -o $@

Shaders:
	mkdir -p shaders
	glslc ./shaders/shader.vert -o ./shaders/out/vert.spv
	glslc ./shaders/shader.frag -o ./shaders/out/frag.spv

.PHONY: all test clean

all: VulkanTest Shaders
	./VulkanTest

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest $(OBJECTS)
	rm -r shaders/*

memory_layout: VulkanTest
	objdump -x VulkanTest

struct_layout: VulkanTest
	pahole VulkanTest

