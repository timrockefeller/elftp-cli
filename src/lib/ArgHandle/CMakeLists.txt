set(TARGET_NAME "ArgHandle")
set(STR_TARGET_SOURCES "ArgHandle.cpp Command.cpp ${CMAKE_SOURCE_DIR}/include/ArgHandle.h ${CMAKE_SOURCE_DIR}/include/Singleton.h" )
set(STR_TARGET_LIBS " ")

SETUP_PROJECT(${MODE} ${TARGET_NAME} ${STR_TARGET_SOURCES} ${STR_TARGET_LIBS})
