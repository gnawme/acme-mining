/// \file   MineTruckStates.h
#pragma once
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>

namespace acme {
class MineTruck;

/// MineTruck states
enum class TruckState { MINING, INBOUND, QUEUED, UNLOADING, OUTBOUND };

/// Enum to string mapping
static std::unordered_map<TruckState, const char*> TRUCK_STATE_NAME{
    {TruckState::MINING, "MINING"},
    {TruckState::INBOUND, "INBOUND"},
    {TruckState::QUEUED, "QUEUED"},
    {TruckState::UNLOADING, "UNLOADING"},
    {TruckState::OUTBOUND, "OUTBOUND"}};

/// \class  MineTruckState
/// \brief  ABC for MineTruck states
class MineTruckState {
public:
    virtual ~MineTruckState() = default;
    virtual void enterState() = 0;
    virtual TruckState getState() const = 0;
    virtual TruckState getNextState() const = 0;
    virtual const char* getStateName() const = 0;
    virtual void outputStatistics(std::ofstream&) = 0;
    virtual void update(const std::string&) = 0;
};

using TruckStateMap = std::unordered_map<TruckState, std::shared_ptr<MineTruckState>>;

/// \class  MineTruckMining
/// \brief  Concrete MINING state
class MineTruckMining : public MineTruckState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineTruckMining(MineTruck& context);
    MineTruckMining() = delete;

    ///
    ~MineTruckMining() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    TruckState getState() const override;

    ///
    TruckState getNextState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineTruck& _context;
    int _duration{0};
    int _timeInState{0};
};

/// \class  MineTruckInbound
/// \brief  Concrete INBOUND state
class MineTruckInbound : public MineTruckState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineTruckInbound(MineTruck& context);
    MineTruckInbound() = delete;

    ///
    ~MineTruckInbound() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    TruckState getState() const override;

    ///
    TruckState getNextState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineTruck& _context;
    int _duration{0};
    int _timeInState{0};
};

/// \class  MineTruckQueued
/// \brief  Concrete QUEUED state
class MineTruckQueued : public MineTruckState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineTruckQueued(MineTruck& context);
    MineTruckQueued() = delete;

    ///
    ~MineTruckQueued() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    TruckState getState() const override;

    ///
    TruckState getNextState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineTruck& _context;
    int _duration{0};
    int _timeInState{0};
};

/// \class  MineTruckUnloading
/// \brief  Concrete UNLOADING state
class MineTruckUnloading : public MineTruckState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineTruckUnloading(MineTruck& context);
    MineTruckUnloading() = delete;

    ///
    ~MineTruckUnloading() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    TruckState getState() const override;

    ///
    TruckState getNextState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineTruck& _context;
    int _duration{0};
    int _timeInState{0};
};

/// \class  MineTruckOutbound
/// \brief  Concrete OUTBOUND state
class MineTruckOutbound : public MineTruckState {
public:
    /// Constructor, gets context by dependency injection
    explicit MineTruckOutbound(MineTruck& context);
    MineTruckOutbound() = delete;

    ///
    ~MineTruckOutbound() override = default;

    /// Sets up conditions when the state is entered
    /// \param duration
    void enterState() override;

    ///
    TruckState getState() const override;

    ///
    TruckState getNextState() const override;

    /// Gets the text of the state name
    const char* getStateName() const override;

    ///
    void outputStatistics(std::ofstream&) override;

    ///
    void update(const std::string&) override;

private:
    MineTruck& _context;
    int _duration{0};
    int _timeInState{0};
};
}  // namespace acme