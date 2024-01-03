
add_library(rcsc_learning_models OBJECT
  onnx_model.cpp
  ball_model.cpp
  )

target_include_directories(rcsc_learning_models
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/onnxruntime/include
  )

target_link_directories(rcsc_learning_models
  PRIVATE
  ${PROJECT_SOURCE_DIR}/onnxruntime/lib
  )

target_link_libraries(rcsc_learning_models onnxruntime)

install(FILES
  onnx_model.h
  ball_model.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rcsc/learning_models
  )
