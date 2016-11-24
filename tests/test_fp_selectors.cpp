#include <pin.H>

#include <utility>

#include "client_lib/default_fp_selectors/normal_fp_implementation.h"
#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/registry/register_function_level_fp_selector.h"
#include "client_lib/registry/register_single_fp_implementation_selector.h"

namespace ftrace {

/**
 * A simple test implementation of floating-point arithmetic operations.
 */
class TestSimpleFpImplementation : public FpImplementation {
 public:
  FLT32 FpAdd(const FLT32 &operand1, const FLT32 &operand2) override {
    return 1.0;
  }

  FLT32 FpSub(const FLT32 &operand1, const FLT32 &operand2) override {
    return 1.0;
  }

  FLT32 FpMul(const FLT32 &operand1, const FLT32 &operand2) override {
    return 1.0;
  }

  FLT32 FpDiv(const FLT32 &operand1, const FLT32 &operand2) override {
    return 1.0;
  }
};

/**
 * A complex test implementation of floating-point arithmetic operations.
 */
class TestComplexFpImplementation : public FpImplementation {
 public:
  /*!
   * A complex implementation of floating-point arithmetic operations.
   */
  FLT32 FpAdd(const FLT32 &operand1, const FLT32 &operand2) override {
    return (operand1 + operand2) * 0.9;
  }

  FLT32 FpSub(const FLT32 &operand1, const FLT32 &operand2) override {
    return (operand1 - operand2) * 0.9;
  }

  FLT32 FpMul(const FLT32 &operand1, const FLT32 &operand2) override {
    return (operand1 * operand2) * 0.9;
  }

  FLT32 FpDiv(const FLT32 &operand1, const FLT32 &operand2) override {
    return (operand1 / operand2) * 0.9;
  }
};

// FpImplementation instances for tests.
static TestSimpleFpImplementation simple;
static TestComplexFpImplementation complex;
static NormalFpImplementation normal;

static const pair<string, FpImplementation *> test_simple_function_level_map[] =
    {{"helper2", &simple}, {"helper1", &complex}};
static const int test_simple_function_level_map_size =
    sizeof(test_simple_function_level_map) /
    sizeof(test_simple_function_level_map[0]);
static const pair<string, FpImplementation *> test_nested_function_level_map[] =
    {{"helper1", &simple}, {"helper2", &simple}, {"nested_helper", &complex}};
static const int test_nested_function_level_map_size =
    sizeof(test_nested_function_level_map) /
    sizeof(test_nested_function_level_map[0]);

// Register FpSelector instances for tests.
static RegisterSingleFpImplementationSelector<TestSimpleFpImplementation>
    test_simple("test_simple");
static RegisterSingleFpImplementationSelector<TestComplexFpImplementation>
    test_complex("test_complex");
static RegisterFunctionLevelFpSelector test_simple_function_level(
    test_simple_function_level_map, test_simple_function_level_map_size,
    &normal, "test_simple_function_level");
static RegisterFunctionLevelFpSelector test_nested_function_level(
    test_nested_function_level_map, test_nested_function_level_map_size,
    &normal, "test_nested_function_level");

}  // namespace ftrace
