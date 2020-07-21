BIN_DIR := $(abspath bin)
SOLUTIONS_DIR := $(abspath solutions)
SOLUTIONS_SOURCE := $(abspath $(wildcard ${SOLUTIONS_DIR}/*.cpp))
SOLUTIONS := $(patsubst %.cpp, %, $(notdir $(basename ${SOLUTIONS_SOURCE})))
SOLUTIONS_RUNNABLE := $(addprefix ${BIN_DIR}/, ${SOLUTIONS})

TEST_TARGETS := $(addprefix test_, ${SOLUTIONS})

CC := g++
CFLAGS := ${CFLAGS} -O2 -Wall -Wpedantic -std=c++11 -fno-stack-limit

.PHONY: all
all: ${BIN_DIR} ${SOLUTIONS_RUNNABLE}

${BIN_DIR}:
	mkdir -p ${BIN_DIR}

${BIN_DIR}/%: ${SOLUTIONS_DIR}/%.cpp
	${CC} ${CFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -r ${BIN_DIR}
