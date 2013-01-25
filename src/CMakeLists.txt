# appender
set(appender_src
	appender.cpp
	layout_appender.cpp
	ostream_appender.cpp
	udp_appender.cpp
	file_appender.cpp
    #ipc_appender.cpp
	rolling_file_appender.cpp
    console_appender.cpp
	tcp_appender.cpp
    log_agent_appender.cpp
	)

set(appender_header
	../include/log4boost/appender/appender.hpp
	../include/log4boost/appender/layout_appender.hpp
	../include/log4boost/appender/ostream_appender.hpp
	../include/log4boost/appender/udp_appender.hpp
    #../include/log4boost/appender/ipc_appender.hpp
	../include/log4boost/appender/file_appender.hpp
	../include/log4boost/appender/rolling_file_appender.hpp
    ../include/log4boost/appender/console_appender.hpp
	../include/log4boost/appender/tcp_appender.hpp
    ../include/log4boost/appender/log_agent_appender.hpp
	)

source_group("Header Files\\Appender" FILES	${appender_header})
source_group("Source Files\\Appender" FILES	${appender_src})

# layout
set(layout_src
	layout.cpp
	simple_xml_layout.cpp
	formatter_layout.cpp
	binary_layout.cpp
	log4j_xml_layout.cpp
	)

set(layout_header
	../include/log4boost/layout/layout.hpp
	../include/log4boost/layout/formatter_layout.hpp
	../include/log4boost/layout/simple_xml_layout.hpp
	../include/log4boost/layout/binary_layout.hpp
	../include/log4boost/layout/log4j_xml_layout.hpp
	)

source_group("Header Files\\Layout" FILES ${layout_header})
source_group("Source Files\\Layout" FILES ${layout_src})

if(MSVC)
    set(appender_header
        ${appender_header}
        ../include/log4boost/appender/win32_debug_appender.hpp
        )
    set(appender_src
        ${appender_src}
	    win32_debug_appender.cpp
        )
endif(MSVC)

# detail
set(detail_src
	ptree.cpp
	transform.cpp
	escape_string.cpp
	singleton.cpp
	hierarchy_maintainer.cpp
    date_time.cpp
    mutex.cpp
	)

set(detail_header
	../include/log4boost/detail/stack_ostream.hpp
	../include/log4boost/detail/thread_id.hpp
	../include/log4boost/detail/process_id.hpp
	../include/log4boost/detail/time_format_holder.hpp
	../include/log4boost/detail/singleton.hpp
	../include/log4boost/detail/hierarchy_maintainer.hpp
	transform.h
	ptree.h
	ptree_inc.h
	utf8.hpp
	escape_string.hpp
    io.hpp
	)

source_group("Header Files\\Detail" FILES ${detail_header})
source_group("Source Files\\Detail" FILES ${detail_src})

set(receiver_src
	receiver.cpp
	udp_receiver.cpp
    #ipc_receiver.cpp
    tcp_receiver.cpp
	)

set(receiver_header
	../include/log4boost/receiver/receiver.hpp
	../include/log4boost/receiver/udp_receiver.hpp
    #../include/log4boost/receiver/ipc_receiver.hpp
	../include/log4boost/receiver/tcp_receiver.hpp
	)

source_group("Header Files\\Receiver" FILES ${receiver_header})
source_group("Source Files\\Receiver" FILES ${receiver_src})

# writer
set(writer_src
    file_writer.cpp
    rolling_file_writer.cpp
    tcp_writer.cpp
    )

set(writer_header
    ../include/log4boost/writer/writer.hpp
    ../include/log4boost/writer/file_writer.hpp
    ../include/log4boost/writer/rolling_file_writer.hpp
    ../include/log4boost/writer/tcp_writer.hpp
    )

source_group("Header Files\\Writer" FILES ${writer_header})
source_group("Source Files\\Writer" FILES ${writer_src})


# other
set(logging_src
	${appender_src}
	${layout_src}
	${detail_src}
	${receiver_src}
    ${writer_src}
	stdafx.cpp
	logger.cpp
	logging_event.cpp
	ndc.cpp
	configurator.cpp
	appender_factory.cpp
	priority.cpp
	layout_factory.cpp
	scoped_tracer.cpp
	log4boost.cpp
	config_watch_dog.cpp
	file_watch_dog.cpp
	)

set(logging_header
	${appender_header}
	${layout_header}
	${detail_header}
	${receiver_header}
    ${writer_header}
	stdafx.h
	../include/log4boost/logger.hpp
	../include/log4boost/logging_event.hpp
	../include/log4boost/ndc.hpp
	../include/log4boost/log4boost.hpp
	../include/log4boost/configurator.hpp
	../include/log4boost/config.hpp
	../include/log4boost/priority.hpp
	../include/log4boost/layout_factory.hpp
	../include/log4boost/appender_factory.hpp
	../include/log4boost/scoped_tracer.hpp
	../include/log4boost/config_watch_dog.hpp
	../include/log4boost/file_watch_dog.hpp
	../include/log4boost/appender_creator_base.hpp
    ../include/log4boost/rolling_file_settings.hpp
	)

include_directories(../include)

# log4boost lib
if(BUILD_STATIC_LIB)
	add_library(log4boost STATIC
		${logging_src}
		${logging_header}
		)
else(BUILD_STATIC_LIB)
	add_library(log4boost SHARED
		${logging_src}
		${logging_header}
		)
endif(BUILD_STATIC_LIB)

#link_boost(log4boost program_options date_time filesystem thread)
target_link_libraries(log4boost ${Boost_LIBRARIES} )

if(BUILD_OUTPUT_NAME_WITH_CONFIG_SUFFIX)
	set_target_properties(log4boost PROPERTIES
		DEBUG_OUTPUT_NAME			"log4boost_Debug"
		RELEASE_OUTPUT_NAME			"log4boost_Release"
		RELWITHDEBINFO_OUTPUT_NAME	"log4boost_RelWithDebInfo"
		MINSIZEREL_OUTPUT_NAME		"log4boost_MinSizeRel"
		)
endif(BUILD_OUTPUT_NAME_WITH_CONFIG_SUFFIX)


# Set precompile header. Only on windows
include(MSVCPCH)
use_msvc_pch(log4boost stdafx.h stdafx.cpp)


# log agent
add_executable(log_agent
    log_agent_main.cpp
    )
link_boost(log_agent system program_options date_time filesystem thread)

target_link_libraries(log_agent log4boost)
add_dependencies(log_agent log4boost)

# log server
add_executable(log_server
    log_server_main.cpp
    )
#link_boost(log_server signals system program_options date_time filesystem thread)
target_link_libraries(log_server ${Boost_LIBRARIES} )

target_link_libraries(log_server log4boost)
add_dependencies(log_server log4boost)



# install
install(DIRECTORY ../include DESTINATION ./ PATTERN ".svn" EXCLUDE)

install(TARGETS log4boost log_agent
	ARCHIVE	DESTINATION	lib
	LIBRARY DESTINATION lib
	RUNTIME	DESTINATION bin
	)




