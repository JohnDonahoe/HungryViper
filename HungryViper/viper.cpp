// ============================================
// The Viper class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "viper.h"

void viper_init (Viper * v)
{
    // TODO: Implement
    //1. Set starting coordinates for your viper head and previous
    v -> head_x = 5;
    v -> head_y = 5;
    v -> head_px = 4;
    v -> head_py = 5;
    v -> length = 3;
    v -> score = 1;
    v -> locations[0].x = 5;
    v -> locations[0].y = 5;
    v -> locations[1].x = 4;
    v -> locations[1].y = 5;
    v -> locations[2].x = 3;
    v -> locations[2].y = 5;
    v -> locations[0].direction = 4;
    v -> INVINCIBL = false;
    //2. Initialize all location for your maximum viper body (loop through the viper)

    //3. Set starting length of your viper

    //4. Set starting coordinates for the rest of viper body from starting length

    //5. Initialize viper status

}
