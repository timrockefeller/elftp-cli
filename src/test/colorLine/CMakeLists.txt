GET_DIR_NAME(DIRNAME)
set(FOLDER_NAME ${DIRNAME})
set(TARGET_NAME "${TARGET_PREFIX}${DIRNAME}")
file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/*.vs"
	# "${CMAKE_CURRENT_SOURCE_DIR}/*.gs"
    # "${CMAKE_CURRENT_SOURCE_DIR}/*.fs"
)
set(STR_TARGET_SOURCES "")
foreach(SOURCE ${ALL_SOURCES})
	set(STR_TARGET_SOURCES "${STR_TARGET_SOURCES} ${SOURCE}")
endforeach(SOURCE ${ALL_SOURCES})
set(STR_TARGET_LIBS " ")
SETUP_PROJECT(${MODE} ${TARGET_NAME} ${STR_TARGET_SOURCES} ${STR_TARGET_LIBS})
