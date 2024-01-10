//include
#include<stdio.h>
#include<stdlib.h>
#include<string.h>    // for strlen
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include <pthread.h>
//end of include

#define DATA_SIZE 100
int FIRST_ARRAY[DATA_SIZE];
int tempFIRST_ARRAY[DATA_SIZE];
int SECOND_ARRAY[DATA_SIZE];
int tempSECOND_ARRAY[DATA_SIZE];
int RESULT_ARRAY[DATA_SIZE];
int CARRY_ARRAY[DATA_SIZE+1];
void *threadAddition(void *arg);
//end of declaration table

// Function definition of error handling
void handleError(const char *errorMessage) {
    fprintf(stderr, "%s\n", errorMessage);
    exit(EXIT_FAILURE);
}

//main code
int main(int argc, char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char *message,*msg2,*msg3;
    char INPUT_STRING[DATA_SIZE];
    //end of declaration
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    } 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(60000);
    // Bind
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Bind failed");
        return 1;
    }
    puts("Socket is binded");
    // Listen
    listen(socket_desc, 3);
    // Accept and incoming connection
    puts("Waiting for incoming connections..."); 
    c = sizeof(struct sockaddr_in);
    while( new_socket = 
           accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) )
    {
        puts("Connection accepted");
        fflush(stdout);
         // Clear the input arrays before reading the next set of arrays
        memset(FIRST_ARRAY, -1, sizeof(FIRST_ARRAY));
        memset(SECOND_ARRAY, -1, sizeof(SECOND_ARRAY));
        memset(tempFIRST_ARRAY, -1, sizeof(tempFIRST_ARRAY));
        memset(tempSECOND_ARRAY, -1, sizeof(tempSECOND_ARRAY));
        memset(RESULT_ARRAY, -1, sizeof(RESULT_ARRAY));
        memset(CARRY_ARRAY, 0, sizeof(CARRY_ARRAY));
        memset(INPUT_STRING, -1, sizeof(INPUT_STRING));
        //inform the client
        message = "Hello, this is Array Addition Server!\n\n";
        msg2="Please enter the first array for addition:";
        write(new_socket, message, strlen(message));
        write(new_socket, msg2, strlen(msg2));
        memset(INPUT_STRING, 0, sizeof(INPUT_STRING));

        // Receive the integer array as a string
        ssize_t bytes_received = read(new_socket, INPUT_STRING, sizeof(INPUT_STRING) - 1);
        if (bytes_received < 0) {
            perror("Read failed");
            close(new_socket);
            continue; // Move to the next iteration of the loop
        }

        // Transform the string into an integer array
        int count = 0;
        int exceed_count=1;
        char *token = strtok(INPUT_STRING, " ");
        while (token != NULL && count < DATA_SIZE) {
            if (sscanf(token, "%d", &FIRST_ARRAY[count]) != 1||FIRST_ARRAY[count]>999) {
                 const char *errorMessage = "ERROR: The inputted integer array contains non-integer characters or out-of-range integers.\n You must input only integers and empty spaces to separate inputted integers!\n";
                 write(new_socket, errorMessage, strlen(errorMessage));
                 close(new_socket);
                 //continue;
            }
            token = strtok(NULL, " ");
            count++;
            exceed_count++;
        }
         // Check if the number of integers exceeds the maximum data size
        if (exceed_count >= (DATA_SIZE-1)/2) {
            const char *errorMessage ="ERROR: Input string exceeds the maximum supported length.\n";
                 write(new_socket, errorMessage, strlen(errorMessage));
                 close(new_socket);
        }
   
        // LSB is last element of array
        // changing position of elements and storing the array backwards for operations
        int temp_index=0;
        for (int i = count-1; i>=0; i--) {   
            tempFIRST_ARRAY[temp_index]=FIRST_ARRAY[i];
            temp_index++;
        }
        // Print the elements of FIRST_ARRAY for verification
        printf("Transformed FIRST_ARRAY: ");
         for (int i =0; i<count; i++) {
            printf("%d ", tempFIRST_ARRAY[i]);   
           
        }
        printf("\n");
        //counting the number of integers in the given array
        int numIntegers = 0;
        while (FIRST_ARRAY[numIntegers] != -1) {       
            numIntegers++;
        }

        //similar operations for second array

        msg3="Please enter the second array for addition:";
        write(new_socket, msg3, strlen(msg3));
       memset(INPUT_STRING, 0, sizeof(INPUT_STRING));
        ssize_t bytes_received2 = read(new_socket, INPUT_STRING, sizeof(INPUT_STRING) - 1);
        if (bytes_received2 < 0) {
            perror("Read failed");
            close(new_socket);
            continue; // Move to the next iteration of the loop
        }

        // Transform the string into an integer array
        int count2 = 0;
        int exceed_count2=1;
        char *token2 = strtok(INPUT_STRING, " ");
        while (token2 != NULL && count2 < DATA_SIZE) {
            if (sscanf(token2, "%d", &SECOND_ARRAY[count2]) != 1||SECOND_ARRAY[count2]>999) {
              const char *errorMessage = "ERROR: The inputted integer array contains non-integer characters or out-of-range integers.\n You must input only integers and empty spaces to separate inputted integers!\n";
                 write(new_socket, errorMessage, strlen(errorMessage));
                 close(new_socket);
                 //continue;
            }
            token2 = strtok(NULL, " ");
            count2++;
            exceed_count2++;
        }
             // Check if the number of integers exceeds the maximum data size
        if (exceed_count2 >= (DATA_SIZE-1)/2) {
            const char *errorMessage ="ERROR: Input string exceeds the maximum supported length.\n";
                 write(new_socket, errorMessage, strlen(errorMessage));
                 close(new_socket);
        }
        int tmp_ndx=0;
        for (int i = count2-1; i >=0 ; i--) {
           tempSECOND_ARRAY[tmp_ndx]=SECOND_ARRAY[i];
           tmp_ndx++;
        }
        printf("Transformed SECOND_ARRAY: ");
         for (int i =0; i <count2 ; i++) {
           printf("%d ", tempSECOND_ARRAY[i]);
        }
        printf("\n");
        
        //error detection and output for different number of integers
       if (count != count2) {
        const char *errorMessage = "ERROR: The number of integers are different for both arrays.\n You must send equal number of integers for both arrays!\n";
        write(new_socket, errorMessage, strlen(errorMessage));
        close(new_socket);
        //continue;
        }
 
      //creating threads
      pthread_t THREAD_ARRAY[DATA_SIZE];
      for (int i = 0; i < numIntegers; i++) 
      {
        int *index = malloc(sizeof(int)); // Allocate memory for the index
        *index = i; // Copy the current index value
        pthread_create(&THREAD_ARRAY[i], NULL, threadAddition, (void *)index);
      }  


         // Wait for all threads to finish
        for (int i = 0; i < numIntegers; i++) {
            pthread_join(THREAD_ARRAY[i], NULL);
        }


        // Add the calculated carries to RESULT_ARRAY
        for (int i = 0; i < numIntegers; i++) {
            RESULT_ARRAY[i] = RESULT_ARRAY[i] +CARRY_ARRAY[i];
            if (RESULT_ARRAY[i] > 999) {
                RESULT_ARRAY[i] %= 1000;
                CARRY_ARRAY[i + 1] = 1;
            } /*else {
                CARRY_ARRAY[i + 1] = 0;
            }*/
        }
        if(CARRY_ARRAY[numIntegers]==1)
        {
             RESULT_ARRAY[numIntegers]=1;
             printf("Final Result array: ");
             for (int i = numIntegers; i>=0; i--) {
             printf("%d ", RESULT_ARRAY[i]);
        }
        }
        else{
             //print result array for verification
             //we converted the arrays backwards at the beginnning now printing the results also backwards
             //to get the real answer on the screen
          printf("Final Result array: ");
        for (int i = numIntegers-1; i>=0; i--) {
            printf("%d ", RESULT_ARRAY[i]);
        }

        }
       
          /* printf("Final carry array: ");
        for (int i =0; i<=numIntegers; i++) {
            printf("%d ", CARRY_ARRAY[i]);
        }*/
         

           // Send the result indicator string
        const char *resultIndicator = "The result of array addition are given below:\n ";
        ssize_t indicator_bytes_sent = write(new_socket, resultIndicator, strlen(resultIndicator));
        if (indicator_bytes_sent < 0) {
            perror("Write failed");
            close(new_socket);
            continue;
        }
        printf("\n");
          char resultString[DATA_SIZE * 4]; // Assuming 4 characters per integer (including space)
        int offset = 0;
           if(CARRY_ARRAY[numIntegers]==1)
        {
             for (int i = numIntegers; i >= 0; i--) {
            offset += sprintf(resultString + offset, "%d ", RESULT_ARRAY[i]);    
        }
        }
        else{
        for (int i = numIntegers - 1; i >= 0; i--) {
            offset += sprintf(resultString + offset, "%d ", RESULT_ARRAY[i]);
        }
        }

        // Send the result to telnet connection
        ssize_t bytes_sent = write(new_socket, resultString, strlen(resultString));
        if (bytes_sent < 0) {
            perror("Write failed");
            close(new_socket);
            continue;
        }
        //exit the array server with message
        const char *endMessage = "\nThank you for Array Addition Server! Good Bye!\n";
        write(new_socket, endMessage, strlen(endMessage));

      // Close the connection to the current telnet client
        close(new_socket);
    }    
    

     if (new_socket < 0)
    {
        perror("Accept failed");
        return 1;
    }

    return 0;
}

// Function definition for array addition in a thread
void *threadAddition(void *arg) {
    int index = *((int *)arg);

    RESULT_ARRAY[index] = tempFIRST_ARRAY[index] + tempSECOND_ARRAY[index];

    // Calculate carry within the thread
    if (RESULT_ARRAY[index] > 999) {
        RESULT_ARRAY[index] %= 1000;
        CARRY_ARRAY[index + 1] = 1;
    } else {
        CARRY_ARRAY[index + 1] = 0;
    }
    free(arg);
    pthread_exit(NULL);
}

