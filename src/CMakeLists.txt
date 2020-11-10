project( digitization )

cmake_minimum_required(VERSION 3.1)
#aux_source_directory(. SRC_LIST)

#Required in DGtal
FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

#Required in Gmp
set(GMP_DIR "/usr")
include_directories(${PROJECT_NAME} ${GMP_DIR}/include)
link_directories(${PROJECT_NAME} ${GMP_DIR}/lib)
find_library(GMP_LIBRARY NAMES gmp libgmp PATHS "${GMP_DIR}")

add_definitions(-g)

#add_executable(${PROJECT_NAME} ${SRC_LIST})
#add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME} gmp gmpxx ${DGTAL_LIBRARIES})
add_executable(digitizeImplicitCurvedObject digitizeImplicitCurvedObject)
target_link_libraries(digitizeImplicitCurvedObject gmp gmpxx ${DGTAL_LIBRARIES})
add_executable(digitizeRationalDisk digitizeRationalDisk.cpp)
target_link_libraries(digitizeRationalDisk gmp gmpxx ${DGTAL_LIBRARIES})
add_executable(digitizeRationalEllipse digitizeRationalEllipse.cpp)
target_link_libraries(digitizeRationalEllipse gmp gmpxx ${DGTAL_LIBRARIES})
