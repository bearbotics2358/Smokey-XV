#include "MqttHandler.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>

#include <frc/smartdashboard/SmartDashboard.h>


MqttHandler::MqttHandler() {}

MqttHandler::MqttHandler(const std::string& addr, u16 port, const std::string& topic) {
    init(addr, port, topic);
}

bool MqttHandler::init(const std::string& addr, u16 port, const std::string& topic) {
    if (m_ready) {
        return true;
    }
    
    m_sockfd = open_nb_socket(addr, port);
    if (m_sockfd == -1) {
        return false;
    }

    m_addres = addr;
    m_port = port;
    m_topic = topic;

    mqtt_init_reconnect(&m_client, reconnect_callback, this, publish_callback);
    m_client.publish_response_callback_state = (void *) this;

    if (m_client.error != MQTT_OK) {
        return false;
    }
    m_ready = true;
    return true;
}

std::optional<MqttHandler> MqttHandler::create(const std::string& address, u16 port, const std::string& topic) {
    MqttHandler out = MqttHandler();
    if (out.init(address, port, topic)) {
        return out;
    } else {
        return {};
    }
}

bool MqttHandler::update() {
    if (!m_ready) {
        return false;
    }
    
    mqtt_sync(&m_client);
    if (m_client.error != MQTT_OK) {
        return false;
    } else {
    }
    return true;
}

bool MqttHandler::publish(const std::string& msg, const std::string& topic) {
    if (!m_ready) {
        return false;
    }
    
    const char *ctopic = (const char *) topic.c_str();
    void *cmsg = (void *) msg.c_str();

    if (mqtt_publish(&m_client, ctopic, cmsg, msg.length(), MQTT_PUBLISH_RETAIN) != MQTT_OK) {
        return false;
    }
    return true;
}

bool MqttHandler::ready() const {
    return m_ready;
}

bool MqttHandler::object_present() const {
    return m_present;
}

float MqttHandler::distance() const {
    return m_distance;
}

float MqttHandler::angle() const {
    return m_angle;
}

int MqttHandler::open_nb_socket(const std::string& addr, u16 port/*, int timeout_ms*/) {
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); // IPv4, byte stream, non-blocking socket
    if (sockfd == -1) {
        return -1;
    }

    const char *caddr = addr.c_str();

    struct sockaddr_in addrs;
    addrs.sin_family = AF_INET;
    addrs.sin_port = htons(port); // Convert to right endianness
    addrs.sin_addr.s_addr = inet_addr(caddr);

    int ret = connect(sockfd, (struct sockaddr *) &addrs, sizeof(addrs));

    /*struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = timeout_ms * 1000;*/

    if (ret == -1) {
        return -1;
    }
    return sockfd;
}

void MqttHandler::publish_callback(void** state, struct mqtt_response_publish *published) {
    MqttHandler *handler = *((MqttHandler **) state);
    i32 status;
    sscanf((const char *) published->application_message, "%d %f %f", &status, &(handler->m_distance), &(handler->m_angle));
    if (status == 0) {
        handler->m_present = false;
    } else {
        handler->m_present = true;
    }
}

void MqttHandler::reconnect_callback(struct mqtt_client *client, void **state) {
    MqttHandler *handler = *((MqttHandler **) state);

    if (client->error != MQTT_ERROR_INITIAL_RECONNECT) {
        close(client->socketfd);
    }

    int sockfd = MqttHandler::open_nb_socket(handler->m_addres, handler->m_port);
    if (sockfd == -1) {
        return;
    }

    printf ("before reinit");

    mqtt_reinit(client, sockfd, handler->m_sendbuf, SEND_BUF_LEN, handler->m_recvbuf, RECV_BUF_LEN);

    printf ("before reconnect");

    mqtt_connect(client, NULL, NULL, NULL, 0, NULL, NULL, MQTT_CONNECT_CLEAN_SESSION, 400);

    printf ("before subscribe");

    mqtt_subscribe(client, handler->m_topic.c_str(), 0);

    printf ("after subscribe");
}