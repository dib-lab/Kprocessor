# Still needs
## [ ] Build & Release options
## [ ] Build with debug option
## [ ] Build test cases option
## [ ] Build and generate static library option
## change the static linking options to something cmake


# Minimum cmake version
cmake_minimum_required (VERSION 3.14...3.20)

# Name of project and that it is C/C++.
project (
        kProcessor
        LANGUAGES CXX C
        VERSION 1.1
)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_APPS "Build kProcessor apps" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_ALL "Build everything!" OFF)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
# IMPORTANT == -FPIC #SDSL-LINKING
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()


include_directories("ThirdParty/CLI")
include_directories("ThirdParty/MQF/include")
include_directories("ThirdParty/json")
include_directories("ThirdParty/sdsl-lite/include")
include_directories("ThirdParty/kmerDecoder/include")
include_directories("ThirdParty/kmerDecoder/lib/parallel_hashmap")
include_directories("ThirdParty/cereal/include")
include_directories("ThirdParty/ntCard/include")


add_subdirectory(ThirdParty/MQF/)
add_subdirectory(ThirdParty/sdsl-lite/)
add_subdirectory(ThirdParty/kmerDecoder)



#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}  -std=c++17 -fPIC  -pedantic -O0 -g -lprofiler")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17 -fPIC  -pedantic  -O3 -Ofast")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}  -std=c++17 -fPIC  -pedantic -O0 -g ")

set (CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS}  -std=c++17 -fPIC  -pedantic -O3 -Ofast")

set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
#set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lprofiler")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


include(ExternalProject)
ExternalProject_Add(
  libntcard_a
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/ntCard
  CONFIGURE_COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/ntCard/autogen.sh && ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/ntCard/configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/ntCard/  CXXFLAGS=-fPIC
  BUILD_COMMAND make
  BUILD_IN_SOURCE 1
)
set(ntcardLib "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/ntCard/libntcard.a")

set(LIBRARY_NAME
    kProcessor
)

file(GLOB kProcessorLibSrc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extend_algorithms.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/batchQuery.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/colorTable.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/kDataFrames/colored_kDataFrame.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/kDataFrames/kDataFrameBMQF.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/kDataFrames/kDataFrame.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/kDataFrames/kDataFrameMAP.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/kDataFrames/kDataFrameMQF.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/kDataFrames/kDataFramePHMAP.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/utils.cpp
        )

add_library(${LIBRARY_NAME} STATIC
    ${kProcessorLibSrc}
    )


target_link_libraries(kProcessor MQF)
target_link_libraries (kProcessor ${ntcardLib}  -ldl)

#set_property(TARGET sdsl PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(kProcessor sdsl)
target_link_libraries(kProcessor kmerDecoder)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)


set(LDFLAGS "-lrt -lpthread -lbz2 -lz")


if(BUILD_ALL)
    SET(BUILD_TESTS ON)
    # SET(BUILD_APPS ON) # Deferred
    SET(BUILD_DOCS ON)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests/)
endif()

if(BUILD_APPS)
    # add_subdirectory(apps/) # Deferred
    message("WARNING: Apps are not available in this release")
endif()

#### Building Documentation with Doxygen

if(BUILD_DOCS)
    if(NOT ${DOXYGEN_FOUND})
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    else()


        include(cmake/CPM.cmake)
        CPMAddPackage(
            NAME MCSS
            DOWNLOAD_ONLY YES
            GITHUB_REPOSITORY mosra/m.css
            GIT_TAG 42d4a9a48f31f5df6e246c948403b54b50574a2a
        )
            
    # set Doxyfile variables
        set(DOXYGEN_PROJECT_NAME kProcessor)
        set(DOXYGEN_PROJECT_VERSION ${kProcessor_VERSION})
        set(DOXYGEN_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}")
        set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")

        configure_file(${CMAKE_CURRENT_LIST_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${CMAKE_CURRENT_LIST_DIR}/conf.py ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

        add_custom_target(
                GenerateDocs
                ${CMAKE_COMMAND} -E make_directory "${DOXYGEN_OUTPUT_DIRECTORY}"
                COMMAND "${MCSS_SOURCE_DIR}/documentation/doxygen.py" "${CMAKE_CURRENT_BINARY_DIR}/conf.py"
                COMMAND echo "Docs written to: ${DOXYGEN_OUTPUT_DIRECTORY}"
                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        )
    endif()
endif()
