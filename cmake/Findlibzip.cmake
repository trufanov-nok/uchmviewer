# Tries to find libzip

cmake_minimum_required(VERSION 3.0)

find_path(libzip_INCLUDE_DIR NAMES zip.h
    PATHS ${libzip_ROOT}
    PATH_SUFFIXES libzip/include include)

find_library(libzip_LIBRARY NAMES zip
    PATHS ${libzip_ROOT}
    PATH_SUFFIXES libzip/lib lib)

if (libzip_INCLUDE_DIR AND libzip_LIBRARY)
   set(libzip_FOUND TRUE)
endif ()


if (libzip_FOUND)
   if (NOT libzip_FIND_QUIETLY)
      message(STATUS "Found libzip: ${libzip_LIBRARY}")
   endif ()

   add_library(libzip::zip SHARED IMPORTED)
   set_target_properties(libzip::zip PROPERTIES
         INTERFACE_INCLUDE_DIRECTORIES ${libzip_INCLUDE_DIR}
         IMPORTED_LOCATION ${libzip_LIBRARY}
         IMPORTED_IMPLIB ${libzip_LIBRARY} # Windows
   )
elseif (libzip_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libzip. Please install libzip and libzip-devel packages.")
endif ()
