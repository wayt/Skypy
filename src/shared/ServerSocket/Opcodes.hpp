#ifndef OPCODES_H_
# define OPCODES_H_

#include "SharedDefines.h"
#include "ASession.hpp"
#include <map>

enum InternalOpcodes
{
    SMSG_WELCOME        = 1,
    CMSG_AUTH           = 2,
    SMSG_AUTH_RESULT    = 3,
};

class Opcodes
{
public:
    void addOpcode(uint16 opcode, std::function<void(ASession&, Packet&)>& func) { _opcodeMap[opcode] = func; }

    std::function<void(ASession&, Packet&)>& getOpcodeFunction(uint16 opcode);

private:
    std::map<uint16, std::function<void(ASession&, Packet&)>> _opcodeMap;
};

#endif /* !OPCODES_H_ */
