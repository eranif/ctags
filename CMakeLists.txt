# Copyright (c) 2021, Aleksandr Platonov
#
# This source code is released for free distribution under the terms of the GNU General Public License version 2 or (at
# your option) any later version.
cmake_minimum_required(VERSION 3.16)

project(ctags)

include(CheckFunctionExists)
include(CheckIncludeFile)

option(ENABLE_STATIC "Enable static build" ON)
option(DISABLE_XML "Disable XML support" OFF)
option(DISABLE_JSON "Disable JSON support" OFF)
option(DISABLE_SECCOMP "Disable seccomp" OFF)
option(DISABLE_INTERNAL_SORT "Use external sort" ON)
option(DISABLE_ICONV "Disable iconv" OFF)
option(DISABLE_YAML "Disable yaml" OFF)

if(UNIX AND NOT APPLE)
  set(LIBS -static)
endif()

if(WIN32)
  execute_process(COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/win32/config_mingw.h ${CMAKE_CURRENT_LIST_DIR}/config.h
                  OUTPUT_VARIABLE _OUT)
  execute_process(
    COMMAND
      cp ${CMAKE_CURRENT_LIST_DIR}/win32/gnulib_h/langinfo.h ${CMAKE_CURRENT_LIST_DIR}/win32/gnulib_h/locale.h
      ${CMAKE_CURRENT_LIST_DIR}/win32/gnulib_h/unistd.h ${CMAKE_CURRENT_LIST_DIR}/win32/gnulib_h/fnmatch.h
      ${CMAKE_CURRENT_LIST_DIR}/win32/gnulib_h/string.h ${CMAKE_CURRENT_LIST_DIR}/win32/gnulib_h/wchar.h
      ${CMAKE_CURRENT_LIST_DIR}/gnulib
    OUTPUT_VARIABLE _OUT)

  include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/gnulib
    ${CMAKE_CURRENT_LIST_DIR}/main
    ${CMAKE_CURRENT_LIST_DIR}/parsers
    ${CMAKE_CURRENT_LIST_DIR}/optlib
    ${CMAKE_CURRENT_LIST_DIR}/extra-cmds
    ${CMAKE_CURRENT_LIST_DIR}/libreadtags
    ${CMAKE_CURRENT_LIST_DIR}/win32)
endif()

set(XML_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/parsers/maven2.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/dbusintrospect.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/glade.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/svg.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/plist.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/relaxng.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/xml.c
    ${CMAKE_CURRENT_LIST_DIR}/parsers/xslt.c)

set(YAML_SRCS ${CMAKE_CURRENT_LIST_DIR}/parsers/yaml.c ${CMAKE_CURRENT_LIST_DIR}/parsers/ansibleplaybook.c
              ${CMAKE_CURRENT_LIST_DIR}/parsers/openapi.c ${CMAKE_CURRENT_LIST_DIR}/parsers/yamlfrontmatter.c)

set(PCRE2_SRCS ${CMAKE_CURRENT_LIST_DIR}/main/lregex-pcre2.c)

