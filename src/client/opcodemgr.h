#ifndef OPCODEMGR_H
#define OPCODEMGR_H

#include "packet.hpp"

enum Opcodes
{
    SMSG_WELCOME        = 1,
    CMSG_AUTH           = 2,
    SMSG_AUTH_RESULT    = 3,
    CMSG_SIP            = 4,
    SMSG_SIP            = 5,
    SMSG_CONTACT_LOGIN  = 6,
    SMSG_CONTACT_LOGOUT = 7,
    MSG_MAX
};

class MainWindow;

class OpcodeMgr
{
public:
    OpcodeMgr();

    struct OpcodeDefinition
    {
        quint16 opcode;
        void (MainWindow::*func)(Packet&);
    };

    static OpcodeDefinition const* getOpcodeDefinition(quint16 opcode);
};

#endif // OPCODEMGR_H
