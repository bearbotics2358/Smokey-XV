#include "vision/PiController.h"

/*static const std::string data_topic { "pi/cv/data" };
static const std::string control_topic { "pi/cv/control" };
static const std::string error_topic { "pi/cv/error" };

PiController::PiController(MqttClient&& client, std::unique_ptr<PiData>&& data):
m_client(std::move(client)),
m_data(std::move(data)) {}

std::optional<PiController> PiController::create(const std::string& address, int port) {
    auto client_opt = MqttClient::create(address, port);
    if (!client_opt.has_value()) {
        return {};
    }

    MqttClient client = std::move(client_opt.value());

    std::unique_ptr<PiData> data(new PiData);

    auto res1 = client.subscribe(data_topic, data_callback, data.get());
    auto res2 = client.subscribe(error_topic, error_callback, data.get());

    if (res1.is_err() || res2.is_err()) {
        return {};
    }

    return std::make_optional<PiController>(PiController(std::move(client), std::move(data)));
}

void PiController::update() {
    auto result = m_client.update();
    if (result.is_err()) {
        m_data->errors.push_back(result);
    }
}

const std::vector<Ball>& PiController::balls() const {
    return m_data->balls;
}

Mode PiController::mode() const {
    return m_data->mode;
}

bool PiController::has_error() const {
    return !m_data->errors.empty();
}

const std::vector<Error>& PiController::errors() const {
    return m_data->errors;
}

std::optional<const Error&> PiController::last_error() const {
    if (m_data->errors.empty()) {
        return {};
    } else {
        return m_data->errors.back();
    }
}

void PiController::clear_errors() {
    // this should call the destructor of error
    m_data->errors.clear();
}

void PiController::data_callback(std::string_view msg, PiData *data) {
    // TODO: complete once vision is complete on pi
}

void PiController::error_callback(std::string_view msg, PiData *data) {
}*/