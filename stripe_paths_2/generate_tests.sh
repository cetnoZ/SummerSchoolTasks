#!/usr/bin/env bash

trap "exit" SIGINT


SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=10       QUERIES_COUNT=10      TREE_TYPE=random    QUERY_TYPE=random       TEST_START_INDEX=0    TEST_COUNT=5  QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=10       QUERIES_COUNT=10      TREE_TYPE=depth     QUERY_TYPE=random       TEST_START_INDEX=5    TEST_COUNT=5  QUERY_PERCENT='0.5'  DEPTH_COEFF=1   python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=10       QUERIES_COUNT=10      TREE_TYPE=line      QUERY_TYPE=random       TEST_START_INDEX=10   TEST_COUNT=5  QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"

SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=100       QUERIES_COUNT=100      TREE_TYPE=random    QUERY_TYPE=random     TEST_START_INDEX=15   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=100       QUERIES_COUNT=100      TREE_TYPE=depth     QUERY_TYPE=random     TEST_START_INDEX=20   TEST_COUNT=5    QUERY_PERCENT='0.5'  DEPTH_COEFF=1   python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=100       QUERIES_COUNT=100      TREE_TYPE=line      QUERY_TYPE=random     TEST_START_INDEX=25   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"


SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=1000      QUERIES_COUNT=1000     TREE_TYPE=random    QUERY_TYPE=random     TEST_START_INDEX=30   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=1000      QUERIES_COUNT=1000     TREE_TYPE=depth     QUERY_TYPE=random     TEST_START_INDEX=35   TEST_COUNT=5    QUERY_PERCENT='0.5'  DEPTH_COEFF=1   python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=1000      QUERIES_COUNT=1000     TREE_TYPE=line      QUERY_TYPE=random     TEST_START_INDEX=40   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"


SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=10000     QUERIES_COUNT=10000    TREE_TYPE=random    QUERY_TYPE=random     TEST_START_INDEX=45   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=10000     QUERIES_COUNT=10000    TREE_TYPE=depth     QUERY_TYPE=random     TEST_START_INDEX=50   TEST_COUNT=5    QUERY_PERCENT='0.5'  DEPTH_COEFF=1   python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=10000     QUERIES_COUNT=10000    TREE_TYPE=line      QUERY_TYPE=random     TEST_START_INDEX=55   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"


SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=30000     QUERIES_COUNT=30000    TREE_TYPE=random    QUERY_TYPE=random     TEST_START_INDEX=60   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=30000     QUERIES_COUNT=30000    TREE_TYPE=depth     QUERY_TYPE=random     TEST_START_INDEX=65   TEST_COUNT=5    QUERY_PERCENT='0.5'  DEPTH_COEFF=1   python3 "${TEST_GENERATOR}"
SEED=${SEED} TEST_DIR="${TESTS_DIR}" VERTEX_COUNT=30000     QUERIES_COUNT=30000    TREE_TYPE=line      QUERY_TYPE=random     TEST_START_INDEX=70   TEST_COUNT=5    QUERY_PERCENT='0.5'                  python3 "${TEST_GENERATOR}"

for input_file in tests/input*.txt; do
    output_file=${input_file/input/output}
	"./${SOLUTION}" <"${input_file}" >"${output_file}"
    test_index=$([[ "${input_file}" =~ ([0-9]+) ]] && echo "${BASH_REMATCH[1]}")
    printf "[Test #%3s] generated output\n" "${test_index}"
done