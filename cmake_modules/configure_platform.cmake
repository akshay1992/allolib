# CHECK PLATFORM ###########################################
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(MACOS 1)
  message("MACOS")
  add_definitions(-DAL_OSX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LINUX 1)
  message("LINUX")
  add_definitions(-DAL_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if (MSYS)
    set(WINDOWS_CYGWIN 1)
    message("WINDOWS_CYGWIN")
    # later change to AL_WINDOWS_MSYS?
    add_definitions(-DAL_WINDOWS)
  else () # WIN32 (NOTE: "MSVC" is defined at visual studio)
    set(WINDOWS 1)
    message("WINDOWS")
    add_definitions(-DAL_WINDOWS)
  endif()
else ()
endif ()