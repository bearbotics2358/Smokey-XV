#pragma once

#include <string>
#include <optional>

#include "types.h"
#include "mqtt.h"
#include "Prefs.h"

class MqttHandler {
    public:
        static std::optional<MqttHandler> create(const std::string& address, const std::string& port, const std::string& topic);
        
        bool update();
        bool has_error() const;

        bool publish(const std::string& msg, const std::string& topic);

        bool object_present() const;
        float distance() const;
        float angle() const;

    private:
        MqttHandler();
        // returns true on success
        bool init(const std::string& addr, const std::string& port, const std::string& topic);

        static int open_nb_socket(const std::string& addr, const std::string& port);
        static void publish_callback(void **state, struct mqtt_response_publish *published);
        static void reconnect_callback(struct mqtt_client *client, void **state);

        struct mqtt_client m_client;
        int m_sockfd;
        bool m_error_flag { false };

        // is object present
        bool m_present;
        float m_distance;
        float m_angle;

        std::string m_addres;
        std::string m_port;
        std::string m_topic;
        u8 m_sendbuf[SEND_BUF_LEN];
        u8 m_recvbuf[RECV_BUF_LEN];
};