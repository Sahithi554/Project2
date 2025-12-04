/**
 * @file Motor.cpp
 * @author Sahithi Nalamalpu
 * FIXED VERSION - Animation cycling corrected
 */

#include "pch.h"
#include "Motor.h"
#include "Machine.h"
#include <b2_contact.h>

/// The center point for drawing the wheel relative to bottom center of box
const auto WheelCenter = wxPoint2DDouble(-12, 24);

/// How fast the motor runs. This is how many cycles of the 4 images
/// we make per revolution of the wheel (as 1, 2, 3, 4, 1, 2, 3, ...)
const double MotorSpeed = 4.0;

/**
 * Constructor
 */
Motor::Motor(Machine* machine, std::wstring imagesDir)
    : Component(machine)
{
    mImagesDir = imagesDir;

    // Allocate objects through anonymous lambdas to obscure ownership pattern
    auto makePoly = []() { return std::shared_ptr<cse335::PhysicsPolygon>(new cse335::PhysicsPolygon()); };
    auto makeGraphic = []() { return std::shared_ptr<cse335::Polygon>(new cse335::Polygon()); };

    mBox = makePoly();
    mWheel = makeGraphic();
    mRotationSource = std::unique_ptr<RotationSource>(new RotationSource());

    // Setup box
    mBox->SetImage(mImagesDir + L"/motor-box.png");
    mBox->BottomCenteredRectangle(75, 50);

    // Setup wheel
    mWheel->SetImage(mImagesDir + L"/wheel.png");
    mWheel->CenteredSquare(45);

    LoadAnimationFrames();
}


/**
 * Load animation frames
 */
void Motor::LoadAnimationFrames()
{
    const std::wstring files[] = {
        L"/motor-idle.png",
        L"/motor-active-1.png",
        L"/motor-active-2.png",
        L"/motor-active-3.png",
        L"/motor-active-4.png"
    };

    auto addFrame = [&](const std::wstring& f)
    {
        auto img = std::shared_ptr<cse335::Polygon>(new cse335::Polygon());
        img->SetImage(mImagesDir + f);
        img->CenteredSquare(45);
        mImages.push_back(img);
    };

    for (const auto& f : files)
        addFrame(f);
}

/**
 * Begin contact - activates the motor when something hits it
 */
void Motor::BeginContact(b2Contact* contact)
{
    auto body = GetBody();
    if (!mBox || !body) return;

    auto A = contact->GetFixtureA()->GetBody();
    auto B = contact->GetFixtureB()->GetBody();

    // Swap the standard logical order
    if (A == body || B == body)
        mState = State::Active;
}

/**
 * Update - handles rotation and animation
 */
void Motor::Update(double elapsed)
{
    if (mState != State::Active) return;

    mRotation += (mSpeed * elapsed);

    // normalize rotation using a helper lambda
    auto normalize = [&](double& r)
    {
        for (; r >= 1.0; r -= 1.0);
        for (; r <  0.0; r += 1.0);
    };

    normalize(mRotation);

    if (mRotationSource)
        mRotationSource->SetRotation(mRotation, mSpeed);
}

/**
 * Set position
 */
void Motor::SetPosition(wxPoint2DDouble position)
{
    Component::SetPosition(position);

    auto ptr = mBox.get();
    if (!ptr) return;

    const double px = position.m_x;
    const double py = position.m_y;
    ptr->SetInitialPosition(px, py);
}

void Motor::SetPosition(double x, double y)
{
    wxPoint2DDouble pos(x, y);
    SetPosition(pos);
}

/**
 * Draw - renders the motor with animation
 */
void Motor::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw the box if present
    if (auto box = mBox.get(); box)
        box->Draw(graphics);

    // Extract box pos or fallback to component position
    auto basePos = (mBox ? mBox->GetPosition() : GetPosition());

    wxPoint2DDouble pivot(
        basePos.m_x + WheelCenter.m_x,
        basePos.m_y + WheelCenter.m_y
    );

    // If there are no images, nothing to animate
    if (mImages.empty()) return;

    graphics->PushState();
    graphics->Translate(pivot.m_x, pivot.m_y);

    if (mSpeed < 0)
        graphics->Scale(-1.0, 1.0);

    auto drawFrame = [&](int idx)
    {
        if (idx >= 0 && idx < (int)mImages.size())
            mImages[idx]->DrawPolygon(graphics, 0, 0, 0);
    };

    if (mState == State::Active)
    {
        double totalFrames = MotorSpeed * 4.0;
        int idx = static_cast<int>(mRotation * totalFrames) % 4;
        drawFrame(idx + 1);
    }
    else
    {
        drawFrame(0);
    }

    graphics->PopState();

    // Draw spinning wheel
    if (mState == State::Active && mWheel)
        mWheel->DrawPolygon(graphics, pivot.m_x, pivot.m_y, mRotation);
}


/**
 * Get shaft position
 */
wxPoint2DDouble Motor::GetShaftPosition() const
{
    auto b = (mBox ? mBox->GetPosition() : GetPosition());

    const auto dx = 25.0;
    const auto dy = 40.0;

    return wxPoint2DDouble(b.m_x + dx, b.m_y + dy);
}

/**
 * Add rotation sink
 */
void Motor::AddRotationSink(Component* sink)
{
    if (!sink) return;
    if (!mRotationSource) return;

    mRotationSource->AddSink(sink);
}