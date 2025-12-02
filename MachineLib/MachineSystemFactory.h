/**
* @file MachineSystemFactory.h
 *
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * IMachineSystem factory class. Creates IMachineSystem objects
 *
 * You are not allowed to change this class in any way!
 */

#ifndef CANADIANEXPERIENCE_MACHINESYSTEMFACTORY_H
#define CANADIANEXPERIENCE_MACHINESYSTEMFACTORY_H

#include <memory>

class IMachineSystem;

/**
 * IMachineSystem factory class. Creates IMachineSystem objects
 */
class MachineSystemFactory
{
private:
    /// The resources directory
    std::wstring mResourcesDir;

public:
    MachineSystemFactory(std::wstring resourcesDir);

    // Do not change the return type for CreateMachineSystem!
    std::shared_ptr<IMachineSystem> CreateMachineSystem();
};

#endif //CANADIANEXPERIENCE_MACHINESYSTEMFACTORY_H