#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BIN="${BUILD_DIR}/01/fill/fill"
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
    mkdir -p "${TEST_DIR}/input" "${TEST_DIR}/output" "${TEST_DIR}/expected"

    cat > "${TEST_DIR}/input/simple.txt" <<EOF
#####
# O #
#####
EOF

    cat > "${TEST_DIR}/expected/simple.txt" <<EOF
#####
#.O.#
#####
EOF


    cat > "${TEST_DIR}/input/multi_o.txt" <<EOF
#####
# O #
#   #
# O #
#####
EOF

    cat > "${TEST_DIR}/expected/multi_o.txt" <<EOF
#####
#.O.#
#...#
#.O.#
#####
EOF


    cat > "${TEST_DIR}/input/leak.txt" <<EOF
#####
# O #
#   #
#
EOF

    cat > "${TEST_DIR}/expected/leak.txt" <<EOF
#####
#.O.#
#...#
#....
EOF


    printf "" > "${TEST_DIR}/input/empty.txt"


    for i in $(seq 1 150); do
        echo "O" >> "${TEST_DIR}/input/too_many_rows.txt"
    done


    head -c 150 < /dev/zero | tr '\0' ' ' > "${TEST_DIR}/input/too_many_cols.txt"
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

    printf "Test '%s': %s... " "${name}" "${desc}"

    "$@" > "${TEST_DIR}/output/${name}.log" 2>&1
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
        echo "Expected:"
        cat "$file2"
        echo "Got:"
        cat "$file1"
        ((FAIL++))
    fi
}


run_tests() {

    echo -e "\n${GREEN}Running tests${NC}"


    run_test "help" "help flag" \
        0 \
        "${BIN}" -h


    run_test "invalid_args" "invalid argument count" \
        1 \
        "${BIN}" a b c


    run_test "missing_input" "missing input file" \
        1 \
        "${BIN}" missing.txt out.txt


    "${BIN}" \
        "${TEST_DIR}/input/simple.txt" \
        "${TEST_DIR}/output/simple.txt"

    run_file_compare \
        "simple_fill" \
        "simple fill test" \
        "${TEST_DIR}/output/simple.txt" \
        "${TEST_DIR}/expected/simple.txt"


    "${BIN}" \
        "${TEST_DIR}/input/multi_o.txt" \
        "${TEST_DIR}/output/multi_o.txt"

    run_file_compare \
        "multi_o" \
        "multiple start points" \
        "${TEST_DIR}/output/multi_o.txt" \
        "${TEST_DIR}/expected/multi_o.txt"


    "${BIN}" \
        "${TEST_DIR}/input/leak.txt" \
        "${TEST_DIR}/output/leak.txt"

    run_file_compare \
        "leak" \
        "flood outside contour" \
        "${TEST_DIR}/output/leak.txt" \
        "${TEST_DIR}/expected/leak.txt"


    "${BIN}" \
        "${TEST_DIR}/input/empty.txt" \
        "${TEST_DIR}/output/empty.txt"

    run_file_compare \
        "empty" \
        "empty file" \
        "${TEST_DIR}/output/empty.txt" \
        "${TEST_DIR}/input/empty.txt"


    "${BIN}" \
        "${TEST_DIR}/input/too_many_rows.txt" \
        "${TEST_DIR}/output/rows.txt"

    rows=$(wc -l < "${TEST_DIR}/output/rows.txt")

    if [ "$rows" -le 100 ]; then
        echo -e "Test 'max_rows': ${GREEN}PASS${NC}"
        ((PASS++))
    else
        echo -e "Test 'max_rows': ${RED}FAIL${NC}"
        ((FAIL++))
    fi


    "${BIN}" \
        "${TEST_DIR}/input/too_many_cols.txt" \
        "${TEST_DIR}/output/cols.txt"

    cols=$(head -n1 "${TEST_DIR}/output/cols.txt" | wc -c)

    if [ "$cols" -le 101 ]; then
        echo -e "Test 'max_cols': ${GREEN}PASS${NC}"
        ((PASS++))
    else
        echo -e "Test 'max_cols': ${RED}FAIL${NC}"
        ((FAIL++))
    fi


    printf "#####\n# O #\n#####\n" | "${BIN}" > "${TEST_DIR}/output/stdin.txt"

    run_file_compare \
        "stdin_mode" \
        "stdin mode works" \
        "${TEST_DIR}/output/stdin.txt" \
        "${TEST_DIR}/expected/simple.txt"
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
