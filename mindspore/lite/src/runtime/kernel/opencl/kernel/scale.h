/**
 * Copyright 2019 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_SCALE_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_SCALE_H_

#include <vector>
#include "nnacl/scale.h"
#include "src/runtime/opencl/opencl_runtime.h"
#include "src/runtime/kernel/opencl/opencl_kernel.h"

namespace mindspore::kernel {

class ScaleOpenCLKernel : public OpenCLKernel {
 public:
  explicit ScaleOpenCLKernel(OpParameter *parameter, const std::vector<lite::Tensor *> &inputs,
                             const std::vector<lite::Tensor *> &outputs, const lite::InnerContext *ctx)
      : OpenCLKernel(parameter, inputs, outputs) {}
  ~ScaleOpenCLKernel() override;

  int Init() override;
  int Run() override;
  int GetImageSize(size_t idx, std::vector<size_t> *img_size) override;

 private:
  std::vector<size_t> InitGlobalSize() const;
  void Image2dGetWorkGroupSize();
  void BufferGetWorkGroupSize();
  int InitBuffer();

  cl::Kernel kernel_;
  lite::opencl::OpenCLRuntime *ocl_runtime_;
  bool element_flag_{true};
  void *scale_ptr_{nullptr};
  void *offset_ptr_{nullptr};
  int axis_{0};

  std::vector<size_t> local_size_;
  std::vector<size_t> global_size_;
};
}  // namespace mindspore::kernel

#endif  // MINDSPORE_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_SCALE_H_
