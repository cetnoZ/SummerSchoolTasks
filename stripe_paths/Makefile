BIN_DIR := $(abspath bin)
SOLUTIONS_DIR := $(abspath solutions)
TESTS_DIR := $(abspath tests)
SOLUTIONS_SOURCE := $(abspath $(wildcard ${SOLUTIONS_DIR}/*.cpp))
SOLUTIONS := $(patsubst %.cpp, %, $(notdir $(basename ${SOLUTIONS_SOURCE})))
SOLUTION := bin/big_small_nsqrtn_1
SOLUTIONS_RUNNABLE := $(addprefix ${BIN_DIR}/, ${SOLUTIONS})
SEED := 'custom seed'
TEST_GENERATOR := $(abspath ./generators/generator.py)

CC := g++
CFLAGS := ${CFLAGS} -O2 -Wall -Wpedantic -std=c++11

.PHONY: all
all: solutions generate_tests

${BIN_DIR} ${TESTS_DIR}:
	mkdir -p $@

${BIN_DIR}/%: ${SOLUTIONS_DIR}/%.cpp
	${CC} ${CFLAGS} -o $@ $<

.PHONY: solutions
solutions: ${BIN_DIR} ${SOLUTIONS_RUNNABLE}


.PHONY: clean
clean:
	rm -r ${BIN_DIR} ${TESTS_DIR}

.PHONY: generate_tests
generate_tests: ${SOLUTION} ${TESTS_DIR}
	SEED=${SEED} TESTS_DIR=${TESTS_DIR} TEST_GENERATOR=${TEST_GENERATOR} SOLUTION=${SOLUTION} ./generate_tests.sh

.PHONY: cross_validate
cross_validate: generate_tests solutions
	BIN_DIR="${BIN_DIR}" ./cross_validate.sh
