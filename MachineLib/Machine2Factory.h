/**
* @file Machine2Factory.h
 * @author Sahithi Nalamalpu
 *
 * Factory for creating Machine #2 (simpler demonstration machine)
 */

#ifndef CANADIANEXPERIENCE_MACHINE2FACTORY_H
#define CANADIANEXPERIENCE_MACHINE2FACTORY_H


#include <memory>
#include <string>

class Machine;

/**
 * Factory for creating Machine #2
 * This is a simpler machine for demonstration
 */
class Machine2Factory {
private:
    /// Path to the resources directory
    std::wstring mResourcesDir;

    /// Path to the images directory
    std::wstring mImagesDir;

public:
    Machine2Factory(std::wstring resourcesDir);
    std::shared_ptr<Machine> Create(int num);
};

#endif //CANADIANEXPERIENCE_MACHINE2FACTORY_H