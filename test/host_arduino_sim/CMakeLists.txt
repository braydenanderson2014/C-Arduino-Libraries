cmake_minimum_required(VERSION 3.20)

project(host_arduino_sim_tests LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HOST_SIM_ENABLE_STRESS_TESTS "Register stress tests in CTest" ON)
set(HOST_SIM_STRESS_MEM_LIMIT_BYTES "2097152" CACHE STRING "Memory limit for stress tests")
set(HOST_SIM_STRESS_MAX_INSTANCES "20000" CACHE STRING "Max structure instances for stress tests")
set(HOST_SIM_STRESS_MAX_ELEMENTS "5000000" CACHE STRING "Max element operations for stress tests")
set(HOST_SIM_STRESS_PROGRESS_INTERVAL_MS "60000" CACHE STRING "Progress heartbeat interval for stress tests (milliseconds)")
set(HOST_SIM_STRESS_BACKENDS "none;sd" CACHE STRING "Semicolon-separated stress backend modes")
set(HOST_SIM_STRESS_OPTIONAL_MODES "off;on" CACHE STRING "Semicolon-separated stress optional modes")
set(HOST_SIM_OUT_DIR "${CMAKE_CURRENT_LIST_DIR}/out" CACHE PATH "Output directory for host simulation artifacts")

file(MAKE_DIRECTORY "${HOST_SIM_OUT_DIR}/bin")

set(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")

set(HOST_SIM_COMMON_INCLUDE_DIRS
    "${REPO_ROOT}/test/host_arduino_sim/include"
    "${REPO_ROOT}/lib/ArrayList/src"
    "${REPO_ROOT}/lib/AVLTree/src"
    "${REPO_ROOT}/lib/DynamicStorageLibrary/src"
    "${REPO_ROOT}/lib/Hashtable/src"
    "${REPO_ROOT}/lib/Json/src"
    "${REPO_ROOT}/lib/MathLib/src"
    "${REPO_ROOT}/lib/Operators/src"
    "${REPO_ROOT}/lib/Predicates/src"
    "${REPO_ROOT}/lib/SimpleCoreRuntime/src"
    "${REPO_ROOT}/lib/SimpleMutex"
    "${REPO_ROOT}/lib/SimpleRWLock/src"
    "${REPO_ROOT}/lib/SimpleSemaphore/src"
    "${REPO_ROOT}/lib/SimpleThreadManager"
    "${REPO_ROOT}/lib/SimpleVector/src"
    "${REPO_ROOT}/lib/SDList/src"
    "${REPO_ROOT}/lib/String/src"
    "${REPO_ROOT}/lib/TypeTraits/src"
)

set(HOST_SIM_STRESS_INCLUDE_DIRS
    "${REPO_ROOT}/test/host_arduino_sim/include"
    "${REPO_ROOT}/lib/ArrayList/src"
    "${REPO_ROOT}/lib/AVLTree/src"
    "${REPO_ROOT}/lib/DynamicStorageLibrary/src"
    "${REPO_ROOT}/lib/Hashtable/src"
    "${REPO_ROOT}/lib/Json/src"
    "${REPO_ROOT}/lib/LinkedList/src"
    "${REPO_ROOT}/lib/MathLib/src"
    "${REPO_ROOT}/lib/Optional/src"
    "${REPO_ROOT}/lib/Operators/src"
    "${REPO_ROOT}/lib/OrderedMap/src"
    "${REPO_ROOT}/lib/Predicates/src"
    "${REPO_ROOT}/lib/Queue/src"
    "${REPO_ROOT}/lib/SimpleVector/src"
    "${REPO_ROOT}/lib/SDList/src"
    "${REPO_ROOT}/lib/Stack/src"
    "${REPO_ROOT}/lib/String/src"
    "${REPO_ROOT}/lib/TypeTraits/src"
)

set(HOST_SIM_JSON_INCLUDE_DIRS
    "${REPO_ROOT}/test/host_arduino_sim/include"
    "${REPO_ROOT}/lib/Json/src"
    "${REPO_ROOT}/lib/SimpleVector/src"
    "${REPO_ROOT}/lib/MathLib/src"
    "${REPO_ROOT}/lib/String/src"
    "${REPO_ROOT}/lib/TypeTraits/src"
)

add_executable(host_experimental_unoqbridge
    "${REPO_ROOT}/test/host_arduino_sim/host_experimental_unoqbridge_tests.cpp"
)
target_include_directories(host_experimental_unoqbridge PRIVATE
    "${REPO_ROOT}/test/host_arduino_sim/include"
    "${REPO_ROOT}/lib/__ArduinoUnoQLibraryDevelopment__Experimental/UnoQBridge/src"
)
target_compile_definitions(host_experimental_unoqbridge PRIVATE
    AL_NO_SERIAL
    _AVR_ATmega2560_
)
set_target_properties(host_experimental_unoqbridge PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${HOST_SIM_OUT_DIR}/bin"
)

function(add_host_integration_target target_name backend optional_mode)
    add_executable(${target_name}
        "${REPO_ROOT}/lib/Json/src/JSON.cpp"
        "${REPO_ROOT}/test/host_arduino_sim/host_arduino_sim_tests.cpp"
    )
    target_include_directories(${target_name} PRIVATE ${HOST_SIM_COMMON_INCLUDE_DIRS})
    target_compile_definitions(${target_name} PRIVATE
        AL_NO_SERIAL
        _AVR_ATmega2560_
    )

    if(backend STREQUAL "littlefs")
        target_compile_definitions(${target_name} PRIVATE USE_LITTLEFS)
    endif()

    if(optional_mode STREQUAL "on")
        target_compile_definitions(${target_name} PRIVATE
            JSON_ENABLE_OPTIONAL_RETURNS=1
            AVL_TREE_ENABLE_ERROR_CODES=1
        )
        target_include_directories(${target_name} PRIVATE "${REPO_ROOT}/lib/Optional/src")
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${HOST_SIM_OUT_DIR}/bin"
    )

    add_test(NAME ${target_name}_run
        COMMAND $<TARGET_FILE:${target_name}>
    )
    set_tests_properties(${target_name}_run PROPERTIES
        WORKING_DIRECTORY "${REPO_ROOT}"
        ENVIRONMENT
            "HOST_SIM_REPORT=${HOST_SIM_OUT_DIR}/report-${optional_mode}-${backend}.json;HOST_SIM_STATS_REPORT=${HOST_SIM_OUT_DIR}/stats-${optional_mode}-${backend}.json;HOST_SIM_FS_ROOT=${HOST_SIM_OUT_DIR}/fs-${optional_mode}-${backend}"
    )
