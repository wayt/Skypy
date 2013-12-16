#include "Opcodes.h"

Opcodes::OpcodeDefinition const* Opcodes::getOpcodeDefinition(uint16 opcode, OpcodeHandleMode mode) const
{
    std::map<uint16, OpcodeDefinition*>::const_iterator itr = _opcodeMap.find(opcode);
    if (itr == _opcodeMap.end())
        return NULL;
    if (itr->second->mode == mode)
        return itr->second;
    return NULL;
}
