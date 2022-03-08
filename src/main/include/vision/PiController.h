#pragma once

#include "error.h"
#include "mqtt/MqttClient.h"
#include <memory>
#include <optional>
#include <vector>

enum class Team {
    Red,
    Blue,
};

enum class Mode {
    Vision,
    RemoteViewing,
    None,
};

struct Ball {
        Team team;
        double distance;
        double angle;
};

struct PiData {
        std::vector<Ball> balls {};
        Mode mode { Mode::None };
        std::optional<Error> error {};
};

class PiController {
    public:
        static std::optional<PiController> create(const std::string& address, int port);

    private:
        PiController(MqttClient&& client);

        MqttClient m_client;
        std::unique_ptr<PiData> m_data;
};