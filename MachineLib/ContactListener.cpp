/**
 * @file ContactListener.cpp
 * @author Charles Owen
 * @author Anik Momtaz
 */


#include "pch.h"
#include <b2_contact.h>
#include "ContactListener.h"

/**
 * @brief Triggered when Box2D detects that two fixtures have begun touching.
 *
 * This method checks each body involved in the collision to see
 * if a listener has been registered. If so, the event is forwarded
 * to that specific listener.
 *
 * @param contact The Box2D contact record.
 */
void ContactListener::BeginContact(b2Contact *contact)
{
    b2ContactListener* listener = nullptr;

    if (ShouldDispatch(contact, 1, listener))
    {
        listener->BeginContact(contact);
    }

    if (ShouldDispatch(contact, 2, listener))
    {
        listener->BeginContact(contact);
    }
}

/**
 * @brief Invoked before Box2D processes the contact through its solver.
 *
 * This is commonly used to adjust friction, restitution, or decide
 * whether a collision should be ignored.
 *
 * @param contact The Box2D contact record.
 * @param oldManifold The previous manifold before solving.
 */
void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    b2ContactListener* listener = nullptr;

    if (ShouldDispatch(contact, 1, listener))
    {
        listener->PreSolve(contact, oldManifold);
    }

    if (ShouldDispatch(contact, 2, listener))
    {
        listener->PreSolve(contact, oldManifold);
    }
}

/**
 * @brief Determines if a collision event should be forwarded to a listener.
 *
 * Checks whether either the first or second body in the contact has
 * a registered dispatch listener. If found, the reference is returned
 * and the caller may forward the current Box2D event.
 *
 * @param contact The collision being examined.
 * @param body Selects body A (1) or body B (2).
 * @param listener Output reference that holds the discovered listener.
 * @return true If the body has a registered listener; otherwise false.
 */
bool ContactListener::ShouldDispatch(b2Contact *contact, int body, b2ContactListener* &listener)
{
    b2Body* bodyPtr = (body == 1)
        ? contact->GetFixtureA()->GetBody()
        : contact->GetFixtureB()->GetBody();

    auto lookup = mDispatch.find(bodyPtr);
    if (lookup != mDispatch.end())
    {
        listener = lookup->second;
        return true;
    }

    return false;
}
