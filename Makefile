CC = clang++
LDFLAGS = -L../truco_domain_wrapper/target/release -ltruco_domain_wrapper
LDTESTFLAGS = -lcriterion
CFLAGS = -std=c++20
ifeq ($(DEBUG),1)
	CFLAGS += -g3
endif
ifeq ($(OPT),1)
	CFLAGS += -O3
endif
ifeq ($(LV),1)
	LDFLAGS += -v
endif

MAINS = $(shell find src/mains/ -maxdepth 1 -type f -name "*.cpp")
SRCS = $(shell find src/ -path "src/mains" -prune -o -path "src/tests" -prune -o -type f -name "*.cpp" | grep cpp)
MAINOBJS = $(patsubst src/mains/%.cpp, obj/mains/%.o, $(MAINS))
MAINOUTS = $(patsubst src/mains/%.cpp, %, $(MAINS))
TESTS = $(shell find src/tests/ -maxdepth 1 -type f -name "*.cpp")
TESTOBJS = $(patsubst src/tests/%.cpp, obj/tests/%.o, $(TESTS))
TESTOUTS = $(patsubst src/tests/%.cpp, %, $(TESTS))
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SRCS))

all: $(MAINOUTS)

$(MAINOUTS): $(OBJS) $(MAINOBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ obj/mains/$@.o $(OBJS) $(LDFLAGS)

$(TESTOUTS): $(OBJS) $(TESTOBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ obj/tests/$@.o $(OBJS) $(LDFLAGS) $(LDTESTFLAGS) -DRUNNING_TESTS

obj/%.o: src/%.cpp 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/mains/%.o: src/mains/%.cpp 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/tests/%.o: src/tests/%.cpp 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test clean

test: $(TESTOUTS)
	 for file in $^ ; do \
		 bin/$${file} ; \
	 done

clean:
	rm -rf bin/ 
	rm -rf obj/
