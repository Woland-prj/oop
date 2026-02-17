#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BIN="${BUILD_DIR}/01/flipbyte/flipbyte"
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
    mkdir -p "${TEST_DIR}/input" "${TEST_DIR}/expected" "${TEST_DIR}/output"

    # 6 -> 96 (00000110 -> 01100000)
    echo "96" > "${TEST_DIR}/expected/test_valid_6.txt"

    # 0 -> 0 (00000000 -> 00000000)
    echo "0" > "${TEST_DIR}/expected/test_valid_0.txt"

    # 255 -> 255 (11111111 -> 11111111)
    echo "255" > "${TEST_DIR}/expected/test_valid_255.txt"

    # 128 -> 1 (10000000 -> 00000001)
    echo "1" > "${TEST_DIR}/expected/test_valid_128.txt"

    # 1 -> 128 (00000001 -> 10000000)
    echo "128" > "${TEST_DIR}/expected/test_valid_1.txt"

    # 170 -> 85 (10101010 -> 01010101)
    echo "85" > "${TEST_DIR}/expected/test_valid_170.txt"

    # 85 -> 170 (01010101 -> 10101010)
    echo "170" > "${TEST_DIR}/expected/test_valid_85.txt"

    # 123 -> 222 (01111011 -> 11011110)
    echo "222" > "${TEST_DIR}/expected/test_valid_123.txt"

    # 200 -> 19 (11001000 -> 00010011)
    echo "19" > "${TEST_DIR}/expected/test_valid_200.txt"

    # 254 -> 127 (11111110 -> 01111111)
    echo "127" > "${TEST_DIR}/expected/test_valid_254.txt"

    # 127 -> 254 (01111111 -> 11111110)
    echo "254" > "${TEST_DIR}/expected/test_valid_127.txt"

    echo "ERROR: Argument is larger then byte See help: flipbyte -h" > "${TEST_DIR}/expected/test_invalid_256.txt"
    echo "ERROR: Argument is larger then byte See help: flipbyte -h" > "${TEST_DIR}/expected/test_invalid_neg1.txt"
    echo "ERROR: Argument is not number See help: flipbyte -h" > "${TEST_DIR}/expected/test_invalid_abc.txt"
    echo "ERROR: Argument is not number See help: flipbyte -h" > "${TEST_DIR}/expected/test_invalid_12.5.txt"
    echo "ERROR: Argument is empty See help: flipbyte -h" > "${TEST_DIR}/expected/test_invalid_empty.txt"
}

build_project() {
    echo "Building project..."
    if [ ! -f "${BIN}" ]; then
        if ! cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -G Ninja > /dev/null 2>&1; then
            echo -e "${RED}CMake configuration failed${NC}"
            exit 1
        fi
        if ! cmake --build "${BUILD_DIR}" > /dev/null 2>&1; then
            echo -e "${RED}Build failed${NC}"
            exit 1
        fi
    fi
    if [ ! -f "${BIN}" ]; then
        echo -e "${RED}Binary not found at ${BIN}${NC}"
        exit 1
    fi
    echo -e "${GREEN}Build successful${NC}"
}

