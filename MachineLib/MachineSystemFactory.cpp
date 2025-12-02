/**
* @file MachineSystemFactory.cpp
 * @author Sahithi Nalamalpu
 * @brief Factory implementation that constructs IMachineSystem instances.
 *
 * This file provides the concrete factory used by the application to
 * produce a MachineSystem configured to load resources from the
 * supplied directory. The implementation may be modified as needed.
 */

#include "pch.h"
#include "MachineSystemFactory.h"
#include "IMachineSystem.h"
#include "MachineSystem.h"

/**
 * @brief Construct a factory with the given resources directory.
 * @param resourcesDir Path where assets (images, etc.) are located.
 */
MachineSystemFactory::MachineSystemFactory(std::wstring resourcesDir) : mResourcesDir(resourcesDir)
{
}

/**
 * @brief Create and return a new IMachineSystem instance.
 *
 * The function returns a pointer to the interface type so the caller is
 * decoupled from the concrete MachineSystem implementation.
 *
 * @return Shared pointer to an IMachineSystem object.
 */
std::shared_ptr<IMachineSystem> MachineSystemFactory::CreateMachineSystem()
{
    return std::make_shared<MachineSystem>(mResourcesDir);
}
