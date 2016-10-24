set(
	QHTTP_BASE_DIR
	"${PROJECT_SOURCE_DIR}/../_vendor-src/qhttp"
)
find_path(
	QHTTP_INCLUDE_DIRS
	NAMES qhttpserverrequest.hpp
	HINTS "${QHTTP_BASE_DIR}/src"
)
find_library(
	QHTTP_LIBRARY_DEBUG
	NAMES qhttp
	HINTS "${QHTTP_BASE_DIR}/build-debug/xbin"
)
find_library(
	QHTTP_LIBRARY_RELEASE
	NAMES qhttp
	HINTS "${QHTTP_BASE_DIR}/build-release/xbin"
)
if(QHTTP_INCLUDE_DIRS AND QHTTP_LIBRARY_DEBUG AND QHTTP_LIBRARY_RELEASE)
	set(QHTTP_FOUND TRUE)
	set(
		QHTTP_LIBRARY
		debug ${QHTTP_LIBRARY_DEBUG}
		optimized ${QHTTP_LIBRARY_RELEASE}
	)
endif()