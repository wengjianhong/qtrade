# ---------------------------------------------------------------------------
# qtrade_proto：Protobuf / gRPC 代码生成与静态库
#
# 流程（configure 期生成）：
#   1. GLOB 扫描 proto/ 下全部 .proto
#   2. cmake .. 时 execute_process 运行 protoc，输出到 build/proto/
#   3. GLOB 扫描 build/proto/ 下全部 .pb.cc / .grpc.pb.cc
#   4. 编译静态库 qtrade_proto
#
# proto 变更时 CONFIGURE_DEPENDS 触发重新 cmake，自动重跑 protoc 与 GLOB
# 新增 proto：在 proto/ 按 package 路径放置 .proto 即可，无需改本文件
# ---------------------------------------------------------------------------

# --- 依赖探测（pkg-config）---
find_package(PkgConfig REQUIRED)
pkg_check_modules(QTRADE_GRPC REQUIRED grpc++)       # gRPC C++ 客户端/服务端
pkg_check_modules(QTRADE_PROTOBUF REQUIRED protobuf) # Protobuf 运行时

# --- 代码生成工具 ---
find_program(QTRADE_PROTOC protoc REQUIRED)                # protoc 编译器
find_program(QTRADE_GRPC_PLUGIN grpc_cpp_plugin REQUIRED)  # gRPC C++ 插件

# --- 路径 ---
set(QTRADE_PROTO_ROOT ${CMAKE_SOURCE_DIR}/proto)    # .proto 源文件根目录
set(QTRADE_PROTO_GEN_DIR ${CMAKE_BINARY_DIR}/proto) # 生成代码输出目录（不入库）
file(MAKE_DIRECTORY ${QTRADE_PROTO_GEN_DIR})        # 预先创建输出根目录（不存在则创建，已存在则忽略）

# --- 1. 扫描 proto/ 下全部 .proto（CONFIGURE_DEPENDS：新增/修改 .proto 触发重新配置）---
file(GLOB_RECURSE QTRADE_PROTO_FILES CONFIGURE_DEPENDS "${QTRADE_PROTO_ROOT}/*.proto")

# --- 静态库目标 ---
if(QTRADE_PROTO_FILES)
  # protoc 入参须为相对 proto/ 的路径
  set(QTRADE_PROTO_REL_FILES "")
  foreach(_proto ${QTRADE_PROTO_FILES})
    file(RELATIVE_PATH _rel ${QTRADE_PROTO_ROOT} ${_proto})
    list(APPEND QTRADE_PROTO_REL_FILES ${_rel})
  endforeach()

  # --- 2. configure 阶段运行 protoc（保证后续 GLOB 能搜到产物）---
  execute_process(
      COMMAND ${QTRADE_PROTOC}
          --proto_path=.
          --cpp_out=${QTRADE_PROTO_GEN_DIR}
          --grpc_out=${QTRADE_PROTO_GEN_DIR}
          --plugin=protoc-gen-grpc=${QTRADE_GRPC_PLUGIN}
          ${QTRADE_PROTO_REL_FILES}
      WORKING_DIRECTORY ${QTRADE_PROTO_ROOT}
      RESULT_VARIABLE _qtrade_protoc_code)
  if(NOT _qtrade_protoc_code EQUAL 0)
    message(FATAL_ERROR "protoc failed with exit code ${_qtrade_protoc_code}")
  endif()

  # --- 3. GLOB 收集 build/proto/ 下全部编译单元（含 service 才有 .grpc.pb.cc）---
  file(GLOB_RECURSE QTRADE_PROTO_SRCS CONFIGURE_DEPENDS
      "${QTRADE_PROTO_GEN_DIR}/*.pb.cc"
      "${QTRADE_PROTO_GEN_DIR}/*.grpc.pb.cc")

  # --- 4. 编译静态库 ---
  add_library(qtrade_proto STATIC ${QTRADE_PROTO_SRCS})
  target_compile_options(qtrade_proto PRIVATE ${QTRADE_GRPC_CFLAGS_OTHER}) # 如 -DNOMINMAX
else()
  message(WARNING "No .proto files under ${QTRADE_PROTO_ROOT}; qtrade_proto is an empty INTERFACE target")
  add_library(qtrade_proto INTERFACE)
endif()

# Generated headers: #include <qtrade/config/v1/config.pb.h>
target_include_directories(qtrade_proto PUBLIC
    $<BUILD_INTERFACE:${QTRADE_PROTO_GEN_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# gRPC / Protobuf 系统头文件（标记 SYSTEM 抑制第三方告警）
target_include_directories(qtrade_proto SYSTEM PUBLIC
    ${QTRADE_GRPC_INCLUDE_DIRS}
    ${QTRADE_PROTOBUF_INCLUDE_DIRS})

# 链接 grpc++ 与 protobuf；PUBLIC 使 qtrade_common / qtrade_core 自动传递
target_link_libraries(qtrade_proto PUBLIC
    ${QTRADE_GRPC_LIBRARIES}
    ${QTRADE_PROTOBUF_LIBRARIES})
