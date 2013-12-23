#include "opcodemgr.h"
#include "mainwindow.h"

OpcodeMgr::OpcodeMgr()
{
}


OpcodeMgr::OpcodeDefinition const* OpcodeMgr::getOpcodeDefinition(quint16 opcode)
{
    static OpcodeDefinition opcodes[] = {
        { SMSG_WELCOME, NULL },
        { CMSG_AUTH, NULL },
        { SMSG_AUTH_RESULT, NULL },
        { SMSG_CONTACT_LOGIN, &MainWindow::handleContactLogin },
        { SMSG_CONTACT_LOGOUT, &MainWindow::handleContactLogout },
        { SMSG_CHAT_TEXT, &MainWindow::handleChatText },
        { rMSG_SIP, &MainWindow::handleSipRep},
        { RMSG_SIP, &MainWindow::handleSipRequest},
        { MSG_MAX, NULL}
    };

    for (quint32 i = 0; opcodes[i].opcode != MSG_MAX; ++i)
        if (opcodes[i].opcode == opcode)
            return &opcodes[i];
    return NULL;
}
