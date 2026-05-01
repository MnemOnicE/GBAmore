#!/bin/bash
cat << 'PATCH' > header.patch
--- include/State_Level1.h
+++ include/State_Level1.h
@@ -3,7 +3,6 @@

 #include "GameState.h"
-#include "shadow.h"
 #include <tonc.h>

 /**
  * Represents the first gameplay state (Level 1) and manages its lifecycle.
@@ -58,6 +57,8 @@
     Entity entities[16];
     int num_entities;
     bool inCutscene;
+    int camera_x;
+    int camera_y;

     bool checkCollision(Entity& a, Entity& b);

PATCH
patch include/State_Level1.h < header.patch
