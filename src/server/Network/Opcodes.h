#ifndef OPCODES_H_
# define OPCODES_H_

#include "SharedDefines.h"
#include "Session.h"
#include "SessionSocket.h"
#include <map>

enum Opcodes
{
    SMSG_WELCOME                = 1,
    CMSG_AUTH                   = 2,
    SMSG_AUTH_RESULT            = 3,
    rMSG_SIP                    = 4,
    RMSG_SIP                    = 5,
    SMSG_CONTACT_LOGIN          = 6,
    SMSG_CONTACT_LOGOUT         = 7,
    CMSG_CHAT_TEXT              = 8,
    SMSG_CHAT_TEXT              = 9,
    CMSG_SEARCH_NEW_CONTACT     = 10,
    SMSG_SEARCH_CONTACT_RESULT  = 11,
    CMSG_ADD_CONTACT_REQUEST    = 12,
    SMSG_ADD_CONTACT_REQUEST    = 13,
    CMSG_ADD_CONTACT_RESPONSE   = 14,
    SMSG_ACCOUNT_INFO           = 15,
    CMSG_GET_ACCOUNT_INFO       = 16,
    CMSG_CHAT_GROUP_ADD_MEMBERS = 17,
    SMSG_CHAT_GROUP_ADD_MEMBER  = 18,
    SMSG_JOIN_CHAT_GROUP        = 19,
    CMSG_GROUP_CHAT_TEXT        = 20,
    SMSG_GROUP_CHAT_TEXT        = 21,
    MSG_MAX
};

class OpcodeMgr
{
public:
    struct OpcodeDefinition
    {
        uint16 opcode;
        void (Session::*func)(Packet&);
    };

    static OpcodeDefinition const* getOpcodeDefinition(uint16 opcode);

};

#endif /* !OPCODES_H_ */
