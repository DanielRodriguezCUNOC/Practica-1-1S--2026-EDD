# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Practica1-1s-2026-EDD_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Practica1-1s-2026-EDD_autogen.dir/ParseCache.txt"
  "Practica1-1s-2026-EDD_autogen"
  )
endif()
