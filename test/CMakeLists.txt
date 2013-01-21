include_directories(../include)
include_directories(${Boost_INCLUDE_DIR})

# A function used to add logging test executable.
function(add_logging_test target_name)
    set(src_list    ${ARGV})
    list(REMOVE_AT src_list 0)
    add_executable(${target_name} ${src_list})
    target_link_libraries(${target_name} log4boost)
    add_dependencies(${target_name} log4boost)
    link_boost(${target_name} date_time filesystem thread)
    message(STATUS  "Add a test: "${target_name})
endfunction(add_logging_test)


add_logging_test(unit_test unit_test.cpp)
add_logging_test(test_file_appender test_file_appender.cpp)
add_logging_test(test_udp_appender test_udp_appender.cpp)

add_logging_test(test_configure test_configure.cpp)
add_logging_test(test_preconfigure test_preconfigure.cpp)
add_logging_test(test_configure_and_watch_0 test_configure_and_watch_0.cpp)
add_logging_test(test_configure_and_watch_1 test_configure_and_watch_1.cpp)

add_logging_test(test_console_appender test_console_appender.cpp)
add_logging_test(test_ipc_receiver test_ipc_receiver.cpp)
add_logging_test(test_ipc_appender test_ipc_appender.cpp)
add_logging_test(test_udp_receiver test_udp_receiver.cpp)

add_logging_test(test_tcp_appender test_tcp_appender.cpp)

if(WIN32)
    add_logging_test(test_log_agent_appender test_log_agent_appender.cpp)
    add_logging_test(test_win32_debug_appender test_win32_debug_appender.cpp)
endif(WIN32)


