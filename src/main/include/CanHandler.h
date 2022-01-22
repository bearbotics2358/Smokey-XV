#pragma once

#include <vector>
#include <optional>

#include <frc/CAN.h>

#include "types.h"

struct DataField {
    int id;
    u8 bits;
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
    std::vector<DataField> data;
};

class CanHandler {
    public:
        CanHandler(std::vector<Arduino>& in);
        std::optional<float> getData(int which) const;
        void update();
    private:
        std::vector<frc::CAN> a_cans;
        std::vector<std::vector<Field>> a_fields;
};

std::vector<Arduino> canLayout2022();