endfunction()

function(add_host_json_mode_target mode backend)
    set(target_name "json_mode_${mode}_${backend}")

    add_executable(${target_name}
        "${REPO_ROOT}/lib/Json/src/JSON.cpp"
        "${REPO_ROOT}/test/host_arduino_sim/host_json_mode_tests.cpp"
    )
    target_include_directories(${target_name} PRIVATE ${HOST_SIM_JSON_INCLUDE_DIRS})
    target_compile_definitions(${target_name} PRIVATE
        AL_NO_SERIAL
        _AVR_ATmega2560_
    )

    if(backend STREQUAL "littlefs")
        target_compile_definitions(${target_name} PRIVATE USE_LITTLEFS)
    endif()

    if(mode STREQUAL "optional-on")
        target_compile_definitions(${target_name} PRIVATE JSON_ENABLE_OPTIONAL_RETURNS=1)
        target_include_directories(${target_name} PRIVATE "${REPO_ROOT}/lib/Optional/src")
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${HOST_SIM_OUT_DIR}/bin"
    )

    add_test(NAME ${target_name}_run
        COMMAND $<TARGET_FILE:${target_name}>
    )
    set_tests_properties(${target_name}_run PROPERTIES
        WORKING_DIRECTORY "${REPO_ROOT}"
        ENVIRONMENT
            "HOST_JSON_MODE_REPORT=${HOST_SIM_OUT_DIR}/report-json-${mode}-${backend}.json;HOST_SIM_FS_ROOT=${HOST_SIM_OUT_DIR}/fs-json-${mode}-${backend}"
    )
endfunction()

