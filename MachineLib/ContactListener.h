/**
 * @file ContactListener.h
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * A contact filter allows for testing for things
 * that should happen based on different contacts.
 */



#ifndef CANADIANEXPERIENCE_MACHINELIB_CONTACTLISTENER_H
#define CANADIANEXPERIENCE_MACHINELIB_CONTACTLISTENER_H

#include <map>
#include <b2_world_callbacks.h>

/**
 * @brief Provides selective dispatching of Box2D collision events.
 *
 * Instead of handling every contact globally, this listener allows
 * per-body registration. A body may be mapped to its own
 * b2ContactListener implementation, and only those mapped bodies
 * receive events.
 */
class ContactListener : public b2ContactListener
{
private:
    /**
     * @brief Stores which listener is responsible for each body.
     *
     * When a collision is detected, the bodies in that contact
     * are checked against this map to determine if forwarding
     * should occur.
     */
    std::map<b2Body*, b2ContactListener*> mDispatch;

    bool ShouldDispatch(b2Contact *contact, int body, b2ContactListener* &listener);

public:
    /**
     * @brief Register a contact listener for a specific body.
     *
     * If the body already has a listener, it will be overwritten.
     *
     * @param body The Box2D body that should trigger the listener.
     * @param listener The listener instance that should receive events.
     */
    void Add(b2Body* body, b2ContactListener* listener) { mDispatch[body] = listener; }

    /// @brief Forwards the BeginContact event if a listener is registered.
    void BeginContact(b2Contact* contact) override;

    /**
     * @brief Called when two fixtures stop touching.
     *
     * Currently unused but provided for completeness.
     *
     * @param contact The contact that ended.
     */
    void EndContact(b2Contact* contact) override
    { /* Optional: implement if needed */ }

    /// @brief Forwards PreSolve if a listener is registered.
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

    /**
     * @brief Post-collision processing hook.
     *
     * Provided so components can react after impulses are applied.
     *
     * @param contact Contact that was solved.
     * @param impulse The impulse applied during resolution.
     */
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
    { /* Optional: implement if needed */ }
};

#endif // CANADIANEXPERIENCE_MACHINELIB_CONTACTLISTENER_H
