add_subdirectory (lib) #includes
add_subdirectory (test) #wip tests

set(MODE "EXE")
set(TARGET_NAME "elftp")

file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
set(STR_TARGET_SOURCES "")
foreach(SOURCE ${ALL_SOURCES})
	set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${SOURCE}")
endforeach(SOURCE ${ALL_SOURCES})
set(STR_TARGET_LIBS "FTPAPI View ArgHandle")

SETUP_PROJECT(${MODE} ${TARGET_NAME} ${STR_TARGET_SOURCES} ${STR_TARGET_LIBS})
