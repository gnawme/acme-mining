/// \file   MineStationState.h
#pragma once
#include <iosfwd>
#include <memory>
#include <unordered_map>

namespace acme {
class MineStation;

/// MineStation states
enum class StationState { IDLE, READY, UNLOADING };

/// Enum to string mapping
static std::unordered_map<StationState, const char*> STATION_STATE_NAME{
    {StationState::IDLE, "IDLE"},
    {StationState::READY, "READY"},
    {StationState::UNLOADING, "UNLOADING"}};

/// \class  MineStationState
/// \brief  ABC for MineStation states
class MineStationState {
public:
    virtual ~MineStationState() = default;
    virtual void enterState() = 0;
    virtual StationState getNextState() const = 0;
    virtual StationState getState() const = 0;
    virtual const char* getStateName() const = 0;
    virtual void outputStatistics(std::ofstream&) = 0;
    virtual void update(const std::string&) = 0;
};

using StationStateMap = std::unordered_map<StationState, std::shared_ptr<MineStationState>>;

/// \class  MineStationIdle
/// \brief  Concrete IDLE state
class MineStationIdle : public MineStationState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineStationIdle(MineStation& context);
    MineStationIdle() = delete;

    ///
    ~MineStationIdle() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    StationState getNextState() const override;

    ///
    StationState getState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineStation& _context;
    int _duration{0};
    int _timeInState{0};
};

/// \class  MineStationReady
/// \brief  Concrete READY state
class MineStationReady : public MineStationState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineStationReady(MineStation& context);
    MineStationReady() = delete;

    ///
    ~MineStationReady() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    StationState getNextState() const override;

    ///
    StationState getState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineStation& _context;
    int _duration{0};
    int _timeInState{0};
};

/// \class  MineStationUnloading
/// \brief  Concrete UNLOADING state
class MineStationUnloading : public MineStationState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineStationUnloading(MineStation& context);
    MineStationUnloading() = delete;

    ///
    ~MineStationUnloading() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    StationState getNextState() const override;

    ///
    StationState getState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineStation& _context;
    int _duration{0};
    int _timeInState{0};
};
}  // namespace acme