function(add_host_stress_target optional_mode)
    set(target_name "host_stress_${optional_mode}")

    add_executable(${target_name}
        "${REPO_ROOT}/lib/Json/src/JSON.cpp"
        "${REPO_ROOT}/test/host_arduino_sim/host_stress_tests.cpp"
    )
    target_include_directories(${target_name} PRIVATE ${HOST_SIM_STRESS_INCLUDE_DIRS})
    target_compile_definitions(${target_name} PRIVATE
        AL_NO_SERIAL
        _AVR_ATmega2560_
    )

    if(optional_mode STREQUAL "on")
        target_compile_definitions(${target_name} PRIVATE
            JSON_ENABLE_OPTIONAL_RETURNS=1
            AVL_TREE_ENABLE_ERROR_CODES=1
        )
        target_include_directories(${target_name} PRIVATE "${REPO_ROOT}/lib/Optional/src")
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${HOST_SIM_OUT_DIR}/bin"
    )
endfunction()

add_host_stress_target("off")
add_host_stress_target("on")

foreach(backend IN ITEMS sd littlefs)
    add_host_integration_target("host_arduino_sim_off_${backend}" "${backend}" "off")
    add_host_integration_target("host_arduino_sim_on_${backend}" "${backend}" "on")

    add_host_json_mode_target("optional-off" "${backend}")
    add_host_json_mode_target("optional-on" "${backend}")
endforeach()

add_test(NAME host_experimental_unoqbridge_run
    COMMAND $<TARGET_FILE:host_experimental_unoqbridge>
)
set_tests_properties(host_experimental_unoqbridge_run PROPERTIES
    WORKING_DIRECTORY "${REPO_ROOT}"
)

if(HOST_SIM_ENABLE_STRESS_TESTS)
    foreach(stress_backend IN LISTS HOST_SIM_STRESS_BACKENDS)
        foreach(stress_optional IN LISTS HOST_SIM_STRESS_OPTIONAL_MODES)
            set(stress_runner_target "host_stress_${stress_optional}")
            set(stress_test_name "host_stress_uno_${stress_backend}_${stress_optional}")
            add_test(NAME ${stress_test_name}
                COMMAND $<TARGET_FILE:${stress_runner_target}>
            )
            set_tests_properties(${stress_test_name} PROPERTIES
                WORKING_DIRECTORY "${REPO_ROOT}"
                LABELS "stress"
                ENVIRONMENT
                    "HOST_MEM_LIMIT_BYTES=${HOST_SIM_STRESS_MEM_LIMIT_BYTES};HOST_STRESS_BOARD=uno;HOST_STRESS_SRAM_BYTES=2048;HOST_STRESS_BACKEND=${stress_backend};HOST_STRESS_OPTIONAL=${stress_optional};HOST_STRESS_PROGRESS_INTERVAL_MS=${HOST_SIM_STRESS_PROGRESS_INTERVAL_MS};HOST_STRESS_MAX_INSTANCES=${HOST_SIM_STRESS_MAX_INSTANCES};HOST_STRESS_MAX_ELEMENTS=${HOST_SIM_STRESS_MAX_ELEMENTS};HOST_STRESS_APPEND=0;HOST_STRESS_REPORT=${HOST_SIM_OUT_DIR}/stress-uno-${stress_backend}-${stress_optional}.json"
            )
        endforeach()
    endforeach()
endif()

add_custom_target(host_sim_all
    DEPENDS
        host_experimental_unoqbridge
        host_stress_off
        host_stress_on
        host_arduino_sim_off_sd
        host_arduino_sim_on_sd
        host_arduino_sim_off_littlefs
        host_arduino_sim_on_littlefs
        json_mode_optional-off_sd
        json_mode_optional-on_sd
        json_mode_optional-off_littlefs
        json_mode_optional-on_littlefs
)

message(STATUS "Host simulation CMake configured")
message(STATUS "Run tests with: ctest --test-dir <build-dir> --output-on-failure")
message(STATUS "Stress tests are enabled by default. Disable with -DHOST_SIM_ENABLE_STRESS_TESTS=OFF")
message(STATUS "Run only stress tests with: ctest --test-dir <build-dir> -L stress --output-on-failure")
message(STATUS "Stress backends: ${HOST_SIM_STRESS_BACKENDS}")
message(STATUS "Stress optional modes: ${HOST_SIM_STRESS_OPTIONAL_MODES}")