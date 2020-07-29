#!/usr/bin/env bash

trap "exit" SIGINT

TMP_DIR=./tmp
mkdir -p "${TMP_DIR}"

for input_file in tests/input*.txt; do
    output_file=${input_file/input/output}
    test_index=$([[ "${input_file}" =~ ([0-9]+) ]] && echo "${BASH_REMATCH[1]}")
    output=`cat "${output_file}"`
    output=${output//$'\n'/}
    printf "[TEST #%3s] cross validation:\n" "${test_index}"
    
    for solution_bin in ${BIN_DIR}/*; do
        solution=`basename ${solution_bin}`
        test_output_file=${TMP_DIR}/output_${solution}

        printf "\t[SOLUTION %20s] " "${solution}"

        timeout 2 "${solution_bin}" <"${input_file}" >"${test_output_file}"
        if [[ $? -ne 0 ]]; then
            printf "timeout\n"
            continue
        fi

        test_output=`cat "${test_output_file}"` 
        test_output=${test_output//$'\n'/}

        diff -q -a "${test_output_file}" "${output_file}" &> /dev/null
        if [[ $? -eq 0 ]]; then
            printf "OK\n"
            rm "${test_output_file}"
        else
            printf "FAIL\n"
            exit -1
        fi
    done
done
