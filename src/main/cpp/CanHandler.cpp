#include "CanHandler.h"

#include <string>
#include <utility>
#include <stdio.h>

#include <frc/smartdashboard/SmartDashboard.h>

#include "Prefs.h"

CanHandler::CanHandler(const std::vector<Arduino>& in)
: m_endpoints() {
    for (const auto& arduino : in) {
        std::vector<Field> fields;

        int bitsum = 0;

        for (const auto& field : arduino.fields) {
            u8 desired_bits = field.desired_bits;

            bitsum += desired_bits;

            if (bitsum > 64 || desired_bits > 32) {
                break;
            }

            u32 bitnum = 0;
            for (u8 k = 0; k < desired_bits; k ++) {
                bitnum |= 1 << k;
            }

            Field temp_d {
                field.id,
                desired_bits,
                bitnum,
                field.multiplier,
                0
            };

            fields.push_back(temp_d);
        }

        auto endpoint = Endpoint {
            frc::CAN(arduino.can_id),
            arduino.api_id,
            fields
        };

        m_endpoints.push_back(std::move(endpoint));
    }
}

CanHandler CanHandler::layout2022() {
    std::vector<Arduino> layout;
    Arduino ard;

    ard.can_id = LEFT_ARDUINO_CAN_ID;
    ard.api_id = LEFT_ARDUINO_API_ID;
    ard.fields = std::vector<DataField>();

    ard.fields.push_back(
        DataField {
            FL_SWERVE_DATA_ID, 16, 10.0f
        }
    );

    ard.fields.push_back(
        DataField {
            BL_SWERVE_DATA_ID, 16, 10.0f
        }
    );

    layout.push_back(ard);

    //ard.can_id = RIGHT_ARDUINO_CAN_ID;
    ard.can_id = RIGHT_ARDUINO_CAN_ID;
    ard.api_id = RIGHT_ARDUINO_API_ID;
    ard.fields = std::vector<DataField>();

    ard.fields.push_back(
        DataField {
            FR_SWERVE_DATA_ID, 16, 10.0f
        }
    );

    ard.fields.push_back(
        DataField {
            BR_SWERVE_DATA_ID, 16, 10.0f
        }
    );

    layout.push_back(ard);

    return CanHandler(layout);
}

std::optional<float> CanHandler::getData(int which) const {
    for (const auto& endpoint : m_endpoints) {
        for (const auto& field : endpoint.data) {
            if (field.id == which) {
                return field.data / field.multiplier;
            }
        }
    }
    return {};
}

void CanHandler::update() {
    frc::CANData data;
    for (auto& endpoint : m_endpoints) {
        if (endpoint.can.ReadPacketNew(endpoint.api_id, &data)) {   
            for (usize j = 0; j / 2 < endpoint.data.size(); j += 2) {
                Field& datas = endpoint.data[j];
                datas.data = (data.data[j] << 8) | data.data[j + 1];
            }
        }
    }
}