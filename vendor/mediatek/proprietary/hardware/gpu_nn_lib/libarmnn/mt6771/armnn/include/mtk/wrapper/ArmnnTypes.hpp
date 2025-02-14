﻿/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>
#include <vector>

namespace armnn_wrapper {

using NetworkId = int;  // Netwotk ID

//
// The Compute enum is now deprecated and it is now
// being replaced by BackendId
//
enum class Compute
{
    Undefined = 0,
    /// CPU Execution: Reference C++ kernels
    CpuRef    = 1,
    /// CPU Execution: NEON: ArmCompute
    CpuAcc    = 2,
    /// GPU Execution: OpenCL: ArmCompute
    GpuAcc    = 3
};

enum class TuningLevel
{
    Rapid = 0,
    Normal = 1,
    Exhaustive = 2
};

constexpr unsigned int MaxNumOfTensorDimensions = 5U;

/// @enum Status enumeration
/// @var Status::Successful
/// @var Status::Failure
enum class Status
{
    Success = 0,
    Failure = 1
};

enum class DataType
{
    Invalid = -1,
    Float16 = 0,
    Float32 = 1,
    QuantisedAsymm8 = 2,
    Signed32 = 3,
    Boolean = 4,
    QuantisedSymm16 = 5
};

enum class DataLayout
{
    NCHW = 1,
    NHWC = 2
};

struct TensorInfo {
    uint64_t uniqueId;
    uint32_t rank;
    std::vector<unsigned int> dim;
    size_t physicalSize;
    DataType datatype;
    float scale = 0;
    int32_t offset = 0;
    const void* data = nullptr;
};

enum class ActivationFunction
{
    Sigmoid     = 0,
    TanH        = 1,
    Linear      = 2,
    ReLu        = 3,
    BoundedReLu = 4, ///< min(a, max(b, input))
    SoftReLu    = 5,
    LeakyReLu   = 6,
    Abs         = 7,
    Sqrt        = 8,
    Square      = 9
};

enum class PoolingAlgorithm
{
    Max     = 0,
    Average = 1,
    L2      = 2
};

enum class ResizeMethod
{
    Bilinear        = 0,
    NearestNeighbor = 1
};

///
/// The padding method modifies the output of pooling layers.
/// In both supported methods, the values are ignored (they are
/// not even zeroes, which would make a difference for max pooling
/// a tensor with negative values). The difference between
/// IgnoreValue and Exclude is that the former counts the padding
/// fields in the divisor of Average and L2 pooling, while
/// Exclude does not.
///
enum class PaddingMethod
{
    /// The padding fields count, but are ignored
    IgnoreValue = 0,
    /// The padding fields don't count and are ignored
    Exclude     = 1
};

enum class NormalizationAlgorithmChannel
{
    Across = 0,
    Within = 1
};

enum class NormalizationAlgorithmMethod
{
    /// Krichevsky 2012: Local Brightness Normalization
    LocalBrightness = 0,
    /// Jarret 2009: Local Contrast Normalization
    LocalContrast = 1
};

enum class OutputShapeRounding
{
    Floor       = 0,
    Ceiling     = 1
};

}  // namespace armnn_wrapper
