#ifndef GAMESTATE_H
#define GAMESTATE_H

class Game;

/**
 * Abstract interface for a game state representing a distinct mode or screen.
 *
 * Implementations provide lifecycle hooks for initialization, per-frame updates,
 * rendering, and cleanup. Instances are intended to be used polymorphically.
 */
/**
 * Virtual destructor to allow safe deletion through a GameState pointer.
 */
/**
 * Initialize the state with the owning game context.
 * @param gameContext Pointer to the Game instance that owns or manages this state.
 */
/**
 * Advance the state's logic by one step or frame.
 */
/**
 * Render the state's current visual representation.
 */
/**
 * Release resources and perform cleanup when the state is no longer active.
 */
class GameState {
public:
    virtual ~GameState() {}
    virtual void init(Game* gameContext) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void teardown() = 0;
};

#endif // GAMESTATE_H
