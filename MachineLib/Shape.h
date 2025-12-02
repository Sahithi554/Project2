/**
 * @file Shape.h
 * @author Sahithi Nalamalpu
 * @brief Shape component with strategy pattern for physics behaviors
 */

#ifndef CANADIANEXPERIENCE_SHAPE_H
#define CANADIANEXPERIENCE_SHAPE_H

#include "Component.h"
#include "RotationSource.h"
#include "PhysicsPolygon.h"
#include <memory>
#include <wx/graphics.h>

class Machine;

/**
 * @brief Shape component with behavior strategies
 */
class Shape : public Component {
private:
    /**
     * @brief Physics behavior interface
     */
    class PhysicsBehavior {
    public:
        virtual ~PhysicsBehavior() = default;
        virtual void Apply(cse335::PhysicsPolygon* polygon) = 0;
        virtual void HandleRotation(cse335::PhysicsPolygon* polygon, double rotation) = 0;
        virtual void HandleSpeed(cse335::PhysicsPolygon* polygon, double speed) = 0;
    };

    class StaticBehavior;
    class DynamicBehavior;
    class KinematicBehavior;

    /// Shape's polygon representation
    std::shared_ptr<cse335::PhysicsPolygon> mPolygon;

    /// Rotation source for this shape
    std::unique_ptr<RotationSource> mRotationSource;

    /// Current physics behavior
    std::unique_ptr<PhysicsBehavior> mBehavior;

    /// Current rotation value
    double mRotation = 0.0;

public:
    explicit Shape(Machine* machine);
    virtual ~Shape() = default;

    Shape(const Shape &) = delete;
    void operator=(const Shape &) = delete;

    // Shape creation methods
    void Rectangle(double x, double y, double width, double height);
    void Circle(double radius);
    void BottomCenteredRectangle(double width, double height);
    void AddPoint(double x, double y);
    void AddPoint(wxPoint2DDouble point);

    // Appearance methods
    void SetImage(const std::wstring& path);
    void SetColor(wxColour color);

    // Physics type methods
    void SetDynamic();
    void SetKinematic();

    // Position and rotation
    void SetInitialPosition(double x, double y);
    void SetInitialPosition(wxPoint2DDouble pos);
    void SetInitialRotation(double rotation);
    void SetPhysics(double density, double friction, double restitution);

    // Component interface
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    // Accessors
    std::shared_ptr<cse335::PhysicsPolygon> GetPolygon() const { return mPolygon; }
    RotationSource* GetRotationSource() const { return mRotationSource.get(); }

    // IRotationSink overrides
    void SetRotation(double rotation) override;
    void Rotate(double rotation, double speed) override;
};

#endif // CANADIANEXPERIENCE_SHAPE_H