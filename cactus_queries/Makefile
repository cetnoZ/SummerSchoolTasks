SEED := 'custom seed'
BIN_DIR := $(abspath bin)
SOLUTIONS_DIR := $(abspath solutions)
TESTS_DIR := $(abspath tests)
SOLUTIONS_SOURCE := $(abspath $(wildcard ${SOLUTIONS_DIR}/*.cpp))
SOLUTIONS := $(patsubst %.cpp, %, $(notdir $(basename ${SOLUTIONS_SOURCE})))
SOLUTION := bin/condensention+lca_n+q_1
SOLUTIONS_RUNNABLE := $(addprefix ${BIN_DIR}/, ${SOLUTIONS})
TEST_GENERATOR := $(abspath ./generators/generator.py)

CC := g++
CFLAGS := ${CFLAGS} -Wall -Wpedantic -std=c++11 -x c++


.PHONY: all
all: solutions generate_tests

${BIN_DIR} ${TESTS_DIR}:
	mkdir -p ${BIN_DIR} ${TESTS_DIR}

${BIN_DIR}/%: ${SOLUTIONS_DIR}/%.cpp
	${CC} ${CFLAGS} -o $@ $<

.PHONY: solutions 
solutions: ${BIN_DIR} ${SOLUTIONS_RUNNABLE}

.PHONY: cross_validate
cross_validate: solutions
	TMP_DIR=${TMP_DIR} BIN_DIR=${BIN_DIR} ./cross_validate.sh 

.PHONY: generate_tests
generate_tests: ${SOLUTION} ${TESTS_DIR}
	SOLUTION=${SOLUTION} TEST_GENERATOR=${TEST_GENERATOR} TESTS_DIR=${TESTS_DIR} SEED=${SEED} ./generate_tests.sh

.PHONY: clean
clean:
	rm -rf ${TESTS_DIR} ${BIN_DIR}
