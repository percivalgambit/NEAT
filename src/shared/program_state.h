#ifndef SHARED_PROGRAM_STATE_H_
#define SHARED_PROGRAM_STATE_H_

#include <stack>
#include <string>

namespace ftrace {

struct ProgramState {
  stack<string> call_stack_;
};

}  // namespace ftrace

#endif  // SHARED_PROGRAM_STATE_H_
