/*
 * Copyright (c) 2023, Tim Ledbetter  <timledbetter@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Base64.h>
#include <LibCrypto/Certificate/Certificate.h>
#include <LibTest/TestCase.h>

TEST_CASE(test_private_key_info_decode)
{
    constexpr auto keyder = "MIIBVQIBADANBgkqhkiG9w0BAQEFAASCAT8wggE7AgEAAkEA5HMXMnY+RhEcYXsa"
                            "OyB/YkcrO1nxIeyDCMqwg5MDrSXO8vPXSEb9AZUNMF1jKiFWPoHxZ+foRxrLv4d9"
                            "sV/ETwIDAQABAkBpC37UJkjWQRHyxP83xuasExuO6/mT5sQN692kcppTJ9wHNWoD"
                            "9ZcREk4GGiklu4qx48/fYt8Cv6z6JuQ0ZQExAiEA9XRZVUnCJ2xOcCFCbyIF+d3F"
                            "9Kht5rR77F9KsRlgUbkCIQDuQ7YzLpQ8V8BJwKbDeXw1vQvcPEnyKnTOoALpF6bq"
                            "RwIhAIDSm8Ajgf7m3RQEoLVrCe/l8WtCqsuWliOsr6rbQq4hAiEAx8R16wvOtZlN"
                            "W4jvSU1+WwAaBZl21lfKf8OhLRXrmNkCIG9IRdcSiNR/Ut8QfD3N9Bb1HsUm+Bvz"
                            "c8yGzl89pYST"sv;

    static_assert(keyder.length() == 460u, "keyder length is not 340");

    auto decoded_keyder = TRY_OR_FAIL(decode_base64(keyder));

    Crypto::ASN1::Decoder decoder(decoded_keyder);
    auto private_key_info = TRY_OR_FAIL(Crypto::Certificate::parse_private_key_info(decoder));

    EXPECT_EQ(private_key_info.algorithm.identifier, Crypto::ASN1::rsa_encryption_oid);
    auto& key = private_key_info.rsa;

    EXPECT_EQ(key.length() * 8, 512u);
    EXPECT_EQ(key.public_exponent(), 65537);

    // $ openssl rsa -in testcase.pem -text

    u8 const modulus[] = {
        0x00, 0xe4, 0x73, 0x17, 0x32, 0x76, 0x3e, 0x46, 0x11, 0x1c, 0x61, 0x7b, 0x1a, 0x3b, 0x20,
        0x7f, 0x62, 0x47, 0x2b, 0x3b, 0x59, 0xf1, 0x21, 0xec, 0x83, 0x08, 0xca, 0xb0, 0x83, 0x93,
        0x03, 0xad, 0x25, 0xce, 0xf2, 0xf3, 0xd7, 0x48, 0x46, 0xfd, 0x01, 0x95, 0x0d, 0x30, 0x5d,
        0x63, 0x2a, 0x21, 0x56, 0x3e, 0x81, 0xf1, 0x67, 0xe7, 0xe8, 0x47, 0x1a, 0xcb, 0xbf, 0x87,
        0x7d, 0xb1, 0x5f, 0xc4, 0x4f
    };
    EXPECT_EQ(key.modulus(), Crypto::UnsignedBigInteger(modulus, sizeof(modulus)));

    u8 const private_exponent[] = {
        0x69, 0x0b, 0x7e, 0xd4, 0x26, 0x48, 0xd6, 0x41, 0x11, 0xf2, 0xc4, 0xff, 0x37, 0xc6, 0xe6,
        0xac, 0x13, 0x1b, 0x8e, 0xeb, 0xf9, 0x93, 0xe6, 0xc4, 0x0d, 0xeb, 0xdd, 0xa4, 0x72, 0x9a,
        0x53, 0x27, 0xdc, 0x07, 0x35, 0x6a, 0x03, 0xf5, 0x97, 0x11, 0x12, 0x4e, 0x06, 0x1a, 0x29,
        0x25, 0xbb, 0x8a, 0xb1, 0xe3, 0xcf, 0xdf, 0x62, 0xdf, 0x02, 0xbf, 0xac, 0xfa, 0x26, 0xe4,
        0x34, 0x65, 0x01, 0x31
    };
    EXPECT_EQ(key.private_exponent(), Crypto::UnsignedBigInteger(private_exponent, sizeof(private_exponent)));

    u8 const prime1[] = {
        0x00, 0xf5, 0x74, 0x59, 0x55, 0x49, 0xc2, 0x27, 0x6c, 0x4e, 0x70, 0x21, 0x42, 0x6f, 0x22,
        0x05, 0xf9, 0xdd, 0xc5, 0xf4, 0xa8, 0x6d, 0xe6, 0xb4, 0x7b, 0xec, 0x5f, 0x4a, 0xb1, 0x19,
        0x60, 0x51, 0xb9
    };
    EXPECT_EQ(key.prime1(), Crypto::UnsignedBigInteger(prime1, sizeof(prime1)));

    u8 const prime2[] = {
        0x00, 0xee, 0x43, 0xb6, 0x33, 0x2e, 0x94, 0x3c, 0x57, 0xc0, 0x49, 0xc0, 0xa6, 0xc3, 0x79,
        0x7c, 0x35, 0xbd, 0x0b, 0xdc, 0x3c, 0x49, 0xf2, 0x2a, 0x74, 0xce, 0xa0, 0x02, 0xe9, 0x17,
        0xa6, 0xea, 0x47
    };
    EXPECT_EQ(key.prime2(), Crypto::UnsignedBigInteger(prime2, sizeof(prime2)));

    u8 const exponent1[] = {
        0x00, 0x80, 0xd2, 0x9b, 0xc0, 0x23, 0x81, 0xfe, 0xe6, 0xdd, 0x14, 0x04, 0xa0, 0xb5, 0x6b,
        0x09, 0xef, 0xe5, 0xf1, 0x6b, 0x42, 0xaa, 0xcb, 0x96, 0x96, 0x23, 0xac, 0xaf, 0xaa, 0xdb,
        0x42, 0xae, 0x21
    };
    EXPECT_EQ(key.exponent1(), Crypto::UnsignedBigInteger(exponent1, sizeof(exponent1)));

    u8 const exponent2[] = {
        0x00, 0xc7, 0xc4, 0x75, 0xeb, 0x0b, 0xce, 0xb5, 0x99, 0x4d, 0x5b, 0x88, 0xef, 0x49, 0x4d,
        0x7e, 0x5b, 0x00, 0x1a, 0x05, 0x99, 0x76, 0xd6, 0x57, 0xca, 0x7f, 0xc3, 0xa1, 0x2d, 0x15,
        0xeb, 0x98, 0xd9
    };
    EXPECT_EQ(key.exponent2(), Crypto::UnsignedBigInteger(exponent2, sizeof(exponent2)));

    u8 const coefficient[] = {
        0x6f, 0x48, 0x45, 0xd7, 0x12, 0x88, 0xd4, 0x7f, 0x52, 0xdf, 0x10, 0x7c, 0x3d, 0xcd, 0xf4,
        0x16, 0xf5, 0x1e, 0xc5, 0x26, 0xf8, 0x1b, 0xf3, 0x73, 0xcc, 0x86, 0xce, 0x5f, 0x3d, 0xa5,
        0x84, 0x93
    };
    EXPECT_EQ(key.coefficient(), Crypto::UnsignedBigInteger(coefficient, sizeof(coefficient)));
}
