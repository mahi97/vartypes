cmake_minimum_required(VERSION 3.16)
project(vartypes VERSION 0.8.0 LANGUAGES CXX)

# ─── Build options ───
option(BUILD_GUI       "Build the vartypes-widgets GUI library"     ON)
option(BUILD_EXAMPLES  "Build example applications"                 OFF)
option(BUILD_TESTS     "Build unit tests"                           OFF)

# ─── Global settings ───
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Place all runtime binaries in a common directory so that
# on Windows, executables can find the shared library without PATH changes.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ─── Find Qt (Qt6 preferred, Qt5 fallback) ───
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Xml)
message(STATUS "Using Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

# ═══════════════════════════════════════════════════════════════════════
# vartypes-core: Non-GUI library (types, serialization, factory)
# ═══════════════════════════════════════════════════════════════════════
set(CORE_SRCS
  vartypes/VarBase64.cpp
  vartypes/VarNotifier.cpp
  vartypes/VarTypes.cpp
  vartypes/VarXML.cpp
  vartypes/VarJSON.cpp
  vartypes/VarTypesInstance.cpp
  vartypes/VarTypesFactory.cpp
  vartypes/xml/xmlParser.cpp

  vartypes/primitives/VarStringVal.cpp
  vartypes/primitives/VarBoolVal.cpp
  vartypes/primitives/VarVal.cpp
  vartypes/primitives/VarIntVal.cpp
  vartypes/primitives/VarDoubleVal.cpp

  vartypes/primitives/VarType.cpp
  vartypes/primitives/VarBlob.cpp
  vartypes/primitives/VarBool.cpp
  vartypes/primitives/VarDouble.cpp
  vartypes/primitives/VarExternal.cpp
  vartypes/primitives/VarInt.cpp
  vartypes/primitives/VarList.cpp
  vartypes/primitives/VarQWidget.cpp
  vartypes/primitives/VarSelection.cpp
  vartypes/primitives/VarString.cpp
  vartypes/primitives/VarStringEnum.cpp
  vartypes/primitives/VarTrigger.cpp

  vartypes/primitives/VarProtoBufferVal.cpp
  vartypes/primitives/VarProtoBuffer.cpp
  vartypes/primitives/VarAnyVal.cpp
  vartypes/primitives/VarAny.cpp
)

set(CORE_HEADERS
  vartypes/VarBase64.h
  vartypes/VarNotifier.h
  vartypes/VarTypes.h
  vartypes/VarXML.h
  vartypes/VarJSON.h
  vartypes/VarTypesInstance.h
  vartypes/VarTypesFactory.h
  vartypes/VarTypesLoaderInterface.h
  vartypes/xml/xmlParser.h

  vartypes/primitives/VarVal.h
  vartypes/primitives/VarStringVal.h
  vartypes/primitives/VarBoolVal.h
  vartypes/primitives/VarIntVal.h
  vartypes/primitives/VarDoubleVal.h

  vartypes/primitives/VarType.h
  vartypes/primitives/VarBlob.h
  vartypes/primitives/VarBool.h
  vartypes/primitives/VarDouble.h
  vartypes/primitives/VarExternal.h
  vartypes/primitives/VarInt.h
  vartypes/primitives/VarList.h
  vartypes/primitives/VarQWidget.h
  vartypes/primitives/VarSelection.h
  vartypes/primitives/VarString.h
  vartypes/primitives/VarStringEnum.h
  vartypes/primitives/VarTrigger.h

  vartypes/primitives/VarProtoBufferVal.h
  vartypes/primitives/VarProtoBuffer.h
  vartypes/primitives/VarAnyVal.h
  vartypes/primitives/VarAny.h
)

add_library(vartypes-core SHARED ${CORE_SRCS} ${CORE_HEADERS})
set_target_properties(vartypes-core PROPERTIES
  VERSION   ${PROJECT_VERSION}
  SOVERSION 1
  OUTPUT_NAME vartypes-core
)

target_compile_features(vartypes-core PUBLIC cxx_std_17)

target_include_directories(vartypes-core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vartypes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vartypes/primitives>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vartypes/gui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vartypes/xml>
    $<INSTALL_INTERFACE:include/vartypes>
)

target_link_libraries(vartypes-core
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
)

if(WIN32)
  set_target_properties(vartypes-core PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ═══════════════════════════════════════════════════════════════════════
# vartypes-widgets: GUI library (tree view, item delegate, model)
# ═══════════════════════════════════════════════════════════════════════
if(BUILD_GUI)
  set(WIDGET_SRCS
    vartypes/gui/VarItem.cpp
    vartypes/gui/VarItemDelegate.cpp
    vartypes/gui/VarTreeModel.cpp
    vartypes/gui/VarTreeView.cpp
    vartypes/gui/VarTreeViewOptions.cpp
  )

  set(WIDGET_HEADERS
    vartypes/gui/VarItem.h
    vartypes/gui/VarItemDelegate.h
    vartypes/gui/VarTreeModel.h
    vartypes/gui/VarTreeView.h
    vartypes/gui/VarTreeViewOptions.h
  )

  set(WIDGET_RC
    vartypes/gui/icons/icons.qrc
  )

  add_library(vartypes-widgets SHARED ${WIDGET_SRCS} ${WIDGET_HEADERS} ${WIDGET_RC})
  set_target_properties(vartypes-widgets PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME vartypes-widgets
  )

  target_compile_features(vartypes-widgets PUBLIC cxx_std_17)

  target_link_libraries(vartypes-widgets
    PUBLIC
      vartypes-core
      Qt${QT_VERSION_MAJOR}::Widgets
  )

  if(WIN32)
    set_target_properties(vartypes-widgets PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  endif()
endif()

# ═══════════════════════════════════════════════════════════════════════
# Backward-compatible combined 'vartypes' target
# ═══════════════════════════════════════════════════════════════════════
if(BUILD_GUI)
  add_library(vartypes INTERFACE)
  target_link_libraries(vartypes INTERFACE vartypes-core vartypes-widgets)
else()
  add_library(vartypes ALIAS vartypes-core)
endif()

# ═══════════════════════════════════════════════════════════════════════
# Examples
# ═══════════════════════════════════════════════════════════════════════
if(BUILD_EXAMPLES)
  if(NOT BUILD_GUI)
    message(FATAL_ERROR "BUILD_EXAMPLES requires BUILD_GUI=ON")
  endif()

  add_executable(vartype_example
    example/src/mainwindow.h
    example/src/mainwindow.cpp
    example/src/example.cpp
  )
  target_link_libraries(vartype_example PRIVATE vartypes)
endif()

# ═══════════════════════════════════════════════════════════════════════
# Tests
# ═══════════════════════════════════════════════════════════════════════
if(BUILD_TESTS)
  enable_testing()
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
  add_subdirectory(tests)
endif()

# ═══════════════════════════════════════════════════════════════════════
# Install
# ═══════════════════════════════════════════════════════════════════════
include(GNUInstallDirs)

install(FILES ${CORE_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vartypes
)

if(BUILD_GUI)
  install(FILES ${WIDGET_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vartypes/gui
  )
endif()

install(TARGETS vartypes-core
  EXPORT vartypes-targets
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(BUILD_GUI)
  install(TARGETS vartypes-widgets
    EXPORT vartypes-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()

install(FILES README.md COPYING
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

