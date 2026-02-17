#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BIN="${BUILD_DIR}/01/replace/replace"
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
# trap cleanup EXIT

setup_tests() {
    mkdir -p "${TEST_DIR}/input" "${TEST_DIR}/expected" "${TEST_DIR}/output"

    # Тест 1: Замена "1231234" - "XYZ"
    printf "12312312345\n" > "${TEST_DIR}/input/test1.txt"
    printf "123XYZ5\n" > "${TEST_DIR}/expected/test1.txt"

    # Тест 2: Замена "ма" - "мама" без цикла
    printf "ма ма ма\n" > "${TEST_DIR}/input/test2.txt"
    printf "мама мама мама\n" > "${TEST_DIR}/expected/test2.txt"

    # Тест 3: Пустая строка поиска - текст не меняется
    printf "any text\n" > "${TEST_DIR}/input/test3.txt"
    printf "any text\n" > "${TEST_DIR}/expected/test3.txt"

    # Тест 4: Совпадение имён файлов
    touch "${TEST_DIR}/input/test4.txt"

    # Тест 6: Stdin режим
    cat > "${TEST_DIR}/input/test6.stdin" <<'EOF'
ma
mama
mama delala pelmeni
EOF
    printf "mamamama delala pelmeni\n" > "${TEST_DIR}/expected/test6.txt"

    # Тест 7: Ошибка в stdin режиме (только одна строка)
    printf "hello\n" > "${TEST_DIR}/input/test7.stdin"

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
    local is_interactive="$3"
    local expect_error="$4"
    local expected_file="$5"
    local stdin_file="$6"
    shift 6
    local cmd=("$@")

    local output_file=""
    local is_file_mode=0
    if [ ${#cmd[@]} -ge 3 ] && [ "${cmd[0]}" = "${BIN}" ] && [ "${cmd[1]}" != "-h" ]; then
        output_file="${cmd[2]}"
        is_file_mode=1
    fi

    local actual_output_file="${TEST_DIR}/output/${name}.txt"
    mkdir -p "$(dirname "${output_file}")"

    printf "Test '%s': %s... " "${name}" "${desc}"

    local actual_exit=0

    if [ ${is_interactive} -eq 1 ]; then
        "${cmd[@]}" < "${stdin_file}" > "${actual_output_file}" 2>&1
        actual_exit=$?
    else
        "${cmd[@]}" > "${actual_output_file}" 2>&1
        actual_exit=$?
    fi

    local expected_exit=0

    if [ ${expect_error} -eq 1 ] && [ $is_interactive -eq 0 ]; then
        expected_exit=1
    fi

    local test_failed=0

    if [ ${actual_exit} -ne ${expected_exit} ]; then
        echo -e "${RED}FAIL${NC}"
        echo -e "${YELLOW}  → Expected exit code: ${expected_exit}${NC}"
        echo -e "${YELLOW}  → Actual exit code:   ${actual_exit}${NC}"
        test_failed=1
    fi

    if [ ${test_failed} -eq 0 ] && [ ${expect_error} -ne 1 ]; then
        local compare_file=""
        if [ ${is_interactive} -eq 0 ] && [ -f "${output_file}" ]; then
            compare_file="${output_file}"
        elif [ -f "${actual_output_file}" ]; then
            compare_file="${actual_output_file}"
        fi

        if [ -z "${compare_file}" ]; then
            echo -e "${RED}FAIL${NC}"
            echo -e "${YELLOW}  → No output produced${NC}"
            test_failed=1
        elif [ ! -f "${expected_file}" ]; then
            echo -e "${RED}FAIL${NC}"
            echo -e "${YELLOW}  → Expected file not found: ${expected_file}${NC}"
            test_failed=1
        else
            if ! diff -u --strip-trailing-cr "${expected_file}" "${compare_file}" > /dev/null 2>&1; then
                echo -e "${RED}FAIL${NC}"
                echo -e "${YELLOW}  → Expected content:${NC}"
                sed 's/^/     /' "${expected_file}"
                echo -e "${YELLOW}  → Actual content:${NC}"
                sed 's/^/     /' "${compare_file}"
                test_failed=1
            fi
        fi
    fi

    if [ ${test_failed} -eq 0 ] && [ ${expect_error} == "true" ]; then
        local error_output=""
        if [ -f "${output_file}" ]; then
            error_output=$(cat "${output_file}" 2>/dev/null || true)
        fi
        if ! echo "${error_output}" | grep -q "^ERROR$"; then
            echo -e "${RED}FAIL${NC}"
            echo -e "${YELLOW}  → Expected output: ERROR${NC}"
            echo -e "${YELLOW}  → Actual output:   '${error_output}'${NC}"
            test_failed=1
        fi
    fi

    if [ ${test_failed} -eq 1 ]; then
        echo -e "${YELLOW}  → Command executed:${NC}"
        printf "     "
        printf '%q ' "${cmd[@]}"
        echo ""
        if [ -n "${stdin_file}" ]; then
            echo -e "${YELLOW}  → Stdin input:${NC}"
            sed 's/^/     /' "${stdin_file}"
        fi
        if [ -f "${output_file}" ] && [ -s "${output_file}" ]; then
            echo -e "${YELLOW}  → Program stdout:${NC}"
            sed 's/^/     /' "${output_file}"
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

    run_test "file_1231234" "1231234 → XYZ (critical case)" \
        0 0 "${TEST_DIR}/expected/test1.txt" "" \
        "${BIN}" "${TEST_DIR}/input/test1.txt" "${TEST_DIR}/output/file_1231234.txt" "1231234" "XYZ" || true

    run_test "file_ma_mama" "ма → мама (no infinite loop)" \
        0 0 "${TEST_DIR}/expected/test2.txt" "" \
        "${BIN}" "${TEST_DIR}/input/test2.txt" "${TEST_DIR}/output/file_ma_mama.txt" "ма" "мама" || true

    run_test "file_empty_search" "empty search string" \
        0 0 "${TEST_DIR}/expected/test3.txt" "" \
        "${BIN}" "${TEST_DIR}/input/test3.txt" "${TEST_DIR}/output/file_empty_search.txt" "" "replace" || true

    run_test "error_same_files" "same input/output filenames" \
        0 1 "" "" \
        "${BIN}" "${TEST_DIR}/input/test4.txt" "${TEST_DIR}/input/test4.txt" "a" "b" || true

    run_test "error_missing_input" "missing input file" \
        0 1 "" "" \
        "${BIN}" "${TEST_DIR}/input/missing.txt" "${TEST_DIR}/output/error_missing_input.txt" "a" "b" || true

    run_test "interactive" "interactive mode" \
        1 0 "${TEST_DIR}/expected/test6.txt" "${TEST_DIR}/input/test6.stdin" \
        "${BIN}" || true

    run_test "interactive_error" "interactive mode error" \
        1 1 "" "${TEST_DIR}/input/test7.stdin" \
        "${BIN}" || true

    run_test "invalid_arg" "invalid argument" \
        0 1 "" "" \
        "${BIN}" "--invalid" || true
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
