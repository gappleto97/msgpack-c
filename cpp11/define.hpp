//
// MessagePack for C++ static resolution routine
//
// Copyright (C) 2008-2013 FURUHASHI Sadayuki and KONDO Takatoshi
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
#ifndef MSGPACK_TYPE_DEFINE_HPP
#define MSGPACK_TYPE_DEFINE_HPP

#define MSGPACK_DEFINE(...) \
	template <typename Packer> \
	void msgpack_pack(Packer& pk) const \
	{ \
		msgpack::type::make_define(__VA_ARGS__).msgpack_pack(pk); \
	} \
	void msgpack_unpack(msgpack::object const& o) \
	{ \
		msgpack::type::make_define(__VA_ARGS__).msgpack_unpack(o); \
	}\
	template <typename MSGPACK_OBJECT> \
	void msgpack_object(MSGPACK_OBJECT* o, msgpack::zone* z) const \
	{ \
		msgpack::type::make_define(__VA_ARGS__).msgpack_object(o, z); \
	}

// MSGPACK_ADD_ENUM must be used in the global namespace.
#define MSGPACK_ADD_ENUM(enum) \
  namespace msgpack { \
	template <> \
	inline enum& operator>> (object const& o, enum& v) \
	{ \
	  int tmp; \
	  o >> tmp; \
	  v = static_cast<enum>(tmp); \
	  return v; \
	} \
	template <> \
	void operator<< (object::with_zone& o, const enum& v) \
	{ \
	  int tmp = static_cast<enum>(v); \
	  o << tmp; \
	} \
  }

namespace msgpack {
namespace type {

template <typename Tuple, std::size_t N>
struct define_imp {
	template <typename Packer>
	static void pack(Packer& pk, Tuple const& t) {
		define_imp<Tuple, N-1>::pack(pk, t);
		pk.pack(std::get<N-1>(t));
	}
	static void unpack(msgpack::object const& o, Tuple& t) {
		define_imp<Tuple, N-1>::unpack(o, t);
		const size_t size = o.via.array.size;
		if(size <= N-1) { return; }
		o.via.array.ptr[N-1].convert(std::get<N-1>(t));
	}
	static void object(msgpack::object* o, msgpack::zone* z, Tuple const& t) {
		define_imp<Tuple, N-1>::object(o, z, t);
		o->via.array.ptr[N-1] = msgpack::object(std::get<N-1>(t), z);
	}
};

template <typename Tuple>
struct define_imp<Tuple, 1> {
	template <typename Packer>
	static void pack(Packer& pk, Tuple const& t) {
		pk.pack(std::get<0>(t));
	}
	static void unpack(msgpack::object const& o, Tuple& t) {
		const size_t size = o.via.array.size;
		if(size <= 0) { return; }
		o.via.array.ptr[0].convert(std::get<0>(t));
	}
	static void object(msgpack::object* o, msgpack::zone* z, Tuple const& t) {
		o->via.array.ptr[0] = msgpack::object(std::get<0>(t), z);
	}
};

template <typename... Args>
struct define {
	typedef define<Args...> value_type;
	typedef tuple<Args...> tuple_type;
	define(Args&... args) :
		a(args...) {}
	template <typename Packer>
	void msgpack_pack(Packer& pk) const
	{
		pk.pack_array(sizeof...(Args));

		define_imp<tuple<Args&...>, sizeof...(Args)>::pack(pk, a);
	}
	void msgpack_unpack(msgpack::object const& o)
	{
		if(o.type != type::ARRAY) { throw type_error(); }

		define_imp<tuple<Args&...>, sizeof...(Args)>::unpack(o, a);
	}
	void msgpack_object(msgpack::object* o, msgpack::zone* z) const
	{
		o->type = type::ARRAY;
		o->via.array.ptr = static_cast<object*>(z->allocate_align(sizeof(object)*sizeof...(Args)));
		o->via.array.size = sizeof...(Args);

		define_imp<tuple<Args&...>, sizeof...(Args)>::object(o, z, a);
	}

	tuple<Args&...> a;
};

template <>
struct define<> {
	typedef define<> value_type;
	typedef tuple<> tuple_type;
	template <typename Packer>
	void msgpack_pack(Packer& pk) const
	{
		pk.pack_array(0);
	}
	void msgpack_unpack(msgpack::object const& o)
	{
		if(o.type != type::ARRAY) { throw type_error(); }
	}
	void msgpack_object(msgpack::object* o, msgpack::zone* z) const
	{
		o->type = type::ARRAY;
		o->via.array.ptr = NULL;
		o->via.array.size = 0;
	}
};

inline define<> make_define()
{
	return define<>();
}

template <typename... Args>
define<Args...> make_define(Args&... args)
{
	return define<Args...>(args...);
}

}  // namespace type
}  // namespace msgpack


#endif /* msgpack/type/define.hpp */
