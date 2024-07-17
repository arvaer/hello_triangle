CFLAGS =  -g -gdwarf-4 -Ilib
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC_SOURCES = $(filter-out src/main.c, $(wildcard src/*.c))
SRC_OBJECTS = $(SRC_SOURCES:.c=.o)

MAIN_SOURCE = src/main.c
MAIN_OBJECT = src/main.o

LIB_SOURCES = $(wildcard lib/*.c)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

TEST_SOURCES = $(wildcard test/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

OBJECTS = $(MAIN_OBJECT) $(SRC_OBJECTS) $(LIB_OBJECTS)
LIB_TEST_OBJECTS = $(TEST_OBJECTS) $(LIB_OBJECTS)

Vulkan: $(OBJECTS)
	set -xe
	g++ $(CFLAGS) -o Vulkan $(OBJECTS) $(LDFLAGS)

TestLib: $(LIB_TEST_OBJECTS)
	g++ $(CFLAGS) -o Test $(LIB_TEST_OBJECTS) $(LDFLAGS)

$(MAIN_OBJECT) : $(MAIN_SOURCE)
	g++ $(CFLAGS) -c $(MAIN_SOURCE) -o $(MAIN_OBJECT)

src/%.o: src/%.c
	g++ $(CFLAGS) -c $< -o $@

lib/%.o: lib/%.c
	g++ $(CFLAGS) -c $< -o $@

test/%.o: test/%.c
	g++ $(CFLAGS) -c $< -o $@

Shaders:
	mkdir -p shaders
	glslc shaders/shader.vert -o shaders/out/vert.spv
	glslc shaders/shader.frag -o shaders/out/frag.spv

.PHONY: all run clean clean_test clean_all memory_layout struct_layout test

all: Vulkan Shaders
	./Vulkan

run: Vulkan
	./Vulkan

test: TestLib
	./Test

clean:
	rm -f Vulkan $(OBJECTS)

clean_test:
	rm -f ./Test $(LIB_TEST_OBJECTS)

clean_all: clean clean_test
	rm -f shaders/out

memory_layout: Vulkan
	objdump -x Vulkan

struct_layout: Vulkan
	pahole Vulkan



