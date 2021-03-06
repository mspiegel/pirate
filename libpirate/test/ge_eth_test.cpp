/*
 * This work was authored by Two Six Labs, LLC and is sponsored by a subcontract
 * agreement with Galois, Inc.  This material is based upon work supported by
 * the Defense Advanced Research Projects Agency (DARPA) under Contract No.
 * HR0011-19-C-0103.
 *
 * The Government has unlimited rights to use, modify, reproduce, release,
 * perform, display, or disclose computer software or computer software
 * documentation marked with this legend. Any reproduction of technical data,
 * computer software, or portions thereof marked with this legend must also
 * reproduce this marking.
 *
 * Copyright 2020 Two Six Labs, LLC.  All rights reserved.
 */

#ifdef _WIN32
#include "windows/libpirate.h"
#else
#include "libpirate.h"
#endif
#include "channel_test.hpp"
#include "cross_platform_test.hpp"

extern "C" {
    uint16_t pirate_ge_eth_crc16(const uint8_t *data, uint16_t len);
}

namespace GAPS
{

using ::testing::WithParamInterface;
using ::testing::TestWithParam;
using ::testing::Values;

TEST(ChannelGeEthTest, CRC16X25)
{
    uint8_t data[4] = { 0xDE, 0xAD, 0xBE, 0xEF };
    uint16_t crc = pirate_ge_eth_crc16(&data[0], 4);
    ASSERT_EQ(0xE5CB, crc);
}

TEST(ChannelGeEthTest, ConfigurationParser) {
    int rv;
    pirate_channel_param_t param;
    const pirate_ge_eth_param_t *ge_eth_param = &param.channel.ge_eth;

    char opt[128];
    const char *name = "ge_eth";
    const char *addr1 = "1.2.3.4";
    const short port1 = 0x4242;
#ifdef _WIN32
    const char *addr2 = "0.0.0.0";
    const short port2 = 0;
#else
    const char *addr2 = "5.6.7.8";
    const short port2 = 0x4243;
#endif
    const uint32_t message_id = 0x4745;
    const uint32_t mtu = 42;

    snprintf(opt, sizeof(opt) - 1, "%s", name);
    rv = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_ERROR(EINVAL, WSAEINVAL);
    ASSERT_EQ(-1, rv);
    CROSS_PLATFORM_RESET_ERROR();

    snprintf(opt, sizeof(opt) - 1, "%s,%s", name, addr1);
    rv = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_ERROR(EINVAL, WSAEINVAL);
    ASSERT_EQ(-1, rv);
    CROSS_PLATFORM_RESET_ERROR();

    snprintf(opt, sizeof(opt) - 1, "%s,%s,%d", name, addr1, port1);
    rv = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_ERROR(EINVAL, WSAEINVAL);
    ASSERT_EQ(-1, rv);
    CROSS_PLATFORM_RESET_ERROR();

    snprintf(opt, sizeof(opt) - 1, "%s,%s,%d,%s", name, addr1, port1, addr2);
    rv = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_ERROR(EINVAL, WSAEINVAL);
    ASSERT_EQ(-1, rv);
    CROSS_PLATFORM_RESET_ERROR();

    snprintf(opt, sizeof(opt) - 1, "%s,%s,%d,%s,%d", name, addr1, port1, addr2, port2);
    rv = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_ERROR(EINVAL, WSAEINVAL);
    ASSERT_EQ(-1, rv);
    CROSS_PLATFORM_RESET_ERROR();

    snprintf(opt, sizeof(opt) - 1, "%s,%s,%d,%s,%d,%u", name, addr1, port1, addr2, port2, message_id);
    rv = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_NO_ERROR();
    ASSERT_EQ(0, rv);
    ASSERT_EQ(GE_ETH, param.channel_type);
    ASSERT_STREQ(addr1, ge_eth_param->reader_addr);
    ASSERT_EQ(port1, ge_eth_param->reader_port);
    ASSERT_STREQ(addr2, ge_eth_param->writer_addr);
    ASSERT_EQ(port2, ge_eth_param->writer_port);
    ASSERT_EQ(message_id, ge_eth_param->message_id);
    ASSERT_EQ(0u, ge_eth_param->mtu);

    snprintf(opt, sizeof(opt) - 1, "%s,%s,%d,%s,%d,%u,mtu=%u", name, addr1, port1, addr2, port2, message_id, mtu);
    rv  = pirate_parse_channel_param(opt, &param);
    ASSERT_CROSS_PLATFORM_NO_ERROR();
    ASSERT_EQ(0, rv);
    ASSERT_EQ(GE_ETH, param.channel_type);
    ASSERT_STREQ(addr1, ge_eth_param->reader_addr);
    ASSERT_EQ(port1, ge_eth_param->reader_port);
    ASSERT_STREQ(addr2, ge_eth_param->writer_addr);
    ASSERT_EQ(port2, ge_eth_param->writer_port);
    ASSERT_EQ(message_id, ge_eth_param->message_id);
    ASSERT_EQ(mtu, ge_eth_param->mtu);
}

class GeEthTest : public ChannelTest, public WithParamInterface<unsigned>
{
public:
    void ChannelInit()
    {
        pirate_ge_eth_param_t *param = &Reader.param.channel.ge_eth;

        pirate_init_channel_param(GE_ETH, &Reader.param);
        snprintf(param->reader_addr, sizeof(param->reader_addr) - 1, "127.0.0.1");
        snprintf(param->writer_addr, sizeof(param->writer_addr) - 1, "0.0.0.0");
        param->reader_port = 0x4745;
        param->writer_port = 0;
        param->message_id = 0x5F475243;
        unsigned mtu = GetParam();
        if (mtu) {
            param->mtu = mtu;
        } else {
            mtu = PIRATE_DEFAULT_GE_ETH_MTU;
        }

        Writer.param = Reader.param;
    }

    void TearDown()
    {
        ChannelTest::TearDown();
        ASSERT_EQ(1, nonblocking_IO_attempt);
    }

    static const unsigned TEST_MTU_LEN = PIRATE_DEFAULT_GE_ETH_MTU / 2;
};


TEST_P(GeEthTest, Run)
{
    Run();
}

// Test with IO vector sizes 0 and 16, passed as parameters
INSTANTIATE_TEST_SUITE_P(GeEthFunctionalTest, GeEthTest,
                        Values(0, GeEthTest::TEST_MTU_LEN));

} // namespace
