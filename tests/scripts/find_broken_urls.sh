#!/usr/bin/env bash

# Should be called from the project root in order to search all subdirectories

# Reads each file in the working directory recursively and for each link runs
# curl to check the HTTP status code. If the status code starts with a 2 or a 3,
# then the link is OK, and BAD otherwise. If at least one BAD link is found, the
# script exits with an error code.

bad_count=0
files="$(find . -type f \
    ! -path "*_build*" ! -path "*_intermediate*" ! -path "*.git*" \
    ! -path "*dxflib*" ! -path "*ui/windows/about.cpp" \
    ! -name "*.svg" )"

for file in $files; do
    echo -e "\e[1;34mFile: $file\e[0m"
    # For each url
    for url in $(grep -Eo "https*://[A-Za-z0-9_~,;:\/\.\!\*\&\?\#\$\=\+@-]+" "$file")
    do
        # Read the HTTP header, then extract the second field (status code)
        status_code=$(curl -Is "$url" | head -n 1 | awk '{print $2}')
        # Print the status of the URL
        if echo $status_code | grep -E '^(2.*.*|3.*.*)' > /dev/null; then
            echo -ne "\e[1;32mOK  \e[0m"
        else
            echo -ne "\e[0;31mBAD \e[0m"
            let bad_count++
        fi
        # Print the URL
        echo "$url"
    done
done

echo "END."; echo "Bad URLs: $bad_count"
if [ "$bad_count" != 0 ]; do exit 1; done
