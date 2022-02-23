#pragma once

#include <optional>
#include <string>

#include "Prefs.h"
#include "mqtt/mqtt.h"
#include "types.h"

class MqttHandler {
    public:
        // Creates an mqtt handler, but it will not be ready, init must be called later
        MqttHandler() = default;
        ~MqttHandler();
        // may fail, in which case ready will be false
        MqttHandler(const std::string& addr, const u16 port, const std::string& topic);

        // returns true on success, and ready is set to true
        // does nothing if already ready
        bool init(const std::string& addr, const u16 port, const std::string& topic);

        // returns none on failure
        static std::optional<MqttHandler> create(const std::string& address, u16 port, const std::string& topic);

        // call to recieve new mqtt messages
        bool update();

        // send an mqtt message
        bool publish(const std::string& msg, const std::string& topic);

        // is handler ready to send messages
        bool ready() const;
        bool object_present() const;
        float distance() const;
        float angle() const;

    private:
        static int open_nb_socket(const std::string& addr, u16 port);
        static void publish_callback(void **state, struct mqtt_response_publish *published);
        static void reconnect_callback(struct mqtt_client *client, void **state);

        struct mqtt_client m_client;
        int m_sockfd { -1 };

        // is handler ready to send messages
        bool m_ready { false };

        // is object present
        bool m_present;
        float m_distance;
        float m_angle;

        std::string m_addres;
        u16 m_port;
        std::string m_topic;

        u8 m_sendbuf[SEND_BUF_LEN];
        u8 m_recvbuf[RECV_BUF_LEN];
};