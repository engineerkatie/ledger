#
# F E T C H   M L   T E S T S
#
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(fetch-ml)

# CMake configuration
include(${FETCH_ROOT_CMAKE_DIR}/BuildTools.cmake)
include_directories("${FETCH_ROOT_DIR}/libs/math/tests/unit")

# Compiler Configuration
setup_compiler()

fetch_add_test(ml_graph_gtest fetch-ml unit/graph)
fetch_add_test(ml_node_gtest fetch-ml unit/node)
fetch_add_test(ml_ops_gtest fetch-ml unit/ops)
fetch_add_test(ml_optimisation_gtest fetch-ml unit/optimisation)
fetch_add_test(ml_regularisers_gtest fetch-ml unit/regularisers)
fetch_add_test(ml_serializers_gtest fetch-ml unit/serializers)
fetch_add_test(ml_state_gtest fetch-ml unit/state)
fetch_add_test(ml_utilities_gtest fetch-ml unit/utilities)

fetch_add_slow_test(ml_clustering_gtest fetch-ml unit/clustering)
fetch_add_slow_test(ml_dataloader_gtest fetch-ml unit/dataloader)
fetch_add_slow_test(ml_model_gtest fetch-ml unit/model)
fetch_add_slow_test(ml_layers_gtest fetch-ml unit/layers)
fetch_add_slow_test(ml_training_gtest fetch-ml unit/training)
