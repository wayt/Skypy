#include "Opcodes.h"

OpcodeMgr::OpcodeDefinition const* OpcodeMgr::getOpcodeDefinition(uint16 opcode)
{

    static OpcodeDefinition opcodes[] = {
        { SMSG_WELCOME, NULL },
        { CMSG_AUTH, NULL },
        { rMSG_SIP , &Session::handleSipRespond },
        { RMSG_SIP, &Session::handleSipPacket },
        { CMSG_CHAT_TEXT, &Session::handleChatText },
        { CMSG_SEARCH_NEW_CONTACT, &Session::handleSearchNewContact },
        { CMSG_ADD_CONTACT_REQUEST, &Session::handleAddContactRequest },
        { CMSG_ADD_CONTACT_RESPONSE, &Session::handleAddContactResponse },
        { MSG_MAX, NULL }
    };

    for (uint32 i = 0; opcodes[i].opcode != MSG_MAX; ++i)
        if (opcodes[i].opcode == opcode)
            return &opcodes[i];
    return NULL;
}