file(GLOB MAIN_SRCS ${CMAKE_CURRENT_LIST_DIR}/main/*.c)
if(WIN32)
  set(DSL_SRCS ${CMAKE_CURRENT_LIST_DIR}/dsl/es.c ${CMAKE_CURRENT_LIST_DIR}/dsl/optscript.c)
  set(GNULIB_SRCS
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/regex.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/nl_langinfo.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/setlocale_null.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/malloc/dynarray_resize.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/fnmatch.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/mempcpy.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/wmempcpy.c
      ${CMAKE_CURRENT_LIST_DIR}/gnulib/localeconv.c)
else()
  file(GLOB DSL_SRCS ${CMAKE_CURRENT_LIST_DIR}/dsl/*.c)
endif()
set(RUBY_SRCS ${CMAKE_CURRENT_LIST_DIR}/parsers/i18nrubygem.c)

file(GLOB LIBREADTAGS_SRCS ${CMAKE_CURRENT_LIST_DIR}/libreadtags/*.c)

list(REMOVE_ITEM MAIN_SRCS ${CMAKE_CURRENT_LIST_DIR}/main/mini-geany.c)
file(GLOB OPTLIB_SRCS ${CMAKE_CURRENT_LIST_DIR}/optlib/*.c)
file(GLOB PARSERS_CXX_SRCS ${CMAKE_CURRENT_LIST_DIR}/parsers/cxx/*.c)
file(GLOB PARSERS_SRCS ${CMAKE_CURRENT_LIST_DIR}/parsers/*.c)

list(REMOVE_ITEM PARSERS_SRCS ${XML_SRCS})
list(REMOVE_ITEM PARSERS_SRCS ${YAML_SRCS})
list(REMOVE_ITEM PARSERS_SRCS ${RUBY_SRCS})
list(REMOVE_ITEM MAIN_SRCS ${PCRE2_SRCS})

if(DISABLE_INTERNAL_SORT)
  set(EXTERNAL_SORT 1)
  check_function_exists(setenv HAVE_SETENV)
  check_function_exists(putenv HAVE_PUTENV)
endif()

check_include_file(direct.h HAVE_DIRECT_H)
check_include_file(dirent.h HAVE_DIRENT_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(io.h HAVE_IO_H)
check_include_file(stat.h HAVE_STAT_H)
check_include_file(types.h HAVE_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(sys/dir.h HAVE_SYS_DIR_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(sys/wait.h HAVE_SYS_WAIT_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(regex.h HAVE_REGCOMP)

if(NOT HAVE_REGCOMP)
  set(REGEX_SRCS gnu_regex/regex.c)
  include_directories(gnu_regex)
  add_definitions(-DHAVE_REGCOMP -D__USE_GNU)
else()
  if(NOT MSVC)
    check_function_exists(regcomp HAVE_REGCOMP_LIBC)
    if(NOT HAVE_REGCOMP_LIBC)
      list(APPEND LIBS regex tre intl)
    endif()
  endif()
endif()
if(NOT DISABLE_ICONV)
  find_package(Iconv QUIET)
  if(Iconv_FOUND)
    set(HAVE_ICONV 1)
    list(APPEND LIBS ${Iconv_LIBRARIES})
    include_directories(${Iconv_INCLUDE_DIRS})
  endif()
endif()
check_include_file(fnmatch.h HAVE_FNMATCH_H)
if(NOT HAVE_FNMATCH_H)
  set(FNMATCH_SRCS gnulib/fnmatch.c)
  include_directories(fnmatch)
endif()

# check_include_file(seccomp.h HAVE_SECCOMP) if (HAVE_SECCOMP) list(APPEND LIBS seccomp) endif()

file(READ main/ctags.h CTAGS_H_CONTENTS)
string(REPLACE "\n" ";" CTAGS_H_LINES "${CTAGS_H_CONTENTS}")
foreach(CTAGS_H_LINE ${CTAGS_H_LINES})
  string(REGEX MATCH "PROGRAM_VERSION (\".*[0-9]\")" _ ${CTAGS_H_LINE})
  if(CMAKE_MATCH_1)
    set(PACKAGE_VERSION ${CMAKE_MATCH_1})
    break()
  endif()
endforeach()
check_function_exists(mkstemp HAVE_MKSTEMP)
if(NOT HAVE_MKSTEMP)
  set(MKSTEMP_SRCS win32/mkstemp/mkstemp.c)
endif()
if(WIN32)
  add_definitions(-DWIN32)
  if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -Dstrcasecmp=stricmp -D_CONSOLE)
  endif()
endif()

configure_file(config.h.in.cmake "${CMAKE_BINARY_DIR}/config.h")
include_directories(${CMAKE_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_H)
include_directories(main)
include_directories(dsl)
include_directories(libreadtags)

add_executable(
  ctags
  ${GNULIB_SRCS}
  ${MAIN_SRCS}
  ${DSL_SRCS}
  ${OPTLIB_SRCS}
  ${PARSERS_SRCS}
  ${PARSERS_CXX_SRCS}
  ${MKSTEMP_SRCS}
  ${REGEX_SRCS}
  ${LIBREADTAGS_SRCS}
  ${FNMATCH_SRCS})
target_link_libraries(ctags ${LIBS})
