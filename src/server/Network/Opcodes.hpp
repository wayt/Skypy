#ifndef OPCODES_H_
# define OPCODES_H_

#include "SharedDefines.h"
#include "ASession.hpp"
#include <map>

/*enum InternalOpcodes
{
    SMSG_WELCOME        = 1,
    CMSG_AUTH           = 2,
    SMSG_AUTH_RESULT    = 3,
}*/

enum OpcodeHandleMode
{
    OPSTATUS_SYNC_UNAUTHED      = 0,
    OPSTATUS_ASYNC_UNAUTHED     = 1,
    OPSTATUS_SYNC_AUTHED        = 2,
    OPSTATUS_ASYNC_AUTHED       = 3
};

class Opcodes
{
public:
    struct OpcodeDefinition
    {
        uint16 opcode;
        std::function<void(ASession&, Packet&)> func;
        OpcodeHandleMode mode;
    };

    void addOpcode(OpcodeDefinition const& def) { _opcodeMap[def.opcode] = new OpcodeDefinition(def); }

    OpcodeDefinition const* getOpcodeDefinition(uint16 opcode, OpcodeHandleMode mode) const;

private:
    std::map<uint16, OpcodeDefinition*> _opcodeMap;
};

#endif /* !OPCODES_H_ */
