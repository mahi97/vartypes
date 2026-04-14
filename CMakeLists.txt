cmake_minimum_required(VERSION 3.16)
project(vartypes VERSION 0.8.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Try Qt6 first, then fall back to Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

message(STATUS "Using Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

set(src_dir .)

set (SRCS
  ${src_dir}/vartypes/VarBase64.cpp
  ${src_dir}/vartypes/VarNotifier.cpp
  ${src_dir}/vartypes/VarTypes.cpp
  ${src_dir}/vartypes/VarXML.cpp
  ${src_dir}/vartypes/VarTypesInstance.cpp
  ${src_dir}/vartypes/VarTypesFactory.cpp
  ${src_dir}/vartypes/xml/xmlParser.cpp

  ${src_dir}/vartypes/primitives/VarStringVal.cpp
  ${src_dir}/vartypes/primitives/VarBoolVal.cpp
  ${src_dir}/vartypes/primitives/VarVal.cpp
  ${src_dir}/vartypes/primitives/VarIntVal.cpp
  ${src_dir}/vartypes/primitives/VarDoubleVal.cpp

  ${src_dir}/vartypes/primitives/VarType.cpp
  ${src_dir}/vartypes/primitives/VarBlob.cpp
  ${src_dir}/vartypes/primitives/VarBool.cpp
  ${src_dir}/vartypes/primitives/VarDouble.cpp
  ${src_dir}/vartypes/primitives/VarExternal.cpp
  ${src_dir}/vartypes/primitives/VarInt.cpp
  ${src_dir}/vartypes/primitives/VarList.cpp
  ${src_dir}/vartypes/primitives/VarQWidget.cpp
  ${src_dir}/vartypes/primitives/VarSelection.cpp
  ${src_dir}/vartypes/primitives/VarString.cpp
  ${src_dir}/vartypes/primitives/VarStringEnum.cpp
  ${src_dir}/vartypes/primitives/VarTrigger.cpp

  ${src_dir}/vartypes/gui/VarItem.cpp
  ${src_dir}/vartypes/gui/VarItemDelegate.cpp
  ${src_dir}/vartypes/gui/VarTreeModel.cpp
  ${src_dir}/vartypes/gui/VarTreeView.cpp
  ${src_dir}/vartypes/gui/VarTreeViewOptions.cpp

  ${src_dir}/vartypes/primitives/VarProtoBufferVal.cpp
  ${src_dir}/vartypes/primitives/VarProtoBuffer.cpp

  ${src_dir}/vartypes/primitives/VarAnyVal.cpp
  ${src_dir}/vartypes/primitives/VarAny.cpp
)

set (HEADERS
  ${src_dir}/vartypes/primitives/VarType.h
  ${src_dir}/vartypes/primitives/VarBlob.h
  ${src_dir}/vartypes/primitives/VarBool.h
  ${src_dir}/vartypes/primitives/VarDouble.h
  ${src_dir}/vartypes/primitives/VarExternal.h
  ${src_dir}/vartypes/primitives/VarInt.h
  ${src_dir}/vartypes/primitives/VarList.h
  ${src_dir}/vartypes/primitives/VarQWidget.h
  ${src_dir}/vartypes/primitives/VarSelection.h
  ${src_dir}/vartypes/primitives/VarString.h
  ${src_dir}/vartypes/primitives/VarStringEnum.h
  ${src_dir}/vartypes/primitives/VarTrigger.h
  ${src_dir}/vartypes/gui/VarItem.h
  ${src_dir}/vartypes/gui/VarItemDelegate.h
  ${src_dir}/vartypes/gui/VarTreeModel.h
  ${src_dir}/vartypes/gui/VarTreeView.h
  ${src_dir}/vartypes/VarNotifier.h
)

set (INSTALLABLE_HEADERS
  ${HEADERS}
  ${src_dir}/vartypes/VarTypesLoaderInterface.h
  ${src_dir}/vartypes/VarTypes.h
  ${src_dir}/vartypes/VarTypesFactory.h
  ${src_dir}/vartypes/gui/VarTreeViewOptions.h
  ${src_dir}/vartypes/xml/xmlParser.h
  ${src_dir}/vartypes/primitives/VarVal.h
  ${src_dir}/vartypes/primitives/VarStringVal.h
  ${src_dir}/vartypes/primitives/VarIntVal.h
  ${src_dir}/vartypes/primitives/VarDoubleVal.h
  ${src_dir}/vartypes/primitives/VarBoolVal.h
  ${src_dir}/vartypes/VarXML.h
  ${src_dir}/vartypes/VarTypesInstance.h
  ${src_dir}/vartypes/VarBase64.h
  ${src_dir}/vartypes/primitives/VarProtoBufferVal.h
  ${src_dir}/vartypes/primitives/VarProtoBuffer.h
  ${src_dir}/vartypes/primitives/VarAnyVal.h
  ${src_dir}/vartypes/primitives/VarAny.h
)

set (INSTALLABLE_DOCFILES
  ${src_dir}/README
  ${src_dir}/COPYING
)

set (RC
  ${src_dir}/vartypes/gui/icons/icons.qrc
)

#flags to set in debug mode
set (CMAKE_CXX_FLAGS_DEBUG "-g")

#flags to set in release mode
if(NOT MSVC)
  set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
else()
  set (CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /O2")
endif()

# On Windows, place all DLLs and EXEs in the same directory so that
# executables can find the shared library at runtime without PATH changes.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

SET(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")
SET(CMAKE_INSTALL_INCLUDEDIR include CACHE PATH "Output directory for header files")
SET(CMAKE_INSTALL_DOCDIR share/doc CACHE PATH "Output directory for documentation and license files")

set (target vartypes)
add_library(${target} SHARED ${SRCS} ${HEADERS} ${RC})
set_target_properties(${target} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
target_include_directories(${target} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/vartypes
  ${CMAKE_CURRENT_SOURCE_DIR}/vartypes/primitives
  ${CMAKE_CURRENT_SOURCE_DIR}/vartypes/gui
  ${CMAKE_CURRENT_SOURCE_DIR}/vartypes/xml
)
target_link_libraries(${target} PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
)

# Windows DLL export
if(WIN32)
  set_target_properties(${target} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set (example_target vartype_example)
set (example_dir example)

add_executable(${example_target}
               ${example_dir}/src/mainwindow.h
               ${example_dir}/src/mainwindow.cpp
               ${example_dir}/src/example.cpp)

target_link_libraries(${example_target} PRIVATE ${target})

# ─── Tests ───
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
  enable_testing()
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
  add_subdirectory(tests)
endif()

INSTALL(FILES ${INSTALLABLE_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vartypes)

INSTALL(FILES ${INSTALLABLE_DOCFILES} DESTINATION ${CMAKE_INSTALL_DOCDIR}/vartypes)

INSTALL(TARGETS ${target}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION bin
)

