#ifndef OPCODEMGR_H
#define OPCODEMGR_H

enum Opcodes
{
    SMSG_WELCOME        = 1,
    CMSG_AUTH           = 2,
    SMSG_AUTH_RESULT    = 3,
};

class OpcodeMgr
{
public:
    OpcodeMgr();
};

#endif // OPCODEMGR_H
