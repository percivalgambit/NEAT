#include <pin.H>

#include <utility>

#include "client_lib/default_fp_selectors/normal_fp_implementation.h"
#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/registry/register_current_function_fp_selector.h"
#include "client_lib/registry/register_function_stack_fp_selector.h"
#include "client_lib/registry/register_single_fp_implementation_selector.h"
#include "client_lib/utils/fp_operation.h"

namespace ftrace {

/**
 * A simple test implementation of floating-point arithmetic operations.
 */
class TestSimpleFpImplementation : public FpImplementation {
 public:
  FLT32 FpAdd(const FpOperation &operation) override { return 1.0; }

  FLT32 FpSub(const FpOperation &operation) override { return 1.0; }

  FLT32 FpMul(const FpOperation &operation) override { return 1.0; }

  FLT32 FpDiv(const FpOperation &operation) override { return 1.0; }
};

/**
 * A complex test implementation of floating-point arithmetic operations.
 */
class TestComplexFpImplementation : public NormalFpImplementation {
 public:
  /**
   * A complex implementation of floating-point arithmetic operations.
   */
  FLT32 PerformOperation(const FpOperation &operation) {
    return NormalFpImplementation::PerformOperation(operation) * 0.9;
  }
};

// FpImplementation instances for tests.
static TestSimpleFpImplementation simple;
static TestComplexFpImplementation complex;
static NormalFpImplementation normal;

static const pair<string, FpImplementation *> test_simple_function_stack_map[] =
    {{"helper2", &simple}, {"helper1", &complex}};
static const int test_simple_function_stack_map_size =
    sizeof(test_simple_function_stack_map) /
    sizeof(test_simple_function_stack_map[0]);
static const pair<string, FpImplementation *> test_nested_function_stack_map[] =
    {{"helper1", &simple}, {"helper2", &simple}, {"nested_helper", &complex}};
static const int test_nested_function_stack_map_size =
    sizeof(test_nested_function_stack_map) /
    sizeof(test_nested_function_stack_map[0]);

// Register FpSelector instances for tests.
static RegisterSingleFpImplementationSelector<TestSimpleFpImplementation>
    test_simple("test_simple");
static RegisterSingleFpImplementationSelector<TestComplexFpImplementation>
    test_complex("test_complex");
static RegisterFunctionStackFpSelector test_simple_function_stack(
    test_simple_function_stack_map, test_simple_function_stack_map_size,
    &normal, "test_simple_function_stack");
static RegisterFunctionStackFpSelector test_nested_function_stack(
    test_nested_function_stack_map, test_nested_function_stack_map_size,
    &normal, "test_nested_function_stack");
static RegisterCurrentFunctionFpSelector test_simple_current_function(
    test_simple_function_stack_map, test_simple_function_stack_map_size,
    &normal, "test_simple_current_function");
static RegisterCurrentFunctionFpSelector test_nested_current_function(
    test_nested_function_stack_map, test_nested_function_stack_map_size,
    &normal, "test_nested_current_function");

}  // namespace ftrace
