#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void fill_image2D(char image_2d[50][50], int height, FILE *inFile);

void get_patterns(int argc, char* argv[], char image_2d[50][50], int img_height, int img_width);

void image_to_sub(char pattern_2d[3][3], char image_2d[50][50], int img_height, int img_width, char* pattern);

bool compare(char test_array[3][3], char pattern_2d[3][3]);

void print_match(char* pattern, int count, int match_array[300]);

int main(int argc, char* argv[])
{
    //initialize each file
    char* image_file = argv[1];

    //open files
    FILE *inFile = fopen(image_file, "r");

    if (inFile == NULL){
        printf("File not found\n");
        exit(404);
    }

    //height and width of image
    int img_height, img_width;
    fscanf(inFile, "%d%d", &img_width, &img_height);

    // create 2d char string with max 50x50, fill with data and close
    char image_2d[50][50];
    fill_image2D(image_2d, img_height, inFile);
    fclose(inFile);

    get_patterns(argc, argv, image_2d, img_height, img_width);


    return 0;
}

void fill_image2D(char image_2d[50][50], int height, FILE *inFile){
    // method that fills 2d char array with data from file

    //each row can be represented a string
    for (int i = 0; i < height; i++){
        fscanf(inFile, "%s", &image_2d[i][0]);
    }

}

void get_patterns(int argc, char* argv[], char image_2d[50][50], int img_height, int img_width){

    if (argc < 3){
        printf("No patterns given\n");
        exit(404);
    }
    // loop through arguments after the image file
    for(int i = 2; i < argc; i++){

        // file location string
        char* pattern = argv[i];

        //open file and initialize height and width
        FILE *pat_file = fopen(pattern, "r");
        if (pat_file == NULL){
            printf("File not found\n");
            exit(404);
        }
        int pat_height, pat_width;
        fscanf(pat_file, "%d%d", &pat_width, &pat_height);

        //create a 2d array for the pattern
        char pattern_2d[3][3];

        //fill pattern array
        for (int i = 0; i < pat_height; i++){
            fscanf(pat_file, "%s", &pattern_2d[i][0]);
        }

        // function to compare pattern to image
        image_to_sub(pattern_2d, image_2d, img_height, img_width, pattern);

        fclose(pat_file);

    }
}

void image_to_sub(char pattern_2d[3][3], char image_2d[50][50], int img_height, int img_width, char* pattern){

    //initialize the count of matches and array that stores the coordinates
    int count = 0;
    int match_array[300];

    //Loop through each coordinate in the 2D image
    for (int i = 0; i < img_height; i++){

        for (int j = 0; j < img_width; j++){

            //create the 3x3 2d array that will store the sub array from the image
            char test_array[3][3];

            //bounds checking to make sure it is getting a proper 3x3 array
            if(img_width - j >= 3 && img_height - i >= 3){

                //loop through each 3x3 sub array and place it in the 2d test array
                for (int x = i; x < i+3; x++){

                    for (int y = j; y < j+3; y++){

                        test_array[x-i][y-j] = image_2d[x][y];
                    }
                }

                // call function to check if it is a match
                bool match = compare(test_array, pattern_2d);

                // if there is a match, add to count and fill match array with coords
                if (match == true){
                    match_array[count] = i+1;
                    match_array[count+1] = j+1;
                    count += 2;

                }

            }

        }

    }

    // print the match data
    print_match(pattern, count, match_array);

}

bool compare(char test_array[3][3], char pattern_2d[3][3]){

    //method that compares 3x3 image sub array to 3x3 pattern
    //returns match boolean after loop
    bool match = true;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (test_array[x][y] != pattern_2d[x][y]) {
                match = false;
                break;
            }
        }
    }

    return match;

}

void print_match(char* pattern, int count, int match_array[300]){

    //prints the matching data
    printf("%s\n", strrchr(pattern, '/')+1);
    printf("    %d ", count/2);
    for(int a = 0; a < count; a++){
        printf("%d ", match_array[a]);
    }
    printf("\n");
}