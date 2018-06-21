//===- ComputeIRTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Scalar.h>
#include <onnx/common/tensor.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/ATen.h>
#include <ostream>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(ComputeIRTest, attr_test)
{
  FloatAttr alpha;
  ASSERT_EQ(alpha.kind(), Attribute::kFloat);
}

SKYPAT_F(ComputeIRTest, scalar_test)
{
  Scalar a;

  ASSERT_EQ(a.kind(), Value::kUndefined);

  Int32Scalar b;

  ASSERT_EQ(b.kind(), Value::kInt32);
  ASSERT_EQ(b.getValue(), 0);

  b.setValue(10);
  ASSERT_EQ(b.getValue(), 10);

  Int64Scalar c;
  ASSERT_EQ(c.kind(), Value::kInt64);

  // string
  ::onnx::Tensor tensor2;
  StringScalar e(tensor2);
  ASSERT_TRUE(e.empty());
  ASSERT_EQ(e.kind(), Value::kString);
  e.setValue("test");
  ASSERT_TRUE(e.getValue() == "test");
}

SKYPAT_F(ComputeIRTest, tensor_test)
{
  Tensor a;
  ASSERT_EQ(a.kind(), Value::kUndefined);

  Int32Tensor b;
  ASSERT_EQ(b.kind(), Value::kInt32);
  ASSERT_EQ(b.getValues().size(), 0);
}

SKYPAT_F(ComputeIRTest, add_compute_op)
{
  onnc::Module module;
  IRBuilder builder(module);

  builder.CreateComputeGraph("top-level");
  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ASSERT_TRUE(op1->name().equals("Conv"));

  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 1);

  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 2);
  ASSERT_TRUE(op2->name().equals("Relu"));

  ATen* op3 = builder.AddComputeOp<ATen>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 3);
  ASSERT_TRUE(op3->name().equals("ATen"));
  
  module.getComputeGraph("top-level")->erase(*op1);
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 2);

  module.getComputeGraph("top-level")->clear();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 0);
}

SKYPAT_F(ComputeIRTest, add_compute_opnd)
{
  onnc::Module module;
  IRBuilder builder(module);

  builder.CreateComputeGraph("top-level");
  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ASSERT_TRUE(op1->name().equals("Conv"));

  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 1);

  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 2);
  ASSERT_TRUE(op2->name().equals("Relu"));

  ATen* op3 = builder.AddComputeOp<ATen>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 3);
  ASSERT_TRUE(op3->name().equals("ATen"));
  
  ComputeMemOperand* opnd1
      = builder.AddComputeOpnd<ComputeMemOperand>(*op1, *op2, 0x11, 20);

  EXPECT_TRUE(builder.getComputeGraph()->name() == "top-level");
  ASSERT_EQ(opnd1->start(), 0x11);
  ASSERT_EQ(opnd1->length(), 20);
  ASSERT_EQ(builder.getComputeGraph()->getArcSize(), 1);

  module.getComputeGraph("top-level")->clear();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 0);
}
