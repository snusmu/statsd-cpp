/**
 * StatsD Client
 *
 * Copyright (c) 2012-2014 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

class statsd
{
public:

    /**
     * Open udp/tcp socket
     *
     * @param host The host of StatsD server
	 * @param port The port of StatsD server
	 * @param mode SOCK_DGRAM/SOCK_STREAM for UDP/TCP
	 * return int  0 for success.
	 */
    static int open(const std::string& host, int16_t port = 8125, int mode = SOCK_DGRAM);

    /**
     * Log timing information
     *
     * @param key           The metric to in log timing info for.
     * @param value         The ellapsed time (ms) to log
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void timing(std::string_view key, const int64_t value, const float sample_rate = 1.0f);

    /**
     * Increments one or more stats counters
     *
     * @param key           The metric(s) to increment.
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void increment(std::string_view key, const float sample_rate = 1.0f);

    /**
     * Decrements one or more stats counters.
     *
     * @param key           The metric(s) to decrement.
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void decrement(std::string_view key, const float sample_rate = 1.0f);

    /**
     * Count is the default statsd method for counting
     *
     * @param key           The metric(s) to count
     * @param value         The count value
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void count(std::string_view key, const int64_t value, const float sample_rate = 1.0f);

    /**
     * Gauge
     *
     * @param key           The metric(s) to count
     * @param value         The count value
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void gauge(std::string_view key, const int64_t value, const float sample_rate = 1.0f);

    /**
     * gaugeIncBy
     *
     * @param key           The metric(s) to increment
     * @param value         The increment value
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void gaugeIncBy(std::string_view key, const int64_t value, const float sample_rate = 1.0f);

    /**
     * gaugeDecBy
     *
     * @param key           The metric(s) to decrement
     * @param value         The decrement value
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void gaugeDecBy(std::string_view key, const int64_t value, const float sample_rate = 1.0f);

    /**
     * Set
     *
     * @param key           The metric(s) to count
     * @param value         The count value
     * @param sample_rate   The rate (0-1) for sampling.
     */
    static void set(std::string_view key, const int64_t value, const float sample_rate = 1.0f);


    /**
    * setPrefix
    * 
    * @param prefix         The prefix to prepend
    */
    static void setPrefix(std::string_view _prefix);

    /**
     * setGlobalTags
     * 
     * @param global_tags   The global tags
     */
    static void setGlobalTags(std::vector<std::string> global_tags);

    /**
     * Close socket
     */
    static void close();

    /**
     * Normalize key
     *
     * @param key   The key of metric(s)
     * @return      The normalized key
     */
    static std::string normalize(std::string_view key);

    /**
     * Prepare message
     *
     * @param key           The grahite node
     * @param value         The count value
     * @param sample_rate   The rate (0-1) for sampling.
     * @param unit          The unit value
     * @param sign          The sign (for gauges)
     * @return              The message
     */
    static std::string prepare(
        std::string_view key,
        const int64_t value,
        const std::vector<std::string> tags,
        const float sample_rate,
        std::string_view unit,
        std::string_view sign = std::string_view{}
    );

    /**
     * Get version
     *
     * @return The string of version
     */
    static const char* version();

private:

    struct statsd_t
    {
        struct sockaddr_in server = {};
        int sock = -1;
	    int type = SOCK_DGRAM;
    };

    static statsd_t info;
    static std::string prefix;
    static std::string global_tags_str;

    /**
     * Send
     *
     * @param key           The grahite node
     * @param value         The count value
     * @param sample_rate   The rate (0-1) for sampling.
     * @param unit          The unit value
     * @param sign          The sign (for gauges)
     */
    static void send(
        std::string_view key,
        const int64_t value,
        const float sample_rate,
        std::string_view unit,
        std::string_view sign = std::string_view{}
    );

    /**
     *
     * @param sample_rate   The rate (0-1) for sampling.
     * @return              True if send
     */
    static bool should_send(const float sample_rate);
};
