#include <pin.H>

#include <iostream>
#include <utility>

#include "client_lib/fp_selectors/normal_floating_point_implementation.h"
#include "client_lib/interfaces/floating_point_implementation.h"
#include "client_lib/registry/register_function_level_fp_selector.h"
#include "client_lib/registry/register_single_fp_implementation_selector.h"

namespace ftrace {

class TestSimpleFpImplementation : public FloatingPointImplementation {
 public:
  /*!
   * A simple implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override {
    return 1.0;
  }
};

class TestComplexFpImplementation : public FloatingPointImplementation {
 public:
  /*!
   * A complex implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override {
    switch (operation) {
      case XED_ICLASS_ADDSS:
        return (operand1 + operand2) * 0.9;
      case XED_ICLASS_SUBSS:
        return (operand1 - operand2) * 0.9;
      case XED_ICLASS_MULSS:
        return (operand1 * operand2) * 0.9;
      case XED_ICLASS_DIVSS:
        return (operand1 / operand2) * 0.9;
      default:
        std::cerr << "Unexpected opcode " << operation
                  << " encountered "
                     "when replacing floating-point instructions"
                  << endl;
        return operand1;
    }
  }
};

static TestSimpleFpImplementation simple;
static TestComplexFpImplementation complex;
static NormalFloatingPointImplementation normal;

static const pair<string, FloatingPointImplementation *>
    test_simple_function_level_map[] = {{"helper2", &simple},
                                        {"helper1", &complex}};
static const int test_simple_function_level_map_size =
    sizeof(test_simple_function_level_map) /
    sizeof(test_simple_function_level_map[0]);
static const pair<string, FloatingPointImplementation *>
    test_nested_function_level_map[] = {{"helper1", &simple},
                                        {"helper2", &simple},
                                        {"nested_helper", &complex}};
static const int test_nested_function_level_map_size =
    sizeof(test_nested_function_level_map) /
    sizeof(test_nested_function_level_map[0]);

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