run_test() {
    local name="$1"
    local desc="$2"
    local input="$3"
    local expect_error="$4"
    local expected_file="$5"

    local actual_output_file="${TEST_DIR}/output/${name}.txt"
    mkdir -p "$(dirname "${actual_output_file}")"

    printf "Test '%s': %s... " "${name}" "${desc}"

    local actual_exit=0
    local cmd=("${BIN}" "${input}")

    if [ "${input}" = "-h" ]; then
        cmd=("${BIN}" "-h")
    fi

    "${cmd[@]}" > "${actual_output_file}" 2>&1
    actual_exit=$?

    local expected_exit=0
    if [ "${expect_error}" = "1" ]; then
        expected_exit=1
    fi

    local test_failed=0

    # Проверка кода возврата
    if [ ${actual_exit} -ne ${expected_exit} ]; then
        echo -e "${RED}FAIL${NC}"
        echo -e "${YELLOW}  → Expected exit code: ${expected_exit}${NC}"
        echo -e "${YELLOW}  → Actual exit code:   ${actual_exit}${NC}"
        test_failed=1
    fi

    if [ ${test_failed} -eq 0 ] && [ "${expect_error}" != "1" ]; then
        if [ ! -f "${expected_file}" ]; then
            echo -e "${RED}FAIL${NC}"
            echo -e "${YELLOW}  → Expected file not found: ${expected_file}${NC}"
            test_failed=1
        else
            local expected_content=$(cat "${expected_file}" | tr -d '\r\n' | tr -d ' ')
            local actual_content=$(cat "${actual_output_file}" | tr -d '\r\n' | tr -d ' ')

            if [ "${expected_content}" != "${actual_content}" ]; then
                echo -e "${RED}FAIL${NC}"
                echo -e "${YELLOW}  → Expected content: '${expected_content}'${NC}"
                echo -e "${YELLOW}  → Actual content:   '${actual_content}'${NC}"
                test_failed=1
            fi
        fi
    fi

    # Ошибки
    if [ ${test_failed} -eq 0 ] && [ "${expect_error}" = "1" ]; then
        if [ ! -f "${expected_file}" ]; then
            echo -e "${RED}FAIL${NC}"
            echo -e "${YELLOW}  → Expected file not found: ${expected_file}${NC}"
            test_failed=1
        else
            local expected_error=$(cat "${expected_file}")
            local actual_error=$(cat "${actual_output_file}" 2>/dev/null || true)

            if ! echo "${actual_error}" | grep -q "ERROR"; then
                echo -e "${RED}FAIL${NC}"
                echo -e "${YELLOW}  → Expected error message with 'ERROR'${NC}"
                echo -e "${YELLOW}  → Actual output: '${actual_error}'${NC}"
                test_failed=1
            fi
        fi
    fi

    if [ ${test_failed} -eq 1 ]; then
        echo -e "${YELLOW}  → Command executed:${NC}"
        printf "     "
        printf '%q ' "${cmd[@]}"
        echo ""
        if [ -f "${actual_output_file}" ]; then
            echo -e "${YELLOW}  → Program output:${NC}"
            sed 's/^/     /' "${actual_output_file}"
        fi
        ((FAIL++))
        return 1
    fi

    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
    return 0
}

run_tests() {
    echo -e "\n${GREEN}Running tests${NC}"

    # Корректные
    run_test "valid_6" "Valid input: 6 -> 96" "6" "0" "${TEST_DIR}/expected/test_valid_6.txt" || true
    run_test "valid_0" "Valid input: 0 -> 0" "0" "0" "${TEST_DIR}/expected/test_valid_0.txt" || true
    run_test "valid_255" "Valid input: 255 -> 255" "255" "0" "${TEST_DIR}/expected/test_valid_255.txt" || true
    run_test "valid_128" "Valid input: 128 -> 1" "128" "0" "${TEST_DIR}/expected/test_valid_128.txt" || true
    run_test "valid_1" "Valid input: 1 -> 128" "1" "0" "${TEST_DIR}/expected/test_valid_1.txt" || true
    run_test "valid_170" "Valid input: 170 -> 85" "170" "0" "${TEST_DIR}/expected/test_valid_170.txt" || true
    run_test "valid_85" "Valid input: 85 -> 170" "85" "0" "${TEST_DIR}/expected/test_valid_85.txt" || true
    run_test "valid_123" "Valid input: 123 -> 222" "123" "0" "${TEST_DIR}/expected/test_valid_123.txt" || true
    run_test "valid_200" "Valid input: 200 -> 19" "200" "0" "${TEST_DIR}/expected/test_valid_200.txt" || true
    run_test "valid_254" "Valid input: 254 -> 127" "254" "0" "${TEST_DIR}/expected/test_valid_254.txt" || true
    run_test "valid_127" "Valid input: 127 -> 254" "127" "0" "${TEST_DIR}/expected/test_valid_127.txt" || true

    # Некорректные
    run_test "invalid_256" "Invalid input: 256 (out of range)" "256" "1" "${TEST_DIR}/expected/test_invalid_256.txt" || true
    run_test "invalid_neg1" "Invalid input: -1 (negative)" "-1" "1" "${TEST_DIR}/expected/test_invalid_neg1.txt" || true
    run_test "invalid_abc" "Invalid input: 'abc' (not a number)" "abc" "1" "${TEST_DIR}/expected/test_invalid_abc.txt" || true
    run_test "invalid_12.5" "Invalid input: '12.5' (not integer)" "12.5" "1" "${TEST_DIR}/expected/test_invalid_12.5.txt" || true
    run_test "invalid_empty" "Invalid input: '' (empty)" "" "1" "${TEST_DIR}/expected/test_invalid_empty.txt" || true

    # Нет аргументов
    run_test "no_args" "No arguments" "" "1" "${TEST_DIR}/expected/test_invalid_empty.txt" || true
}

print_summary() {
    echo -e "\n${GREEN}Summary:${NC} ${PASS} passed, ${RED}${FAIL} failed${NC}"
    if [ ${FAIL} -eq 0 ]; then
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
