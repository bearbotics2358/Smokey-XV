#pragma once

#include <photonlib/PhotonCamera.h>
#include <string>

class Vision {
    public:
        // takes in the mdns name of the camera
        Vision(const std::string& cameraName);
    private:
        photonlib::PhotonCamera camera;
};