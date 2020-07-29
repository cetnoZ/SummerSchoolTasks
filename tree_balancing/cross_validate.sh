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

        note=$(${CHECKER} "${input_file}" "${output_file}" "${test_output_file}")
        result=$?

        case result in
            0)  printf "OK\n"
                continue;;
            1)  printf "WA %s\n" "${note}";;
            2)  printf "PE %s\n" "${note}";;
            3)  printf "FAIL %s\n" "${note}";;
        esac
    done
done
