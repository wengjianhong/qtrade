# ---------------------------------------------------------------------------
# qtrade_proto：Protobuf / gRPC 生成与静态库
# ---------------------------------------------------------------------------

find_package(PkgConfig REQUIRED)
pkg_check_modules(QTRADE_GRPC REQUIRED grpc++)
pkg_check_modules(QTRADE_PROTOBUF REQUIRED protobuf)

find_program(QTRADE_PROTOC protoc REQUIRED)
find_program(QTRADE_GRPC_PLUGIN grpc_cpp_plugin REQUIRED)

set(QTRADE_PROTO_ROOT ${CMAKE_SOURCE_DIR}/proto)
set(QTRADE_PROTO_GEN_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${QTRADE_PROTO_GEN_DIR})

set(QTRADE_CONFIG_PROTO ${QTRADE_PROTO_ROOT}/qtrade/config/v1/config.proto)
set(QTRADE_CONFIG_PROTO_GEN_DIR ${QTRADE_PROTO_GEN_DIR}/qtrade/config/v1)

set(QTRADE_CONFIG_PROTO_SRCS
    ${QTRADE_CONFIG_PROTO_GEN_DIR}/config.pb.cc
    ${QTRADE_CONFIG_PROTO_GEN_DIR}/config.grpc.pb.cc
)
set(QTRADE_CONFIG_PROTO_HDRS
    ${QTRADE_CONFIG_PROTO_GEN_DIR}/config.pb.h
    ${QTRADE_CONFIG_PROTO_GEN_DIR}/config.grpc.pb.h
)

add_custom_command(
    OUTPUT ${QTRADE_CONFIG_PROTO_SRCS} ${QTRADE_CONFIG_PROTO_HDRS}
    COMMAND ${QTRADE_PROTOC}
        --proto_path=${QTRADE_PROTO_ROOT}
        --cpp_out=${QTRADE_PROTO_GEN_DIR}
        --grpc_out=${QTRADE_PROTO_GEN_DIR}
        --plugin=protoc-gen-grpc=${QTRADE_GRPC_PLUGIN}
        qtrade/config/v1/config.proto
    DEPENDS ${QTRADE_CONFIG_PROTO}
    COMMENT "Generating gRPC sources from config.proto"
    VERBATIM
)

add_library(qtrade_proto STATIC ${QTRADE_CONFIG_PROTO_SRCS})
target_include_directories(qtrade_proto PUBLIC ${QTRADE_PROTO_GEN_DIR})
target_include_directories(qtrade_proto SYSTEM PUBLIC
    ${QTRADE_GRPC_include_DIRS}
    ${QTRADE_PROTOBUF_include_DIRS}
)
target_link_libraries(qtrade_proto PUBLIC
    ${QTRADE_GRPC_LIBRARIES}
    ${QTRADE_PROTOBUF_LIBRARIES}
)
target_compile_options(qtrade_proto PRIVATE ${QTRADE_GRPC_CFLAGS_OTHER})
