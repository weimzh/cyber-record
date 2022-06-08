cmake_minimum_required(VERSION 3.10)
project(cyber_record_py)

find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)

pkg_check_modules(PYTHON3 REQUIRED python3)
pkg_check_modules(GLOG REQUIRED libglog)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    cyber/proto/proto_desc.proto
    cyber/proto/scheduler_conf.proto
    cyber/proto/record.proto
    cyber/proto/classic_conf.proto
    cyber/proto/cyber_conf.proto
    cyber/proto/qos_profile.proto
    cyber/proto/choreography_conf.proto
    cyber/proto/component_conf.proto
    cyber/proto/dag_conf.proto
    cyber/proto/transport_conf.proto
    cyber/proto/run_mode_conf.proto
    cyber/proto/perf_conf.proto
)

add_library(_cyber_record_wrapper SHARED
    cyber/binary.cc
    cyber/message/protobuf_factory.cc
    cyber/common/global_data.cc
    cyber/common/file.cc
    cyber/record/record_viewer.cc
    cyber/record/record_reader.cc
    cyber/record/header_builder.cc
    cyber/record/record_writer.cc
    cyber/record/file/record_file_base.cc
    cyber/record/file/record_file_reader.cc
    cyber/record/file/record_file_writer.cc
    cyber/python/internal/py_record.cc
    cyber/time/clock.cc
    cyber/time/duration.cc
    cyber/time/rate.cc
    cyber/time/time.cc
    ${PROTO_SRCS}
)

target_include_directories(_cyber_record_wrapper PRIVATE .
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PYTHON3_INCLUDE_DIRS}
    ${GLOG_INCLUDE_DIRS}
)

set_target_properties(
    _cyber_record_wrapper
    PROPERTIES
    PREFIX ""
    OUTPUT_NAME "_cyber_record_wrapper"
    LINKER_LANGUAGE CXX
)

target_link_libraries(_cyber_record_wrapper ${PROTOBUF_LIBRARY} ${PYTHON3_LIBRARIES} ${GLOG_LIBRARIES})
