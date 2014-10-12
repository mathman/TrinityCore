#ifndef __WEBBYTEBUFFER_H__
#define __WEBBYTEBUFFER_H__

#include "Define.h"
#include <vector>
#include "Socket.h"
#include "ByteBuffer.h"

class WebByteBuffer : public ByteBuffer
{
    public:
        WebByteBuffer() : ByteBuffer(READ_BLOCK_SIZE), _opcode(0)
        {
        }

        explicit WebByteBuffer(size_t res) : ByteBuffer(res) { }

        void setOpcode(uint8 opcode) { _opcode = opcode; }
        uint8 getOpcode() { return _opcode; }

    private:
        uint8 _opcode;
};

#endif