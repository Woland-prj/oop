#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BIN="${BUILD_DIR}/01/invert/invert"
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

    # 1: Пример
    cat > "${TEST_DIR}/input/test1.txt" <<'EOF'
1	2	3
0	1	4
5	6	0
EOF
    cat > "${TEST_DIR}/expected/test1.txt" <<'EOF'
-24.000	18.000	5.000
20.000	-15.000	-4.000
-5.000	4.000	1.000
EOF

    # 2: Пример - дробные
    cat > "${TEST_DIR}/input/test2.txt" <<'EOF'
4	7	2.3
2	1	1
3	-2	-2.31
EOF
    cat > "${TEST_DIR}/expected/test2.txt" <<'EOF'
-0.009	0.321	0.131
0.212	-0.448	0.017
-0.194	0.806	-0.278
EOF

    # 3: Non-invertible
    cat > "${TEST_DIR}/input/test3.txt" <<'EOF'
1	2	3
2	4	6
1	2	3
EOF
    echo "Non-invertible" > "${TEST_DIR}/expected/test3.txt"

    # 4: не 3×3
    cat > "${TEST_DIR}/input/test4.txt" <<'EOF'
1	2
3	4
EOF
    echo "Invalid matrix format" > "${TEST_DIR}/expected/test4.txt"

    # 5: Не числа
    cat > "${TEST_DIR}/input/test5.txt" <<'EOF'
1	2	a
3	4	5
6	7	8
EOF
    echo "Invalid matrix" > "${TEST_DIR}/expected/test5.txt"

    # 6: Единичная матрица
    cat > "${TEST_DIR}/input/test6.txt" <<'EOF'
1	0	0
0	1	0
0	0	1
EOF
    cat > "${TEST_DIR}/expected/test6.txt" <<'EOF'
1.000	0.000	0.000
0.000	1.000	0.000
0.000	0.000	1.000
EOF

    # 7: Симметричная матрица
    cat > "${TEST_DIR}/input/test7.txt" <<'EOF'
2	1	1
1	2	1
1	1	2
EOF
    cat > "${TEST_DIR}/expected/test7.txt" <<'EOF'
0.750	-0.250	-0.250
-0.250	0.750	-0.250
-0.250	-0.250	0.750
EOF

    # 8: Просто матрица
    cat > "${TEST_DIR}/input/test8.stdin" <<'EOF'
1	2	3
0	1	4
5	6	0
EOF

    # 9: 4x4
    cat > "${TEST_DIR}/input/test9.stdin" <<'EOF'
1	2	3 7
0	1	4 9
5	6	0 12
3 8 19 11
EOF

    # 9: 4x4
    cat > "${TEST_DIR}/input/test9.stdin" <<'EOF'
1	2	3 7
0	1	4 9
5	6	0 12
3 8 19 11
EOF

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
    [ -f "${BIN}" ] || {
        echo -e "${RED}Binary not found${NC}"
        exit 1
    }
    echo -e "${GREEN}Build successful${NC}"
}

run_test() {
    local name="$1"
    local desc="$2"
    local mode="$3"  # file | stdin
    local input="$4"
    local expected="$5"

    local output="${TEST_DIR}/output/${name}.txt"
    mkdir -p "$(dirname "${output}")"

    printf "Test '%s': %s... " "${name}" "${desc}"

    if [ "${mode}" = "file" ]; then
        "${BIN}" "${input}" > "${output}" 2>&1
    else
        "${BIN}" < "${input}" > "${output}" 2>&1
    fi

    local exit_code=$?

    # Проверка содержимого
    if diff -w "${expected}" "${output}" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC}"
        ((PASS++))
        return 0
    else
        echo -e "${RED}FAIL${NC}"
        echo -e "${YELLOW}Expected:${NC}"
        cat "${expected}" | sed 's/^/  /'
        echo -e "${YELLOW}Got:${NC}"
        cat "${output}" | sed 's/^/  /'
        ((FAIL++))
        return 1
    fi
}

run_tests() {
    echo -e "\n${GREEN}Running tests${NC}"

    run_test "example1" "Example 1" "file" \
        "${TEST_DIR}/input/test1.txt" "${TEST_DIR}/expected/test1.txt"

    run_test "fractions" "Matrix with fractions" "file" \
        "${TEST_DIR}/input/test2.txt" "${TEST_DIR}/expected/test2.txt"

    run_test "singular" "Singular matrix (Non-invertible)" "file" \
        "${TEST_DIR}/input/test3.txt" "${TEST_DIR}/expected/test3.txt"

    run_test "invalid_format" "Invalid matrix format" "file" \
        "${TEST_DIR}/input/test4.txt" "${TEST_DIR}/expected/test4.txt"

    run_test "invalid_values" "Invalid matrix values" "file" \
        "${TEST_DIR}/input/test5.txt" "${TEST_DIR}/expected/test5.txt"

    run_test "identity" "Identity matrix" "file" \
        "${TEST_DIR}/input/test6.txt" "${TEST_DIR}/expected/test6.txt"

    run_test "symmetric" "Symmetric matrix" "file" \
        "${TEST_DIR}/input/test7.txt" "${TEST_DIR}/expected/test7.txt"

    run_test "stdin" "Read from stdin" "stdin" \
        "${TEST_DIR}/input/test8.stdin" "${TEST_DIR}/expected/test1.txt"

    run_test "stdin" "Read from stdin" "stdin" \
        "${TEST_DIR}/input/test9.stdin" "${TEST_DIR}/expected/test9.txt"
}

print_summary() {
    echo -e "\n${GREEN}Summary:${NC} ${PASS} passed, ${RED}${FAIL} failed${NC}"
    [ ${FAIL} -eq 0 ] && {
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    } || {
        echo -e "${RED}Some tests failed${NC}"
        exit 1
    }
}

setup_tests
build_project
run_tests
print_summary
