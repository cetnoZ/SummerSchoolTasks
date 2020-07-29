SEED := 'custom seed'
BIN_DIR := $(abspath bin)
SOLUTIONS_DIR := $(abspath solutions)
TESTS_DIR := $(abspath tests)
SOLUTIONS_SOURCE := $(abspath $(wildcard ${SOLUTIONS_DIR}/*.cpp))
SOLUTIONS := $(patsubst %.cpp, %, $(notdir $(basename ${SOLUTIONS_SOURCE})))
SOLUTION := bin/dp_n^3_1
SOLUTIONS_RUNNABLE := $(addprefix ${BIN_DIR}/, ${SOLUTIONS})
TEST_GENERATOR := $(abspath ./generators/generator.py)
CHECKER := $(abspath ./checker/checker)

CC := g++
CFLAGS := ${CFLAGS} -O2 -Wall -Wpedantic -std=c++11 -x c++

.PHONY: all
all: solutions checker generate_tests

${BIN_DIR} ${TESTS_DIR} ${TMP_DIR}:
	mkdir -p $@

${BIN_DIR}/%: ${SOLUTIONS_DIR}/%.cpp
	${CC} ${CFLAGS} -o $@ $<

${CHECKER}: ${CHECKER}.cpp
	${CC} ${CFLAGS} -o $@ $<

.PHONY: solutions
solutions: ${BIN_DIR} ${SOLUTIONS_RUNNABLE}

.PHONY: checker
checker: ${CHECKER}

.PHONY: cross_validate
cross_validate: checker generate_tests ${SOLUTIONS_RUNNABLE}
	CHECKER=${CHECKER} TMP_DIR=${TMP_DIR} BIN_DIR=${BIN_DIR} ./cross_validate.sh 

.PHONY: generate_tests
generate_tests: ${TESTS_DIR} ${SOLUTION}
	SOLUTION=${SOLUTION} TEST_GENERATOR=${TEST_GENERATOR} TESTS_DIR=${TESTS_DIR} SEED=${SEED} ./generate_tests.sh

.PHONY: clean
clean:
	rm -r ${TESTS_DIR} ${BIN_DIR} ${CHECKER}

