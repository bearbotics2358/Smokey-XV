#include "vision/photon.h"

TargetTracker::Mode::Mode(InnerMode mode, int pipelineIndex):
m_innerMode(mode),
m_pipelineIndex(pipelineIndex)
{}

TargetTracker::Mode TargetTracker::Mode::target(int pipelineIndex) {
    return Mode(InnerMode::Target, pipelineIndex);
}

TargetTracker::Mode TargetTracker::Mode::ball(int pipelineIndex) {
    return Mode(InnerMode::Ball, pipelineIndex);
}

int TargetTracker::Mode::getPipelineIndex() const {
    return m_pipelineIndex;
}

bool TargetTracker::Mode::isTarget() const {
    return m_innerMode == InnerMode::Target;
}

bool TargetTracker::Mode::isBall() const {
    return m_innerMode == InnerMode::Ball;
}


TargetTracker::TargetTracker(const std::string& cameraName, TargetTracker::Mode mode):
m_camera(cameraName),
m_mode(mode),
m_team(Team::Red)
{}

TargetTracker::TargetTracker(const std::string& cameraName, TargetTracker::Mode mode, Team team):
m_camera(cameraName),
m_mode(mode),
m_team(team)
{}

void TargetTracker::setTeam(Team team) {
    // TODO: maybe switch pipelines if in ball mode
    m_team = team;
}

void TargetTracker::setMode(TargetTracker::Mode mode) {
    m_mode = mode;
    m_camera.SetPipelineIndex(mode.getPipelineIndex());
}

void TargetTracker::update() {
    auto result = m_camera.GetLatestResult();

    if (m_mode.isTarget()) {
        // TODO:
    } else {
        // TODO:
    }
}

std::optional<Target> TargetTracker::getTarget() const {
    return m_target;
}

const std::vector<Target>& TargetTracker::getBalls() const {
    return m_balls;
}