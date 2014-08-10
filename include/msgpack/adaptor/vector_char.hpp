//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2014 KONDO Takatoshi
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
#ifndef MSGPACK_TYPE_VECTOR_CHAR_HPP
#define MSGPACK_TYPE_VECTOR_CHAR_HPP

#include "msgpack/object.hpp"
#include <vector>

namespace msgpack {

inline std::vector<char>& operator>> (object const& o, std::vector<char>& v)
{
    switch (o.type) {
    case type::BIN:
        v.resize(o.via.bin.size);
        std::memcpy(v.data(), o.via.bin.ptr, o.via.bin.size);
        break;
    case type::STR:
        v.resize(o.via.str.size);
        std::memcpy(v.data(), o.via.str.ptr, o.via.str.size);
        break;
    default:
        throw type_error();
        break;
    }
    return v;
}

template <typename Stream>
inline packer<Stream>& operator<< (packer<Stream>& o, const std::vector<char>& v)
{
    o.pack_bin(v.size());
    o.pack_bin_body(v.data(), v.size());

    return o;
}

inline void operator<< (object& o, const std::vector<char>& v)
{
    o.type = type::BIN;
    o.via.bin.ptr = v.data();
    o.via.bin.size = static_cast<uint32_t>(v.size());
}

inline void operator<< (object::with_zone& o, const std::vector<char>& v)
{
    o.type = type::BIN;
    char* ptr = static_cast<char*>(o.zone->allocate_align(v.size()));
    o.via.bin.ptr = ptr;
    o.via.bin.size = static_cast<uint32_t>(v.size());
    std::memcpy(ptr, v.data(), v.size());
}

}  // namespace msgpack

#endif // MSGPACK_TYPE_VECTOR_CHAR_HPP
