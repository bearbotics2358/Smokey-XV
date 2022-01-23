#include "CanHandler.h"

#include <string>
#include <stdio.h>

#include <frc/smartdashboard/SmartDashboard.h>

#include "Prefs.h"

CanHandler::CanHandler(std::vector<Arduino>& in)
: a_cans(),
a_fields() {
    for (usize i = 0; i < in.size (); i ++) {
        if (i != 0 || in[i].can_id != in[i - 1].can_id) {
            a_cans.push_back(frc::CAN(in[i].can_id));
            a_fields.push_back(std::vector<Field>());

            int bitsum = 0;

            for (usize j = 0; j < in[i].fields.size(); j ++) {
                u8 desired_bits = in[i].fields[j].desired_bits;

                bitsum += desired_bits;

                if (bitsum > 64 || desired_bits > 32) {
                    break;
                }

                u32 bitnum = 0;
                for (u8 k = 0; k < desired_bits; k ++) {
                    bitnum |= 1 << k;
                }

                Field temp_d {
                    in[i].fields[j].id,
                    desired_bits,
                    bitnum,
                    in[i].fields[j].multiplier,
                    0
                };

                a_fields[i].push_back(temp_d);
            }
        }
    }
}

CanHandler CanHandler::layout2022() {
    std::vector<Arduino> layout;
    Arduino ard;

    ard.can_id = LEFT_ARDUINO_CAN_ID;
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
    ard.can_id = 1;
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
    for (auto field_vec : a_fields) {
        for (auto field : field_vec) {
            if (field.id == which) {
                return field.data / field.multiplier;
            }
        }
    }
    return {};
}

void CanHandler::update() {
    frc::CANData data;
    for (usize i = 0; i < a_cans.size(); i ++) {
        if (a_cans[i].ReadPacketNew(3, &data)) {   
            for (usize j = 0; j / 2 < a_fields[i].size(); j += 2) {
                Field datas = a_fields[i][j];
                datas.data = (data.data[j] << 8) | data.data[j + 1];
            }
        }
    }
}