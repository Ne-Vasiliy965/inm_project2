EXE=test.exe
SRC=test.cpp linear_system_solver.cpp
OBJ=$(SRC:.cpp=.o)
COMPILER=g++

all: $(EXE)
	@echo "test program builded"

clean:
	rm $(OBJ) $(EXE) liblinear_solver.a

lib: linear_system_solver.o
	ar rcs liblinear_solver.a linear_system_solver.o

$(EXE): $(OBJ)
	$(COMPILER) -o $(EXE) $(OBJ)

linear_system_solver.o: linear_system_solver.cpp
	$(COMPILER) -c $< -o $@

test.o: test.cpp
	$(COMPILER) -c $< -o $@

test.o: linear_system_solver.o