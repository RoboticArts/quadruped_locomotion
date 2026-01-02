cmake_minimum_required(VERSION 3.8)
project(quadruped_locomotion)

# ----------------------------------------
# Compiler
# ----------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ----------------------------------------
# Set cmake / ROS2 ament
# ----------------------------------------
option(BUILD_WITH_AMENT "Build as ROS 2 package" ON)

if(BUILD_WITH_AMENT)
  message(STATUS "Building in ROS 2 ament mode")
  find_package(ament_cmake REQUIRED)
else()
  message(STATUS "Building in standalone mode")
endif()

# ----------------------------------------
# Dependencies
# ----------------------------------------
find_package(gz-transport REQUIRED)
find_package(gz-msgs REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

find_package(yaml-cpp REQUIRED)

# ----------------------------------------
# Target
# ----------------------------------------

# Create stack
add_executable(quadruped_locomotion
  src/main.cpp
  src/core/system.cpp
  src/settings/loader.cpp
  src/settings/argument_parser.cpp
  src/core/thread_component.cpp
  src/api/server.cpp
  src/control/quadruped_controller.cpp
  src/driver/quadruped_driver.cpp
  src/driver/gazebo_driver.cpp
)

target_link_libraries(quadruped_locomotion
  gz-transport::gz-transport
  gz-msgs::gz-msgs
  ${ZMQ_LIBRARIES}
  yaml-cpp
)

target_include_directories(quadruped_locomotion
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${ZMQ_INCLUDE_DIRS}
)

# Create client library
add_library(quadruped_locomotion_client 
  SHARED
  src/api/client.cpp
)

add_library(quadruped_locomotion::client ALIAS quadruped_locomotion_client)

set_target_properties(quadruped_locomotion_client
  PROPERTIES
    EXPORT_NAME client
)

target_link_libraries(quadruped_locomotion_client
  PUBLIC
    ${ZMQ_LIBRARIES}
)

target_include_directories(quadruped_locomotion_client
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PUBLIC
    ${ZMQ_INCLUDE_DIRS}
)

# Create test 
add_executable(example_1
  examples/example_1.cpp
)

target_link_libraries(example_1
  PRIVATE
    quadruped_locomotion_client
)

# ----------------------------------------
# CMake -specific install
# ----------------------------------------
if(NOT BUILD_WITH_AMENT)

  # Create build/bin/quadruped_locomotion
  set_target_properties(quadruped_locomotion PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )

  # Create build/share/
  file(MAKE_DIRECTORY
    ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}
  )

  # Create symlink from config to build/share/config
  add_custom_target(link_runtime_config ALL
  COMMAND ${CMAKE_COMMAND} -E rm -rf
          ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/config
  COMMAND ${CMAKE_COMMAND} -E create_symlink
          ${CMAKE_SOURCE_DIR}/config
          ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/config
  )

  install(
    TARGETS quadruped_locomotion
    RUNTIME DESTINATION bin
  )

  install(
    FILES
      config/quadruped_drivers.yaml
    DESTINATION
      share/${PROJECT_NAME}/config
  )

endif()

# ----------------------------------------
# ROS 2-specific install
# ----------------------------------------
if(BUILD_WITH_AMENT)

  # Stack
  install(TARGETS quadruped_locomotion
    DESTINATION lib/${PROJECT_NAME}
  )

  install(DIRECTORY launch
    DESTINATION share/${PROJECT_NAME}
  )

  # Library
  install(
    TARGETS quadruped_locomotion_client
    EXPORT quadruped_locomotionTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
  )

  install(
    EXPORT quadruped_locomotionTargets
    NAMESPACE quadruped_locomotion::
    DESTINATION lib/cmake/quadruped_locomotion
  )

  install(
    DIRECTORY include/
    DESTINATION include
  )

  install(DIRECTORY
    launch
    config
    DESTINATION share/${PROJECT_NAME}
  )

  ament_export_targets(quadruped_locomotionTargets)
  ament_export_include_directories(include)

  if(BUILD_TESTING)

    # install(TARGETS zmq_endpoint_test
    #   DESTINATION lib/${PROJECT_NAME}
    # )

  endif()

  ament_package()
endif()
