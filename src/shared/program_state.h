#ifndef SHARED_PROGRAM_STATE_H_
#define SHARED_PROGRAM_STATE_H_

#include <string>
#include <vector>

namespace ftrace {

/**
 * Stores information about the state of an instrumented application.
 */
struct ProgramState {
  vector<string> function_stack; //< Stack containing the sequence of nested
                                 //< functions called up to the current
                                 //< function.
};

}  // namespace ftrace

#endif  // SHARED_PROGRAM_STATE_H_
