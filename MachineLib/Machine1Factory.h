/**
* @file Machine1Factory.h
 * @author Anik Momtaz
 *
 * Factory for creating Machine #1
 */

#ifndef CANADIANEXPERIENCE_MACHINE1FACTORY_H
#define CANADIANEXPERIENCE_MACHINE1FACTORY_H

#include <memory>
#include <string>

class Machine;
class Shape;

/**
 * Factory for creating Machine #1
 */
class Machine1Factory {
private:
    /// Path to the resources directory
    std::wstring mResourcesDir;

    /// Path to the images directory
    std::wstring mImagesDir;

    /// The possible domino colors
    enum class DominoColor { Black, Red, Green, Blue };

    /// Height of a domino
    const double DominoHeight = 25;

    /// Width of a domino
    const double DominoWidth = 5;

    /// Height of a blowing pin
    const double BowlingPinHeight = 35;

    /// Width of a blowing pin
    const double BowlingPinWidth = 10;

    void ElevatorAndConveyor(std::shared_ptr<Machine> machine);
    void SiegeContraption(std::shared_ptr<Machine> machine);
    void DominoStack(std::shared_ptr<Machine> machine, wxPoint2DDouble position);
    void BowlingPinStack(std::shared_ptr<Machine> machine, wxPoint2DDouble position);

    std::shared_ptr<Shape> Domino(std::shared_ptr<Machine> machine, wxPoint2DDouble position, double rotation, DominoColor color);
    std::shared_ptr<Shape> BowlingPin(std::shared_ptr<Machine> machine, wxPoint2DDouble position);

public:
    Machine1Factory(std::wstring resourcesDir);

    std::shared_ptr<Machine> Create(int num);
};

#endif //CANADIANEXPERIENCE_MACHINE1FACTORY_H
