#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BIN="${BUILD_DIR}/01/crypt/crypt"
TEST_DIR="${SCRIPT_DIR}/tests"

PASS=0
FAIL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

cleanup() {
    rm -rf "${TEST_DIR}"
}
trap cleanup EXIT


setup_tests() {
    mkdir -p "${TEST_DIR}/input" "${TEST_DIR}/output"

    printf "Hello world\n" > "${TEST_DIR}/input/text.txt"

    printf "" > "${TEST_DIR}/input/empty.txt"

    head -c 100000 /dev/urandom > "${TEST_DIR}/input/random.bin"
}


build_project() {
    echo "Building project..."

    if [ ! -f "${BIN}" ]; then
        cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -G Ninja > /dev/null 2>&1 || {
            echo -e "${RED}CMake failed${NC}"
            exit 1
        }

        cmake --build "${BUILD_DIR}" > /dev/null 2>&1 || {
            echo -e "${RED}Build failed${NC}"
            exit 1
        }
    fi

    if [ ! -f "${BIN}" ]; then
        echo -e "${RED}Binary not found: ${BIN}${NC}"
        exit 1
    fi

    echo -e "${GREEN}Build successful${NC}"
}


run_test() {
    local name="$1"
    local desc="$2"
    local expect_error="$3"
    shift 3
    local cmd=("$@")

    printf "Test '%s': %s... " "${name}" "${desc}"

    "${cmd[@]}" >"${TEST_DIR}/output/${name}.log" 2>&1
    local exit_code=$?

    if [ "$expect_error" -eq 1 ]; then
        if [ "$exit_code" -eq 0 ]; then
            echo -e "${RED}FAIL${NC}"
            ((FAIL++))
            return
        fi
    else
        if [ "$exit_code" -ne 0 ]; then
            echo -e "${RED}FAIL${NC}"
            cat "${TEST_DIR}/output/${name}.log"
            ((FAIL++))
            return
        fi
    fi

    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
}


run_file_compare() {
    local name="$1"
    local desc="$2"
    local file1="$3"
    local file2="$4"

    printf "Test '%s': %s... " "${name}" "${desc}"

    if diff -q "$file1" "$file2" > /dev/null; then
        echo -e "${GREEN}PASS${NC}"
        ((PASS++))
    else
        echo -e "${RED}FAIL${NC}"
        ((FAIL++))
    fi
}


run_tests() {

    echo -e "\n${GREEN}Running tests${NC}"

    run_test "help" "help flag" \
        0 \
        "${BIN}" --help

    run_test "invalid_cmd" "unknown command" \
        1 \
        "${BIN}" wrong a b c

    run_test "missing_args" "not enough arguments" \
        1 \
        "${BIN}" crypt

    run_test "invalid_key_string" "key is not number" \
        1 \
        "${BIN}" crypt \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/out.txt" \
        abc

    run_test "invalid_key_range" "key out of range" \
        1 \
        "${BIN}" crypt \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/out.txt" \
        999

    run_test "missing_input" "missing input file" \
        1 \
        "${BIN}" crypt \
        "${TEST_DIR}/input/missing.txt" \
        "${TEST_DIR}/output/out.txt" \
        10


    "${BIN}" crypt \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/encrypted.txt" \
        42

    "${BIN}" decrypt \
        "${TEST_DIR}/output/encrypted.txt" \
        "${TEST_DIR}/output/decrypted.txt" \
        42

    run_file_compare \
        "encrypt_decrypt" \
        "encrypt then decrypt returns original" \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/decrypted.txt"


    "${BIN}" crypt \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/key0.bin" \
        0

    "${BIN}" decrypt \
        "${TEST_DIR}/output/key0.bin" \
        "${TEST_DIR}/output/key0_dec.txt" \
        0

    run_file_compare \
        "key0" \
        "boundary key = 0" \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/key0_dec.txt"


    "${BIN}" crypt \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/key255.bin" \
        255

    "${BIN}" decrypt \
        "${TEST_DIR}/output/key255.bin" \
        "${TEST_DIR}/output/key255_dec.txt" \
        255

    run_file_compare \
        "key255" \
        "boundary key = 255" \
        "${TEST_DIR}/input/text.txt" \
        "${TEST_DIR}/output/key255_dec.txt"


    "${BIN}" crypt \
        "${TEST_DIR}/input/random.bin" \
        "${TEST_DIR}/output/random_enc.bin" \
        123

    "${BIN}" decrypt \
        "${TEST_DIR}/output/random_enc.bin" \
        "${TEST_DIR}/output/random_dec.bin" \
        123

    run_file_compare \
        "large_file" \
        "large random file encryption/decryption" \
        "${TEST_DIR}/input/random.bin" \
        "${TEST_DIR}/output/random_dec.bin"
}


print_summary() {

    echo -e "\n${GREEN}Summary:${NC} ${PASS} passed, ${RED}${FAIL} failed${NC}"

    if [ "${FAIL}" -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed${NC}"
        exit 1
    fi
}


setup_tests
build_project
run_tests
print_summary
