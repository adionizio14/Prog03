#!/bin/bash

function get_pat_files() {
  #initalize the folder and array for each function call
  local nested_folder="$1"
  local pattern_files=()

  #loop through each file in the current folder
    for file in "$nested_folder"/*; do

      #check that the file is either a .pat or .txt
        if [ "${file##*.}" == "pat" ] || [ "${file##*.}" == "txt" ]; then
            pattern_files+=("$file")

      #if not, make sure it exists and call the function again to check the new folder
        elif [ -d "$file" ]; then
            pattern_files+=($(get_pat_files "$file"))
        fi
    done

    #print the result array to the variable in
    echo "${pattern_files[@]}"
}
# check to make sure the arguments are valid
if [ "$#" -lt 3 ]; then
  echo "Not enough arguments given"
fi

if [ "$#" -gt 3 ]; then
  echo "Too many arguments given"
fi

#initialize each variable
pattern_folder="$1"
image_folder="$2"
output_folder="$3"

#fill pattern files array recursively using a function call
pattern_files=($(get_pat_files "$pattern_folder"))

#build Version2 script
gcc ./Programs/prog03v2.c -o prog03v2

#loop through each image in the image folder
for image in "$image_folder"/*;
do
  #run the program with each image, the output destination and every pattern in the array
    ./prog03v2 "$image" "$output_folder" "${pattern_files[@]}"

done