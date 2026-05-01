# GBAmore Engine Architectural Roadmap

## Phase 1-3: The Engine Foundation (Completed)
**Goal:** Establish the underlying architecture and rendering pipeline.
*   **Milestones:** CI/CD pipeline establishment, standard devkitARM Makefile, core Finite State Machine (FSM), Tonc Text Engine (TTE) integration, and the Object Attribute Memory (OAM) pipeline for the Chipmunk sprite.
*   **Status:** Testing/Verification pending.

## Phase 4: The Skeleton Loop (In Progress)
**Goal:** Establish the basic routing and persistent data tracking.
*   **Tasks:** Refactoring the FSM to use static memory (no new/delete to prevent crashes), establishing the `PlayerProfile` struct, and creating text-based state transitions between the Title Menu, the Hub (Nest), and a Level.
*   **Status:** Jules is currently building.

## Phase 5: The Automap Backend
**Goal:** Build the Hollow Knight-style exploration data structure.
*   **Data Architecture:** Initialize a 2D array representing the global grid of the Hub World. Given the GBA's strict memory limits (256KB EWRAM), implement this as a bitfield or boolean array (`permanentMap` and `tempRunMap`) rather than an array of integers to minimize the footprint.
*   **Logic Integration:** Update the array dynamically based on the `player_x` and `player_y` coordinates transforming across room boundaries or screen transitions. If the Chipmunk successfully completes a level, `tempRunMap` data pushes to `permanentMap`; if the Chipmunk resets, the temporary data clears.
*   **UI Implementation:** Create a new `GameState` (e.g., `State_Map`) accessible via the START or SELECT button from the active gameplay loop. Render the array to the screen using primitive tile drawing or dynamic TTE text symbols to indicate explored rooms vs. uncharted territory.

## Phase 6: The Interactive Hub (The Nest)
**Goal:** Upgrade the Hub World from a text menu to a playable room.
*   **Tasks:** Add top-down movement logic to `State_Nest`. Use coordinate-based triggers and TTE (text engine) to place the four anchor interactions: The TV (lore), The Ring (agate piece tracking), The Bed (saving/resting), and The Clock (thematic messaging).

## Phase 7: Cartridge SRAM (The Save Engine)
**Goal:** Establish persistent memory so the player's progress survives a console power cycle.
*   **Toolchain Validation:** Ensure the linker is properly assigning the SRAM section in the compiled `.gba` header so emulators and flash carts allocate the 32KB/64KB save file.
*   **Data Serialization:** Expand the `PlayerProfile` struct to encapsulate all necessary save variables (Agate count, Dash unlock boolean, Automap bitfield).
*   **Hardware Interface:** Utilize `libtonc`'s `sram_write()` and `sram_read()` functions. Implement a checksum validation upon reading to ensure the save data isn't corrupted.
*   **Game Loop Integration:** Tie the `sram_write()` execution to a physical interaction in the game world, specifically interacting with the "Bed" asset inside the Nest.
*   *Technical Consideration:* Is the standard `libtonc` SRAM implementation sufficient for our needs, or do we need to manually manage the waitstates for SRAM access to ensure data integrity across different flash carts?

## Phase 8: The Background Asset Pipeline
**Goal:** Move from pure text backgrounds to actual pixel-art environments.
*   **Tasks:** Write a C++ wrapper to load tilemaps created in external software (like Tiled) and integrate background tilesets into the asset pipeline. This is when the game stops looking like a terminal and starts looking like a Pokémon game.
*   **Hardware Scrolling:** Implement background map scrolling utilizing the GBA's hardware registers (`REG_BG0HOFS`, `REG_BG0VOFS`) to allow levels larger than the 240x160 screen resolution.

## Phase 9: The Audio Engine (Maxmod)
**Goal:** Integrate the Maxmod library to handle background music (BGM) and sound effects (SFX) without stalling the CPU.
*   **Makefile Modification:** Introduce the Maxmod audio processing rules to the `Makefile`. Configure the pipeline to convert audio assets placed in an `audio/` directory into a compiled `soundbank.bin`.
*   **Asset Pipeline:** 
    *   **BGM:** Utilize tracker formats (`.mod`, `.it`, or `.s3m`) for the lo-fi boom-bap loops. Tracker formats are hardware-efficient and save massive amounts of ROM space compared to raw audio.
    *   **SFX:** Utilize standard `.wav` files for short UI interactions (menu blips, agate collection chimes).
*   **C++ Implementation:** Initialize the engine in `main.cpp` via `mmInitDefault()`. Implement `mmStart()` for continuous tracker playback on level initialization, and `mmEffect()` for asynchronous sound effects triggered by player input.
*   *Technical Consideration:* Will the Maxmod VBlank interrupt handler conflict with our existing OAM update/copy routine happening during VBlank?

## Phase 10: Entities & Collision (The Shadows)
**Goal:** Introduce the non-violent obstacles and spatial limits, and manage dynamic multi-entity rendering.
*   **Object Pooling:** Create a robust Object Attribute Memory (OAM) manager to handle multiple sprites on screen simultaneously without flickering or memory leaks.
*   **Collision Detection:** Establish an AABB (Axis-Aligned Bounding Box) collision system to handle interactions between the player, environmental solid tiles, and other entities.
*   **Entity AI:** Program the basic patrol AI for the "Storm Cloud" sprites.
*   **Soft Reset Mechanic:** Build the "soft reset" mechanic where touching a shadow teleports the Chipmunk back to the start of the room without a game over.

## Phase 11: Abilities & Upgrades
**Goal:** Implement the unlockable mechanics.
*   **Tasks:** Program the "Dash" (speed boost on button hold) and the "Push" (Sokoban block manipulation). Link these abilities to boolean flags in the `PlayerProfile` so they only activate after finding specific agate pieces.

## Phase 12: The Event & Dialogue Engine
**Goal:** Create a dynamic, data-driven system for handling NPC interactions, cutscenes, and state transitions without hardcoding logic into the main update loop.
*   **Data Architecture:** Design a `DialogueNode` struct to hold text strings, text-crawl speeds, and pointers to the next node in the conversation tree. Establish an `EventTrigger` struct that pairs a specific map coordinate (AABB bounding box) with a callback function or state change.
*   **Logic Integration:** Hook into the collision detection from Phase 10. When the player presses 'A' within an `EventTrigger` bounding box, pause the main entity update loop and hand control over to the Dialogue/Event manager.
*   **UI Implementation:** Expand upon our Phase 1 TTE (Tonc Text Engine) foundation to render a standardized dialogue box at the bottom of the screen. Implement a typewriter text-crawl effect and a blinking cursor to indicate when the player needs to press 'A' to advance the text.

## Phase 13: Content Assembly & Polish
**Goal:** Build the actual game out of the engine we've constructed.
*   **Architectural Concept:** Leverage the established `GameState` interface to instantiate completely isolated gameplay loops that don't interfere with the main physics or UI logic.
*   **Content Tasks:**
    *   Design and load the tilemaps for Level 1 (The Shoreline), Level 2 (The Living Room), and Level 3 (The Duluth Hotel).
    *   Write and insert the specific memory texts into the TTE cutscenes.
    *   Draw the visual upgrades for the Nest (Shoddy vs. Cozy).
    *   **The Nest TV:** A puzzle-based channel-surfing minigame requiring the player to tune to specific frequencies.
    *   **The Shoreline:** A timing-based fishing minigame utilizing custom sprite animations and isolated RNG calculations. 
*   **Final Polish:** Implement screen transitions (fade-to-black using palette manipulation). Finalize the integration of all completed graphics and audio assets into the production ROM.
