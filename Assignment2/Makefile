LDLIBS = -lopengl32 -lglew32 -lfreeglut

.PHONY: clean cleanall

all: simple

simple: $(INIT_SHADER_OBJ)
	g++ -Wall -g -O2 InitShader.cpp main.cpp $(LDLIBS) -o simple -std=c++11

clean:
	rm simple
