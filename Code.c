int updateAllMatrices(char *firstMatrix, char *secondMatrix, char *resMatrix)
{
    /*
    Initialize all needed variables:
    matrices is a string that holds all the input not including the command
    i and j are counters
    placementOfFirstComma is the index of the first comma - at the base case it is 5 (Exmaple: add_mat MAT_A, MAT_B, MAT_C)
    placementOfSecondComma is the index of the second comma
    placementOfCommandEnd is the index of the end of the command
    startOfThirdMatrixName is the index of the start of the third matrix name
    hasSeenFirstComma is a flag that indicates if the first comma was seen
    hasSeenSecondComma is a flag that indicates if the second comma was seen
    isInThirdMatrixName is a flag that indicates if the third matrix name is being read
    hasSeenEndOfCommand is a flag that indicates if the end of the command was seen
    */
    char matrices[SIZE_OF_MATRIX_NAME*10];
    int i, j;
    int placementOfFirstComma = 5, placementOfSecondComma = 0, placementOfCommandEnd = 0, startOfThirdMatrixName = 0;
    int hasSeenFirstComma = FALSE, hasSeenSecondComma = FALSE, isInThirdMatrixName = FALSE, hasSeenEndOfCommand = FALSE;

    // get the entire input into matrices
    scanf("%[^\n]", matrices);

    // run on all the input and find the first comma, the second comma and the end of the command
    for(i = 0; i < strlen(matrices) && matrices[i] != '\0'; i++)
    {
        // if the current character is not a whitespace and we have already seen the end of the command - print an error and return
        if(matrices[i] != ' ' && matrices[i] != '\t' && hasSeenEndOfCommand == TRUE)
        {
            // if there are extraneous text after the end of the command - print an error and return
            printf("Extraneous text after end of command\n");

            // clear the buffer
            while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF);

            // return false (the command is not valid)
            return FALSE;
        }

        // Check if we are in the end of the command
        if(i == placementOfCommandEnd && placementOfCommandEnd != 0)
        {
            hasSeenEndOfCommand = TRUE; // update the flag that indicates that we have seen the end of the command
        }

        // if we are in the start of the third matrix name
        if(hasSeenFirstComma == TRUE && hasSeenSecondComma == TRUE && isInThirdMatrixName != TRUE && matrices[i] == 'M')
        {
            // set the flag to true and save the index of the start of and the end of the third matrix name
            isInThirdMatrixName = TRUE;
            startOfThirdMatrixName = i; // save the index of the start of the third matrix name
            placementOfCommandEnd = i + 4; // the length of the third matrix name is 5 so the end of the command is the index of the start of the third matrix name + 4
        }

        // if the current character is a whitespace and we haven't seen the first comma yet - update the index of the first comma
        if((matrices[i] == ' ' || matrices[i] == '\t') && hasSeenFirstComma == FALSE)
        {
            placementOfFirstComma += 1; // update the index of the first comma
        }
        // if the current character is a whitespace and we have seen the first comma but not the second comma - update the index of the second comma
        else if((matrices[i] == ' ' || matrices[i] == '\t') && hasSeenFirstComma == TRUE && hasSeenSecondComma == FALSE)
        {
            placementOfSecondComma += 1; // update the index of the second comma
        }

        // if the current character is a comma
        if(matrices[i] == ',')
        {
            // if the character before or after the comma is a comma - print an error and return
            if(matrices[i-1] == ',' || matrices[i+1] == ',')
            {
                // if the comma is in a place that should have a comma - print an error of multiple consecutive commas
                if(i == placementOfFirstComma || i == placementOfSecondComma)
                {
                    printf("Multiple consecutive commas\n"); // print an error of multiple consecutive commas
                }
                // if the comma is in a place that should not have a comma - print an error of illegal comma
                else
                {
                    printf("Illegal comma\n"); // print an error of illegal comma
                }

                // clear the buffer
                while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF);

                // return false (the command is not valid)
                return FALSE;
            }

            // if we are at the index of the first comma - save the first matrix name
            if(i == placementOfFirstComma)
            {
                // run on all the characters of the first matrix name and save them in firstMatrix
                // SIZE_OF_MATRIX_NAME-1 because the last character is the comma
                for(j = 0; j < SIZE_OF_MATRIX_NAME-1; j++)
                {
                    // save the character in firstMatrix - the index of the character is the index of the comma + the index of the character in the matrix name - 5
                    *(firstMatrix+j) = matrices[j+placementOfFirstComma-5];
                }
                // add the null terminator to the end of the string
                *(firstMatrix+SIZE_OF_MATRIX_NAME-1) = '\0';
                // set the flag that indicates that we have seen the first comma to true
                hasSeenFirstComma = TRUE;
                // update the index of the second comma - the index of the first comma + 6 (at the base case the index of the second comma is the index of the first comma + 6)
                placementOfSecondComma = placementOfFirstComma+6;
            }
            // if we are at the index of the second comma - save the second matrix name
            else if(i == placementOfSecondComma)
            {
                // run on all the characters of the second matrix name and save them in secondMatrix
                // SIZE_OF_MATRIX_NAME-1 because the last character is the comma
                for(j = 0; j < SIZE_OF_MATRIX_NAME-1; j++)
                {
                    // save the character in secondMatrix - the index of the character is the index of the comma + the index of the character in the matrix name - 5
                    *(secondMatrix+j) = matrices[j+placementOfSecondComma-5];
                }
                // add the null terminator to the end of the string
                *(secondMatrix+SIZE_OF_MATRIX_NAME-1) = '\0';
                // set the flag that indicates that we have seen the second comma to true
                hasSeenSecondComma = TRUE;
                // update the index of the end of the command - the index of the second comma + 6 (at the base case the index of the end of the command is the index of the second comma + 6)
                placementOfCommandEnd = placementOfSecondComma+6;
            }
            else
            {
                // if there is a comma in a place that should not have a comma - print an error of "Illegal comma" and return
                printf("Illegal comma\n");

                // clear the buffer
                while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF);

                // return false (the command is not valid)
                return FALSE;
            }
        }
    }

    // make sure we got all the matrices - all the flags should be true
    if(hasSeenFirstComma == FALSE || hasSeenSecondComma == FALSE || isInThirdMatrixName == FALSE)
    {
        // if we are missing an argument - print an error and return
        printf("Missing argument\n");

        while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF); // clear the buffer

        return FALSE; // return false (the command is not valid)
    }

    // run on all the characters of the third matrix name and save them in resMatrix
    // placementOfCommandEnd - startOfThirdMatrixName + 1 because the length of the third matrix name is placementOfCommandEnd - startOfThirdMatrixName + 1 (5)
    for(j = 0; j < placementOfCommandEnd - startOfThirdMatrixName + 1; j++ && isInThirdMatrixName)
    {
        // save the character in resMatrix - the index of the character is the index of the start of the third matrix name + the index of the character in the matrix name - 5  
        *(resMatrix+j) = matrices[j+startOfThirdMatrixName];
    }
    // add the null terminator to the end of the string
    *(resMatrix+SIZE_OF_MATRIX_NAME-1) = '\0';

    // If there weren't commas between the matrix names - print an error and return
    if(matrices[placementOfFirstComma] != ',' || matrices[placementOfSecondComma] != ',')
    {
        printf("Missing comma\n"); // print an error of missing comma

        while((matrices[i] = getchar()) != '\n' && matrices[i] != EOF); // clear the buffer

        return FALSE; // return false (the command is not valid)
    }

    return TRUE; // return true (the command is valid)
}