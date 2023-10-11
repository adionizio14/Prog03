#!/bin/bash

# check to make sure the arguments are valid
if [ "$#" -lt 3 ]; then
  echo "Not enough arguments given"
fi

if [ "$#" -gt 3 ]; then
  echo "Too many arguments given"
fi

#initialize each variable
image_folder="$1"
scrap_folder="$2"
write_folder="$3"

# make the scrap directory
mkdir "$scrap_folder"

#loop through the image folder
for file in "$image_folder"/*;
do
  #check that the file is regular
  if [ -f "$file" ]; then

    #create a temp file
    temp_file="$scrap_folder/$(basename "$file")"

    #rewrite file endings and copy data into them
    sed 's/\r$//' "$file" > "$temp_file"

  fi

done

#when the file endings are done, move the contents of the scrap folder into designated folder
mv "$scrap_folder"/* "$write_folder/"

#get ride of scrap folder and everything in it
rm -rf "$scrap_folder"




