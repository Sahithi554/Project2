/**
 * @file Machine.h
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * A machine that contains components.
 */

#ifndef CANADIANEXPERIENCE_MACHINE_H
#define CANADIANEXPERIENCE_MACHINE_H

#include <vector>
#include <memory>
#include <b2_world.h>

class Component;
class ContactListener;
class b2World;

/**
 * A machine that contains components.
 */
class Machine {
private:
    /// Location of the machine root
    wxPoint mLocation;

    /// Machine number
    int mMachineNum = 1;

    /// Frame rate in frames per second
    double mFrameRate = 30.0;

    /// Current frame
    int mCurrFrame = 0;

    /// Components in this machine
    std::vector<std::shared_ptr<Component>> mComponents;

    /// Physics world for this machine
    std::shared_ptr<b2World> mWorld;

    /// Contact listener for this machine
    std::shared_ptr<ContactListener> mContactListener;

public:
    /**
     * Constructor
     * @param machineNum Machine number
     */
    Machine(int machineNum);

    /// Destructor
    virtual ~Machine() = default;

    /// Copy constructor (disabled)
    Machine(const Machine &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Machine &) = delete;

    /**
     * Add a component to this machine
     * @param component Component to add
     */
    void AddComponent(std::shared_ptr<Component> component);

    /**
     * Set the current frame
     * @param frame Frame number
     */
    void SetFrame(int frame);

    /**
     * Set the machine number
     * @param num Machine number
     */
    void SetNum(int num) { mMachineNum = num; }

    /**
     * Draw the machine
     * @param context Graphics context to draw on
     */
    void Draw(std::shared_ptr<wxGraphicsContext> context);

    /**
     * Update the machine based on elapsed time
     * @param time Elapsed time in seconds
     */
    void Update(double time);

    /**
     * Set the location of the machine
     * @param location Location point
     */
    void SetLocation(wxPoint location) { mLocation = location; }

    /**
     * Get the location of the machine
     * @return Location point
     */
    wxPoint GetLocation() const { return mLocation; }

    /**
     * Get the machine number
     * @return Machine number
     */
    int GetMachineNum() const { return mMachineNum; }

    /**
     * Get the frame rate
     * @return Frame rate
     */
    double GetFrameRate() const { return mFrameRate; }

    /**
     * Set the frame rate
     * @param rate Frame rate
     */
    void SetFrameRate(double rate) { mFrameRate = rate; }

    /**
     * Get the physics world
     * @return Physics world
     */
    std::shared_ptr<b2World> GetWorld() const { return mWorld; }

    /**
     * Get the contact listener
     * @return Contact listener
     */
    std::shared_ptr<ContactListener> GetContactListener() const { return mContactListener; }


    /**
     * Reset the machine to initial state
     */
    void Reset();
};

#endif //CANADIANEXPERIENCE_MACHINE_H

