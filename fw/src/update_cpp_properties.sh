#!/bin/bash

# Extract -D defines from the Makefile output
DEFINES=$(make -n | grep -oE -- '-D\w+' | sed 's/-D//g' | sort -u | jq -R . | jq -s .)

# Define the path to c_cpp_properties.json
PROP_FILE="../../.vscode/c_cpp_properties.json"

# Insert extracted defines into the JSON file
jq --argjson defines "$DEFINES" '
  .configurations[0].defines = $defines
' "$PROP_FILE" > tmp.json && mv tmp.json "$PROP_FILE"

echo "Updated c_cpp_properties.json with extracted defines!"
