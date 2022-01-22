#include "CanHandler.h"

#include <string>

#include "Prefs.h"

inline void set_vals(std::vector<DataField>& in, int id, u8 bits, float multiplier) {
    DataField dfield {
        id,
        bits,
        multiplier
    };
    in.push_back(dfield);
}

CanHandler::CanHandler(std::vector<Arduino>& in)
: a_cans(),
a_fields() {
    for (usize i = 0; i < in.size (); i ++) {
        if (i != 0 || in[i].can_id != in[i - 1].can_id) {
            a_cans.push_back(frc::CAN(in[i].can_id));
            a_fields.push_back(std::vector<Field>());

            int bitsum = 0;

            for (usize j = 0; j < in[i].data.size(); j ++) {
                u8 desired_bits = in[i].data[j].bits;

                bitsum += desired_bits;

                if (bitsum > 64 || desired_bits > 32) {
                    break;
                }

                u32 bitnum = 0;
                for (u8 k = 0; k < desired_bits; k ++) {
                    bitnum |= 1 << k;
                }

                Field temp_d {
                    in[i].data[j].id,
                    desired_bits,
                    bitnum,
                    in[i].data[j].multiplier,
                    0
                };

                a_fields[i].push_back(temp_d);
            }
        }
    }
}

std::optional<float> CanHandler::getData(int which) const {
    for (auto field_vec : a_fields) {
        for (auto field : field_vec) {
            if (field.id == which) {
                return field.data / field.multiplier;
            }
        }
    }
}

void CanHandler::update() {
    frc::CANData data;
    for (usize i = 0; i < a_cans.size(); i ++) {
        if (a_cans[i].ReadPacketNew(0, &data)) {   
            /*uint8_t temp;
            temp = data.data[0];
            data.data[0] = data.data[1];
            data.data[1] = temp;
            temp = data.data[2];
            data.data[2] = data.data[3];
            data.data[3] = temp;
            temp = data.data[4];
            data.data[4] = data.data[5];
            data.data[5] = temp;
            temp = data.data[6];
            data.data[6] = data.data[7];
            data.data[7] = temp;
            uint64_t in = *((uint64_t *) data.data);*/
            for (usize j = 0; j / 2 < a_fields[i].size(); j += 2) {
                Field datas = a_fields[i][j];
                /*datas.data = in & datas.bitnum;
                in >>= datas.bits;*/
                datas.data = (data.data[j] << 8) | data.data[j + 1];
            }
        }
    }
}

/*std::vector<Arduino> canMakeIn2020() {
    std::vector<Arduino> can_in;
    Arduino ard;
    DataField dfield;

    ard.can_id = ARDUINO_L_CAN;
    ard.data = std::vector<DataField>();
    set_vals(&ard.data, FL_SWERVE_D, 16, 10.0f);
    set_vals(&ard.data, BL_SWERVE_D, 16, 10.0f);
    set_vals(&ard.data, ARM_D, 16, 10.0f);
    can_in.push_back(ard);

    ard.can_id = ARDUINO_R_CAN;
    ard.data = std::vector<DataField>();
    set_vals(&ard.data, BR_SWERVE_D, 16, 10.0f);
    set_vals(&ard.data, FR_SWERVE_D, 16, 10.0f);
    set_vals(&ard.data, B_TOF_D, 16, 1.0f);
    set_vals(&ard.data, F_TOF_D, 16, 1.0f);
    can_in.push_back(ard);
    return can_in;
}*/
std::vector<Arduino> canLayout2022() {
    
}