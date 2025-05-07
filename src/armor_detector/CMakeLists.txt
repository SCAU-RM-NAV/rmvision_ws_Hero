cmake_minimum_required(VERSION 3.10)
project(armor_detector)

## Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OPENVINO_ROOT "/opt/intel/openvino_2024")
## By adding -Wall and -Werror, the compiler does not ignore warnings anymore,
## enforcing cleaner code.
add_definitions(-Wall -Werror)

## Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

#######################
## Find dependencies ##
#######################

find_package(ament_cmake_auto REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Ceres REQUIRED)
find_package(glog REQUIRED)
find_package(fmt REQUIRED)
find_package(Sophus REQUIRED)


ament_auto_find_build_dependencies()

###########
## Build ##
###########

ament_auto_add_library(${PROJECT_NAME} SHARED
  DIRECTORY src
)



list( APPEND CMAKE_MODULE_PATH /home/auto/g2o-master/cmake_modules ) 
set(G2O_ROOT /usr/local/include/g2o) 
find_package(G2O REQUIRED) 
include_directories( 
${G2O_INCLUDE_DIRS}
${OPENVINO_ROOT}/runtime/include
${OPENVINO_ROOT}/runtime/include/openvino
 ) 

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${GLOG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CERES_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${G2O_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENVINO_ROOT}/runtime/lib/intel64/libopenvino.so)

target_include_directories(${PROJECT_NAME} PUBLIC
${OpenCV_INCLUDE_DIRS}
${G2O_INCLUDE_DIRS}
${Sophus_INCLUDE_DIRS}
${EIGEN3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
g2o_core
g2o_stuff
g2o_solver_dense
fmt::fmt
)




rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN rm_auto_aim::ArmorDetectorNode
  EXECUTABLE armor_detector_node
)

#############
## Testing ##
#############

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  list(APPEND AMENT_LINT_AUTO_EXCLUDE
    ament_cmake_copyright
    ament_cmake_uncrustify
    ament_cmake_cpplint
  )
  ament_lint_auto_find_test_dependencies()

  find_package(ament_cmake_gtest)
  ament_add_gtest(test_node_startup test/test_node_startup.cpp)
  target_link_libraries(test_node_startup ${PROJECT_NAME})

  ament_add_gtest(test_number_cls test/test_number_cls.cpp)
  target_link_libraries(test_number_cls ${PROJECT_NAME})

endif()

#############
## Install ##
#############

ament_auto_package(
  INSTALL_TO_SHARE
  model
)
