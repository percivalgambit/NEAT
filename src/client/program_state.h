#ifndef CLIENT_PROGRAM_STATE_H_
#define CLIENT_PROGRAM_STATE_H_

#include <stack>
#include <string>

namespace ftrace {

struct ProgramState {
  stack<string> call_stack_;
};

}  // namespace ftrace

#endif  // CLIENT_PROGRAM_STATE_H_
