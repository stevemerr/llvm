//==----- device_binary_image.cpp --- SYCL device binary image abstraction -==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <CL/sycl/detail/pi.hpp>

#include <memory>

#include <CL/sycl/detail/device_binary_image.hpp>

using namespace sycl::detail;

DynRTDeviceBinaryImage::DynRTDeviceBinaryImage(
    std::unique_ptr<char[]> &&DataPtr, size_t DataSize, OSModuleHandle M)
    : RTDeviceBinaryImage(M) {
  Data = std::move(DataPtr);
  Bin = new pi_device_binary_struct();
  Bin->Version = PI_DEVICE_BINARY_VERSION;
  Bin->Kind = PI_DEVICE_BINARY_OFFLOAD_KIND_SYCL;
  Bin->DeviceTargetSpec = PI_DEVICE_BINARY_TARGET_UNKNOWN;
  Bin->CompileOptions = "";
  Bin->LinkOptions = "";
  Bin->ManifestStart = nullptr;
  Bin->ManifestEnd = nullptr;
  Bin->BinaryStart = reinterpret_cast<unsigned char *>(Data.get());
  Bin->BinaryEnd = Bin->BinaryStart + DataSize;
  Bin->EntriesBegin = nullptr;
  Bin->EntriesEnd = nullptr;
  Bin->Format = pi::getBinaryImageFormat(Bin->BinaryStart, DataSize);
  init(Bin);
}

DynRTDeviceBinaryImage::~DynRTDeviceBinaryImage() {
  delete Bin;
  Bin = nullptr;
}
