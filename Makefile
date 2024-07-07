CFLAGS = -std=c++17 -g -gdwarf-4
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest

memory_layout: VulkanTest
	objdump -x VulkanTest

struct_layout: VulkanTest
	pahole VulkanTest
