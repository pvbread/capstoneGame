set(HEADER_FILES
	catch.hpp
)

set(SOURCE_FILES	
	test.cpp
)

set(LIBRARY_NAME pythonMigration)

add_executable(test ${SOURCE_FILES})
target_link_libraries(test ${LIBRARY_NAME})
