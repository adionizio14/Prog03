#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

void fill_image2D(char image_2d[50][50], int height, FILE *inFile);

void get_patterns(int argc, char* argv[], char image_2d[50][50], int img_height, int img_width);

void image_to_sub(char pattern_2d[3][3], char image_2d[50][50], int img_height, int img_width, char* pattern, FILE *out_file);

bool compare(char test_array[3][3], char pattern_2d[3][3]);

void fill_output_file(char* pattern, int count, int match_array[300], FILE *out_file);

char* make_dir(char* output_dir);

char* make_new_file_name(char* image_name);

char* make_output_file_path(char* new_name, char* output_dir);

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
        printf("No Directory given\n");
        exit(404);
    }

    //initialize the output directory and get the name of the image
    char* output_dir = argv[2];
    char* image_name = argv[1];
    image_name = strrchr(image_name, '/') + 1;

    //create the new name and also the correct file path for the txt file
    char* new_name = make_new_file_name(image_name);
    output_dir = make_dir(output_dir);
    char* output_file_path = make_output_file_path(new_name, output_dir);

    // create/open the output txt in write mode
    FILE *out_file;
    out_file = fopen(output_file_path,"w");

    if (argc < 4){
        printf("No pattern given\n");
        exit(404);
    }

    // loop through arguments after the image file
    for(int i = 3; i < argc; i++){

        // file location string
        char* pattern = argv[i];

        //open file and initialize height and width
        FILE *pat_file = fopen(pattern, "r");
        int pat_height, pat_width;
        fscanf(pat_file, "%d%d", &pat_width, &pat_height);

        if (pat_file == NULL){
            printf("File not found\n");
            exit(404);
        }

        //create a 2d array for the pattern
        char pattern_2d[3][3];

        //fill pattern array
        for (int i = 0; i < pat_height; i++){
            fscanf(pat_file, "%s", &pattern_2d[i][0]);
        }

        // function to compare pattern to image
        image_to_sub(pattern_2d, image_2d, img_height, img_width, pattern, out_file);

        fclose(pat_file);

    }

    fclose(out_file);
}

void image_to_sub(char pattern_2d[3][3], char image_2d[50][50], int img_height, int img_width, char* pattern, FILE *out_file){

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

    // write the match data to the output txt
    fill_output_file(pattern, count, match_array, out_file);

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

void fill_output_file(char* pattern, int count, int match_array[300], FILE *out_file){

    //writes the match data to the output txt file
    fprintf(out_file,"%s\n", strrchr(pattern, '/')+1);
    fprintf(out_file,"    %d ", count/2);
    for(int a = 0; a < count; a++){
        fprintf(out_file,"%d ", match_array[a]);
    }
    fprintf(out_file,"\n");


}

char* make_dir(char* output_dir){

    //formats the input directory so it is all the same
    if (output_dir[strlen(output_dir) - 1] == '/'){
        output_dir[strlen(output_dir) - 1] = '\0';
    }

    //creates directory and returns back with new format
    mkdir(output_dir, 0777);

    return output_dir;
}

char* make_new_file_name(char* image_name){
    // method that creates the new file name requested in handout

    //creates new char string of ending and also pointer for the .img or .pat
    char* ending_txt = "_Matches.txt";
    char* dot_pointer = strrchr(image_name, '.');

    //gets length of new string by subtracting the .img or .pat
    int new_name_len = strlen(image_name) - strlen(dot_pointer);
    //allocates memory with ending text
    char* new_name = (char*)malloc(new_name_len + strlen(ending_txt) + 1);

    //fills new char string with everything before .img/.pat
    for (int i = 0; i < new_name_len; i++){
        new_name[i] = image_name[i];
    }

    //end the char string
    new_name[new_name_len] = '\0';

    //adds ending text to end of string
    strcat(new_name, ending_txt);

    //returns
    return new_name;
}

char* make_output_file_path(char* new_name, char* output_dir){
    //method that adds directory and new file name together

    //first allocate memeory for both strings and /
    char* output_file_path = (char*)malloc(strlen(new_name) + strlen(output_dir) + 2);

    //initalize the char string
    output_file_path[0] = '\0';

    //add all strings together in correct order and return
    strcat(output_file_path, output_dir);
    strcat(output_file_path, "/");
    strcat(output_file_path, new_name);

    return output_file_path;
}
