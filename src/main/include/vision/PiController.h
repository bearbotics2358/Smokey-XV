#pragma once

#include "error.h"
#include "mqtt/MqttClient.h"
#include <memory>
#include <optional>
#include <string_view>
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
        // TODO: update this mode to what the pi will start in
        Mode mode { Mode::None };
        std::vector<Error> errors {};
};

class PiController {
    public:
        ~PiController();

        // create new pi controller, returns none on failure
        static std::optional<PiController> create(const std::string& address, int port);

        void update();

        // get targets recognised by vision
        const std::vector<Ball>& balls() const;
        // get current mode of pi
        Mode mode() const;

        // has the pi sent over an error since clear_error last called
        bool has_error() const;
        // get all errors that have occured since last clear_errors called
        const std::vector<Error>& errors() const;
        // gets the last error that has occured
        std::optional<const Error&> last_error() const;
        // clear any errors that have occured
        void clear_errors();

    private:
        PiController(MqttClient&& client, std::unique_ptr<PiData>&& data);

        // TODO:
        static void data_callback(std::string_view msg, PiData *data);
        // TODO:
        static void error_callback(std::string_view msg, PiData *data);

        MqttClient m_client;
        std::unique_ptr<PiData> m_data;

        // static does not work for these
};