/*
 * Copyright (c) 2015 Spotify AB
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include <string>

#include <boost/test/unit_test.hpp>

#include <spotify/json/codec/object.hpp>
#include <spotify/json/codec/string.hpp>
#include <spotify/json/encode_decode.hpp>
#include <spotify/json/extension/boost.hpp>

#include "only_true.hpp"

BOOST_AUTO_TEST_SUITE(spotify)
BOOST_AUTO_TEST_SUITE(json)

namespace {

class base_class {
 public:
  virtual ~base_class() = default;
};

class sub_class : public base_class {
};

codec::object<sub_class> sub_codec() {
  codec::object<sub_class> codec;
  return codec;
}

}  // namespace


/// boost::shared_ptr

BOOST_AUTO_TEST_CASE(json_codec_boost_shared_ptr_should_decode) {
  const auto obj = decode<boost::shared_ptr<std::string>>("\"hello\"");
  BOOST_REQUIRE(obj);
  BOOST_CHECK_EQUAL(*obj, "hello");
}

BOOST_AUTO_TEST_CASE(json_codec_boost_cast_pointer_should_construct_with_helper) {
  const boost::shared_ptr<base_class> ptr = boost::make_shared<sub_class>();
  const auto codec = codec::cast<boost::shared_ptr<base_class>>(boost_shared_ptr(sub_codec()));
  BOOST_CHECK_EQUAL(encode(codec, ptr), "{}");
}


/// boost::optional

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_construct) {
  const auto c = codec::optional_t<codec::string_t>(codec::string());
  static_cast<void>(c);
}

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_construct_with_helper) {
  const auto c = codec::optional((codec::string()));
  static_cast<void>(c);
}

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_construct_with_default_codec) {
  default_codec<boost::optional<std::string>>();
}

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_encode) {
  const auto codec = default_codec<boost::optional<std::string>>();
  BOOST_CHECK_EQUAL(encode(codec, boost::make_optional(std::string("hi"))), "\"hi\"");
}

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_encode_none) {
  const auto codec = default_codec<boost::optional<std::string>>();
  BOOST_CHECK_EQUAL(encode(codec, boost::none), "");
}

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_decode) {
  const auto codec = default_codec<boost::optional<std::string>>();
  BOOST_CHECK(decode(codec, "\"hi\"") == boost::make_optional(std::string("hi")));

  boost::optional<std::string> value;
  BOOST_CHECK(!try_decode(value, codec, "\"hi"));
  BOOST_CHECK(!try_decode(value, codec, ""));
  BOOST_CHECK(!try_decode(value, codec, "5"));
}

BOOST_AUTO_TEST_CASE(json_codec_boost_optional_should_implement_should_encode) {
  const auto codec = default_codec<boost::optional<std::string>>();
  BOOST_CHECK(codec.should_encode(boost::make_optional(std::string(""))));
  BOOST_CHECK(!codec.should_encode(boost::none));
}

BOOST_AUTO_TEST_SUITE_END()  // json
BOOST_AUTO_TEST_SUITE_END()  // spotify