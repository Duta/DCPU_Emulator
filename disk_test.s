    SET A 0       // if(!isSupported(disk))
    HWI 1         // { goto end; }
    IFE B 0
      SET PC end

    SET PUSH 25   // Push some values onto
    SET PUSH 16   // the stack which will
    SET PUSH 9    // popped off and written
    SET PUSH 4    // to disk (in reverse
    SET PUSH 1    // order 'cause it's a stack)

    SET I 0               // for(I = 0, J = 30000; SP; I++)
    SET J NW              // { RAM[J+I] = pop(); }
    30000
loop_condition_1:
    IFN SP 0
      SET PC loop_body_1
    SET PC loop_end_1
loop_body_1:
    ADD J I
    SET [J] POP
    SUB J I
    ADD I 1
    SET PC loop_condition_1
loop_end_1:

    SET A 0       // if(!isSupported(disk))
    HWI 1         // { goto end; }
    IFE B 0
      SET PC end

    SET A 1       // if(disk.isWriteLocked())
    HWI 1         // { goto end; }
    IFE X 1       // push(disk.numWordsPerSector);
      SET PC end
    SET PUSH B

    SET A 17  // initialSector = 0;
    SET B 0   // numSectors = numWords/pop() + 1;
    SET C I   // bufferStart = 30000;
    DIV C POP // write(initialSector, numSectors, bufferStart);
    ADD C 1
    SET X J
    HWI 1

    SET A 0               // for(A = 0; A < I; A++)
loop_condition_2:         // { RAM[J+A] = 1; }
    IFL A I
      SET PC loop_body_2
    SET PC loop_end_2
loop_body_2:
    ADD J A
    SET [J] 1
    SUB J I
    ADD A 1
    SET PC loop_condition_2
loop_end_2:

    SET Z 1

    SET A 0       // if(!isSupported(disk))
    HWI 1         // { goto end; }
    IFE B 0
      SET PC end

    SET A 16  // bufferStart = 40000;
    SET X NW  // read(initialSector, numSectors, bufferStart);
    40000
    HWI 1

    SET Z 2

end:
