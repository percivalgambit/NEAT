#ifndef SHARED_PROGRAM_STATE_H_
#define SHARED_PROGRAM_STATE_H_

#include <string>
#include <vector>

namespace ftrace {

struct ProgramState {
  vector<string> function_stack_;
};

}  // namespace ftrace

#endif  // SHARED_PROGRAM_STATE_H_
