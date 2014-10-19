#ifndef __WEBOPCODES_H__
#define __WEBOPCODES_H__

#include "Common.h"

/// List of Opcodes
enum WebOpcode
{
    MSG_NULL_ACTION_WEB     = 0x00,
    SMSG_WEB_NAME_REQUEST   = 0x01,
    CMSG_WEB_NAME_RESPONSE  = 0x02,
    NUM_MSG_TYPES_WEB       = 0x03
};

class WebSession;

struct WebOpcodeHandler
{
    char const* name;
    void (WebSession::*handler)();
};

extern WebOpcodeHandler webOpcodeTable[NUM_MSG_TYPES_WEB];

/// Lookup opcode name for human understandable logging
inline const char* LookupOpcodeNameWeb(int id)
{
    if (id >= NUM_MSG_TYPES_WEB)
        return "Received unknown opcode, it's more than max!";
    return webOpcodeTable[id].name;
}

inline std::string GetOpcodeNameForLoggingWeb(int opcode)
{
    std::ostringstream ss;
    ss << '[' << LookupOpcodeNameWeb(opcode) << " 0x" << std::hex << std::uppercase << opcode << std::nouppercase << " (" << std::dec << opcode << ")]";
    return ss.str();
}

#endif