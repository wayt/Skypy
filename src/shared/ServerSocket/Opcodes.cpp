#include "Opcodes.hpp"
#include <sstream>

std::function<void(ASession&, Packet&)>& Opcodes::getOpcodeFunction(uint16 opcode)
{
    std::map<uint16, std::function<void(ASession&, Packet&)>>::iterator itr = _opcodeMap.find(opcode);
    if (itr == _opcodeMap.end())
    {
        std::stringstream ss;
        ss << "Unkmow opcode: ";
        ss << opcode;
        throw std::runtime_error(ss.str());
    }
    return itr->second;
}
