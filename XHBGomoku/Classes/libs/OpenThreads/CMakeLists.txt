project(OpenThreads)

set(HEADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/OpenThreads)
set(OpenThreads_PUBLIC_HEADERS
    ${HEADER_PATH}/Atomic
    ${HEADER_PATH}/Barrier
    ${HEADER_PATH}/Block
    ${HEADER_PATH}/Condition
    ${HEADER_PATH}/Exports
    ${HEADER_PATH}/Mutex  
    ${HEADER_PATH}/ReadWriteMutex  
    ${HEADER_PATH}/ReentrantMutex  
    ${HEADER_PATH}/ScopedLock  
    ${HEADER_PATH}/Thread
    ${HEADER_PATH}/Version
)
source_group(include FILES ${OpenThreads_PUBLIC_HEADERS})

set(COMMON_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Atomic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Version.cpp
)
source_group(src\\common FILES ${COMMON_SRC})

set(PTHREAD_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadBarrier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadBarrierPrivateData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadCondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadConditionPrivateData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadMutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadMutexPrivateData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pthreads/PThreadPrivateData.h
)
source_group(src\\pthreads FILES ${PTHREAD_SRC})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

if (APPLE OR ANDROID)
    add_library(OpenThreads STATIC ${OpenThreads_PUBLIC_HEADERS} ${COMMON_SRC} ${PTHREAD_SRC})
endif()
