    SET A 0   // Assign the LEM1802
    SET B NW  // hardware the section
    30000     // of RAM starting at
    HWI 0     // 30000 for video RAM

    SET I 0             // for(I = 0; I < 128; I++)
loop_condition:         // { X = WhatToDisplay(I);
    IFL I NW            //   Y = WhereToDisplay(I);
    128                 //   Display(X, Y);
      SET PC loop_body  // }
    SET PC end
loop_body:
    SET X 15   // Foreground color
    SHL X 4
    ADD X 0    // Background color
    SHL X 4
    ADD X 0    // Should blink
    SHL X 1
    ADD X I    // Character to display
    SET Y B    // Start of video RAM
    ADD Y I    // Offset from start
    SET [Y] X  // Display
    ADD I 1
    SET PC loop_condition
end:
