/**
 * @file MachineSystem.h
 * @author
 *  - Charles Owen
 *  - Anik Momtaz
 *  - Revised by Sahithi Nalamalpu
 *
 * Declaration of MachineSystem which implements IMachineSystem and
 * provides higher-level management of Machine instances.
 */

#ifndef CANADIANEXPERIENCE_MACHINESYSTEM_IMPL_H
#define CANADIANEXPERIENCE_MACHINESYSTEM_IMPL_H

#include <memory>
#include <vector>
#include <wx/graphics.h>
#include <wx/geometry.h>
#include "IMachineSystem.h"

class Machine;

/**
 * Concrete manager implementing the IMachineSystem interface.
 *
 * Responsibilities include creating machines on demand, routing draw
 * requests, handling frame/time conversion, and distributing frame rate
 * settings to all machines.
 */
class MachineSystem : public IMachineSystem {
private:
    /// Simulation frame rate in frames per second.
    double mFrameRate = 30.0;

    /// Current frame counter (used to compute elapsed time).
    int mCurrentFrame = 0;

    /// Collection of machines indexed by machine number.
    std::vector<std::shared_ptr<Machine>> mMachines;

    /// Root directory for resources (images, etc.).
    std::wstring mResourcesDir;

    /**
     * Retrieve or create the Machine for the supplied index.
     *
     * @param machineNum Index of the machine to fetch.
     * @return Shared pointer to the machine instance.
     */
    std::shared_ptr<Machine> GetOrCreateMachine(int machineNum);

public:
    /**
     * Construct the system providing the resources directory.
     *
     * @param resourcesDir Path to content resources used by machines.
     */
    MachineSystem(std::wstring resourcesDir);

    /// Default destructor.
    virtual ~MachineSystem() = default;

    /// Non-copyable.
    MachineSystem(const MachineSystem &) = delete;

    /// Non-assignable.
    void operator=(const MachineSystem &) = delete;

    // IMachineSystem implementation
    void SetLocation(wxPoint location) override;
    wxPoint GetLocation() override;
    void DrawMachine(std::shared_ptr<wxGraphicsContext> graphics) override;
    void SetMachineFrame(int frame) override;
    void SetFrameRate(double rate) override;
    void ChooseMachine(int machine) override;
    int GetMachineNumber() override;
    double GetMachineTime() override;
    void SetFlag(int flag) override;
};

#endif //CANADIANEXPERIENCE_MACHINESYSTEM_IMPL_H
