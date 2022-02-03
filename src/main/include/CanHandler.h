#pragma once

#include <vector>
#include <optional>

#include <frc/CAN.h>

#include "types.h"

// TODO: comment better
struct DataField {
    int id;
    u8 desired_bits;
    float multiplier;
};


struct Arduino {
    u8 can_id;
    u8 api_id;
    std::vector<DataField> fields;
};

// only used internally
struct Field {
    int id;
    u8 bits;
    u32 bitnum;
    float multiplier;
    i32 data;
};

struct Endpoint {
    frc::CAN can;
    u8 api_id;
    std::vector<Field> data;
};

class CanHandler {
    public:
        CanHandler(const std::vector<Arduino>& in);
        static CanHandler layout2022();

        std::optional<float> getData(int field_id) const;
        void update();
    private:
        std::vector<Endpoint> m_endpoints;
};