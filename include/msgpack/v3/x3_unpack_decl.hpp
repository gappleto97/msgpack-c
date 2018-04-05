//
// MessagePack for C++ deserializing routine
//
// Copyright (C) 2017 KONDO Takatoshi
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MSGPACK_V3_X3_UNPACK_DECL_HPP
#define MSGPACK_V3_X3_UNPACK_DECL_HPP

#if defined(MSGPACK_USE_X3_PARSE)

#include "msgpack/v2/x3_unpack_decl.hpp"

namespace msgpack {

/// @cond
MSGPACK_API_VERSION_NAMESPACE(v3) {
/// @endcond

using v2::unpack;

/// @cond
}  // MSGPACK_API_VERSION_NAMESPACE(v3)
/// @endcond

}  // namespace msgpack

#endif // defined(MSGPACK_USE_X3_PARSE)

#endif // MSGPACK_V3_X3_UNPACK_DECL_HPP
