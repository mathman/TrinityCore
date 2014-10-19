#include "WebOpcodes.h"
#include "WebSession.h"

/// Correspondence between opcodes and their names
WebOpcodeHandler webOpcodeTable[NUM_MSG_TYPES_WEB] =
{
    /*0x000*/{ "MSG_NULL_ACTION_WEB",               &WebSession::Handle_NULL },
    /*0x001*/{ "SMSG_WEB_NAME_REQUEST",             &WebSession::HandleCheckCoreName },
    /*0x002*/{ "CMSG_WEB_NAME_RESPONSE",            &WebSession::Handle_ClientSide },
};
