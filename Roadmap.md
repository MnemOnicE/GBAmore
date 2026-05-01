# GBAmore Engine Architectural Roadmap

## Phase 5: The Automap Data Structure
**Objective:** Implement a Metroidvania-style exploration tracker that records the player's movement through the Hub World and displays it in a dedicated UI state.

*   **Data Architecture:** 
    *   Initialize a 2D array representing the global grid of the Hub World. 
    *   *Optimization Check:* Given the GBA's strict memory limits (256KB EWRAM), implement this as a bitfield or boolean array rather than an array of integers to minimize the footprint.
*   **Logic Integration:** 
    *   Update the array dynamically based on the `player_x` and `player_y` coordinates transforming across room boundaries or screen transitions.
*   **UI Implementation:** 
    *   Create a new `GameState` (e.g., `State_Map`) accessible via the START or SELECT button from the active gameplay loop.
    *   Render the array to the screen using primitive tile drawing or dynamic TTE text symbols to indicate explored rooms vs. uncharted territory.

## Phase 6.5: Cartridge SRAM (The Save Engine)
**Objective:** Establish persistent memory so the player's progress (agates, map data, unlocked abilities) survives a console power cycle.

*   **Toolchain Validation:** Ensure the linker is properly assigning the SRAM section in the compiled `.gba` header so emulators and flash carts allocate the 32KB/64KB save file.
*   **Data Serialization:** 
    *   Expand the `PlayerProfile` struct to encapsulate all necessary save variables (Agate count, Dash unlock boolean, Automap bitfield).
*   **Hardware Interface:** 
    *   Utilize `libtonc`'s `sram_write()` and `sram_read()` functions. 
    *   Implement a checksum validation upon reading to ensure the save data isn't corrupted.
*   **Game Loop Integration:** 
    *   Tie the `sram_write()` execution to a physical interaction in the game world, specifically interacting with the "Bed" asset inside the Nest.

## Phase 7.5: The Audio Engine (Maxmod)
**Objective:** Integrate the Maxmod library to handle background music (BGM) and sound effects (SFX) without stalling the CPU.

*   **Makefile Modification:** 
    *   Introduce the Maxmod audio processing rules to the `Makefile`. 
    *   Configure the pipeline to convert audio assets placed in an `audio/` directory into a compiled `soundbank.bin`.
*   **Asset Pipeline:** 
    *   **BGM:** Utilize tracker formats (`.mod`, `.it`, or `.s3m`) for the lo-fi boom-bap loops. Tracker formats are hardware-efficient and save massive amounts of ROM space compared to raw audio.
    *   **SFX:** Utilize standard `.wav` files for short UI interactions (menu blips, agate collection chimes).
*   **C++ Implementation:** 
    *   Initialize the engine in `main.cpp` via `mmInitDefault()`.
    *   Implement `mmStart()` for continuous tracker playback on level initialization, and `mmEffect()` for asynchronous sound effects triggered by player input.

## Phase 8: Advanced Graphics & Entity Management
**Objective:** Transition from a single shadowed OAM buffer testing a static sprite to a dynamic, multi-entity rendering engine capable of handling enemies, NPCs, and scrolling environments.

*   **Object Pooling:** 
    *   Create a robust Object Attribute Memory (OAM) manager to handle multiple sprites on screen simultaneously without flickering or memory leaks.
*   **Hardware Scrolling:** 
    *   Implement background map scrolling utilizing the GBA's hardware registers (`REG_BG0HOFS`, `REG_BG0VOFS`) to allow levels larger than the 240x160 screen resolution.
*   **Collision Detection:** 
    *   Establish an AABB (Axis-Aligned Bounding Box) collision system to handle interactions between the player, environmental solid tiles, and other entities.

## Phase 9: The Event & Dialogue Engine
**Objective:** Create a dynamic, data-driven system for handling NPC interactions, cutscenes, and state transitions without hardcoding logic into the main update loop.

*	**Data Architecture:**
	*	Design a DialogueNode struct to hold text strings, text-crawl speeds, and pointers to the next node in the conversation tree.
	*	Establish an EventTrigger struct that pairs a specific map coordinate (AABB bounding box) with a callback function or state change.
*	**Logic Integration:**
	*	Hook into the collision detection from Phase 8. When the player presses 'A' within an EventTrigger bounding box, pause the main entity update loop and hand control over to the Dialogue/Event manager.
*	**UI Implementation:**
	*	Expand upon our Phase 1 TTE (Tonc Text Engine) foundation to render a standardized dialogue box at the bottom of the screen.
	*	Implement a typewriter text-crawl effect and a blinking cursor to indicate when the player needs to press 'A' to advance the text.

## Phase 10: Content, Minigames & Polish
**Objective:** Branch the core engine out into specialized, divergent gameplay states that utilize custom rendering and input logic.

*   **Architectural Concept:** 
    *   Leverage the established `GameState` interface to instantiate completely isolated gameplay loops that don't interfere with the main physics or UI logic.
*   **Implementation Targets:**
    *   **The Nest TV:** A puzzle-based channel-surfing minigame requiring the player to tune to specific frequencies.
    *   **The Shoreline:** A timing-based fishing minigame utilizing custom sprite animations and isolated RNG calculations. 
*   **Final Polish:** 
    *   Implement screen transitions (fade-to-black using palette manipulation).
    *   Finalize the integration of all completed graphics and audio assets into the production ROM.
