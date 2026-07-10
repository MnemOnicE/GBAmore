sed -i 's/if (player_x > 140 && player_y > 100) {/if (player_x > DOOR_ZONE_X \&\& player_y > DOOR_ZONE_Y) {/' source/State_Nest.cpp
sed -i 's/else if (player_x < 50 && player_y < 40) {/else if (player_x < BED_ZONE_X \&\& player_y < BED_ZONE_Y) {/' source/State_Nest.cpp
sed -i 's/else if (player_x > 140 && player_y < 40) {/else if (player_x > TV_ZONE_X \&\& player_y < TV_ZONE_Y) {/' source/State_Nest.cpp
sed -i 's/else if (player_x < 50 && player_y > 100) {/else if (player_x < RING_ZONE_X \&\& player_y > RING_ZONE_Y) {/' source/State_Nest.cpp

# Add constants at the top
sed -i 's/#include <tonc.h>/#include <tonc.h>\n\nconst int BED_ZONE_X = 50;\nconst int BED_ZONE_Y = 40;\nconst int TV_ZONE_X = 140;\nconst int TV_ZONE_Y = 40;\nconst int RING_ZONE_X = 50;\nconst int RING_ZONE_Y = 100;\nconst int DOOR_ZONE_X = 140;\nconst int DOOR_ZONE_Y = 100;\n/' source/State_Nest.cpp
