/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_GRAD_ARITHMETIC_SELF_GRAD_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_GRAD_ARITHMETIC_SELF_GRAD_H_

#include <vector>
#include "src/lite_kernel.h"
#include "schema/model_generated.h"
#include "ir/anf.h"

namespace mindspore::kernel {

class ArithmeticSelfGradCPUKernel : public LiteKernel {
  typedef int (*ArithmeticSelfGradOperation)(float *, float *, float *, int);

 public:
  ArithmeticSelfGradCPUKernel(OpParameter *parameter, const std::vector<lite::Tensor *> &inputs,
                              const std::vector<lite::Tensor *> &outputs, const lite::InnerContext *ctx,
                              const mindspore::lite::PrimitiveC *primitive)
      : LiteKernel(parameter, inputs, outputs, ctx, primitive) {}
  ~ArithmeticSelfGradCPUKernel() override {}
  int Init() override;
  int ReSize() override;
  int Run() override;
  int DoArithmeticSelfGrad(int thread_id);

 private:
  int thread_stride_;
  ArithmeticSelfGradOperation self_grad_operation_;
};
}  // namespace mindspore::kernel

#endif  // MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_GRAD_ARITHMETIC_SELF_GRAD_H_
