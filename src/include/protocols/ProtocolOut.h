 /**
 *  ProtocolOut.h
 *
 *  Bridge to work with the message buffer.
 *  All messages go through the ProtocolOut, but not on a straight line in the buffer.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer {

    class ProtocolOut
    {
    public:
        ProtocolOut(const EvResponse &r) :resp(r) {}
        ProtocolOut(EvResponse &&r) :resp(std::move(r)) {}

        const ProtocolOut& setValue(const std::string &val) const
        {
            resp.add_printf("%lu\n%s", val.size(), val.c_str());
            return *this;
        }
        
        const ProtocolOut& setFailValue() const
        {
            resp.add("-1\n", 3);
            return *this;
        }

        template<typename KeyType, typename ValueType>
        const ProtocolOut& setPair(const KeyType &key, const ValueType &val) const
        {
            resp.add(key.data(), key.size())
                .add_printf("\n%lu\n", val.size())
                .add(val.data(), val.size())
                .endl();
            return *this;
        }
        
        template<typename KeyType>
        const ProtocolOut& setFailPair(const KeyType &key) const
        {
            resp.add(key.data(), key.size())
                .add("\n-1\n", 4);
            return *this;
        }

        template<typename ...ValueType>
        const ProtocolOut& setStr(ValueType&&... val) const
        {
            resp.add(std::forward<ValueType>(val)...);
            return *this;
        }

        template<typename ValueType>
        const ProtocolOut& setKey(ValueType &&val) const
        {
            resp.add(std::forward<ValueType>(val)).endl();
            return *this;
        }

        const ProtocolOut& ok() const
        {
            resp.add("OK\n", 3);
            return *this;
        }
        const ProtocolOut& fail() const
        {
            resp.add("FAIL\n", 5);
            return *this;
        }

    private:
        const EvResponse &resp;
    };

}
