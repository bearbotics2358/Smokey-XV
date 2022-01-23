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

// only used internally
struct Field {
    int id;
    u8 bits;
    u32 bitnum;
    float multiplier;
    i32 data;
};

struct Arduino {
    int can_id;
    std::vector<DataField> fields;
};

class CanHandler {
    public:
        CanHandler(std::vector<Arduino>& in);
        static CanHandler layout2022();

        std::optional<float> getData(int field_id) const;
        void update();
    private:
        std::vector<frc::CAN> a_cans;
        std::vector<std::vector<Field>> a_fields;
};