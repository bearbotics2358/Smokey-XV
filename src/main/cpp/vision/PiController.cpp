#include "vision/PiController.h"

PiController::PiController(MqttClient&& client):
m_client(std::move(client)),
m_data(new PiData) {}

std::optional<PiController> PiController::create(const std::string& address, int port) {
    auto client = MqttClient::create(address, port);
    if (!client.has_value()) {
        return {};
    }

    return std::make_optional<PiController>(PiController(std::move(*client)));
}