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
all: ${BIN_DIR} ${SOLUTIONS_RUNNABLE}

${BIN_DIR}:
	mkdir -p ${BIN_DIR}

${TESTS_DIR}:
	mkdir -p ${TESTS_DIR}

${BIN_DIR}/%: ${SOLUTIONS_DIR}/%.cpp
	${CC} ${CFLAGS} -o $@ $<

.PHONY: cross_validate
cross_validate: ${SOLUTIONS_RUNNABLE}
	TMP_DIR=${TMP_DIR} BIN_DIR=${BIN_DIR} ./cross_validate.sh 

.PHONY: generate_tests
generate_tests: ${SOLUTION} ${TESTS_DIR}
	SOLUTION=${SOLUTION} TEST_GENERATOR=${TEST_GENERATOR} TESTS_DIR=${TESTS_DIR} SEED=${SEED} ./generate_tests.sh

.PHONY: clean
clean:
	rm -r ${TESTS_DIR}
	rm -r ${BIN_DIR}
