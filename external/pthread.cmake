# Find Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

if(NOT CMAKE_USE_PTHREADS_INIT AND NOT CMAKE_USE_HP_PTHREAD_INIT)
  message(FATAL_ERROR "TiledArray requires pthreads.")
endif()

set(TiledArray_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} ${TiledArray_LIBRARIES})