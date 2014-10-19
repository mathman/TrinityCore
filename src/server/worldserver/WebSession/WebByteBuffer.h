#ifndef __WEBBYTEBUFFER_H__
#define __WEBBYTEBUFFER_H__

#include "Define.h"
#include <vector>
#include "Socket.h"
#include "ByteBuffer.h"

class WebByteBuffer
{
    public:
        WebByteBuffer() : _rpos(0), _wpos(0)
        {
            _storage.reserve(READ_BLOCK_SIZE);
        }

        explicit WebByteBuffer(size_t res) : _rpos(0), _wpos(0)
        {
            _storage.reserve(res);
        }

        explicit WebByteBuffer(size_t res, int opcode) : _rpos(0), _wpos(0)
        {
            _storage.reserve(res + 4 + 4);
            WriteInt(opcode);
            WriteInt((int)res);
        }

        int getOpcode() { return ReadInt(0); }
        int getLenghtData() { return ReadInt(4); }
        size_t size() const { return _storage.size(); }

        void WriteInt(int32 value)
        {
            // Convert Endian if necessary
            EndianConvert(value);
            append((int8 *)&value, sizeof(value));
        }

        void WriteShort(int16 value)
        {
            // Convert Endian if necessary
            EndianConvert(value);
            append((int8 *)&value, sizeof(value));
        }

        void WriteFloat(float value)
        {
            // Convert Endian if necessary
            EndianConvert(value);
            append((int8 *)&value, sizeof(value));
        }

        void WriteByte(int8 value)
        {
            // Convert Endian if necessary
            EndianConvert(value);
            append((int8 *)&value, sizeof(value));
        }

        void WriteString(std::string value)
        {
            if (size_t len = value.length())
                append((int8 const*)value.c_str(), len);
            append((int8)0);
        }

        template<class T> void append(const T *src, size_t cnt)
        {
            append((const int8 *)src, cnt * sizeof(T));
            ReadInt();
            ReadInt();
        }

        template <typename T> void append(T value)
        {
            EndianConvert(value);
            append((int8 *)&value, sizeof(value));
        }

        void append(const int8 *src, size_t cnt)
        {
            if (!cnt)
                return;

            if (!src)
                return;

            ASSERT(size() < 10000000);

            if (_storage.size() < _wpos + cnt)
                _storage.resize(_wpos + cnt);
            std::memcpy(&_storage[_wpos], src, cnt);
            _wpos += cnt;
        }

        int8 ReadByte()
        {
            int8 value = read<int8>();
            return value;
        }

        int8 ReadByte(int offset)
        {
            int8 value = read<int8>(offset);
            return value;
        }

        int16 ReadShort()
        {
            int16 value = read<int16>();
            return value;
        }

        int16 ReadShort(int offset)
        {
            int16 value = read<int16>(offset);
            return value;
        }

        int32 ReadInt()
        {
            int32 value = read<int32>();
            return value;
        }

        int32 ReadInt(int offset)
        {
            int32 value = read<int32>(offset);
            return value;
        }

        float ReadFloat()
        {
            float value = read<float>();
            return value;
        }

        float ReadFloat(int offset)
        {
            float value = read<float>(offset);
            return value;
        }

        void ReadString(std::string& value)
        {
            value.clear();
            while (rpos() < size())                         // prevent crash at wrong string format in packet
            {
                char c = read<char>();
                if (c == 0)
                    break;
                value += c;
            }
        }

        template <typename T> T read()
        {
            T r = read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        }

        template <typename T> T read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
                return 0;
            T val = *((T const*)&_storage[pos]);

            // Convert Endian if necessary
            EndianConvert(val);
            return val;
        }

        int8* contents()
        {
            if (_storage.empty())
                return 0;
            return _storage.data();
        }

        int8 const* contents() const
        {
            if (_storage.empty())
                return 0;
            return _storage.data();
        }

        size_t rpos() const { return _rpos; }
        size_t wpos() const { return _wpos; }

    protected:
        size_t _rpos, _wpos;
        std::vector<int8> _storage;
};

#endif