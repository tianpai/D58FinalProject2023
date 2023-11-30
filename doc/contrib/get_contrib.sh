#!/bin/bash

# Function to generate commit logs for an author and calculate totals
generate_commit_logs() {
	local author_name=$1
	local file_name=$2

	# Clear out the file for initial command
	# Including indicators for additions (+) and deletions (-)
	git log --author="$author_name" --numstat --pretty=format:"Commit: %h" |
		awk '{if (NF==3) print $1 " additions(+), " $2 " deletions(-), Commit: " $3; else print $0;}' >"$file_name"

	# Append the detailed commit messages
	git log --author="$author_name" --pretty=format:"%h - %an, %ad : %s" >>"$file_name"

	# Calculate and display total number of additions and deletions
	local total_additions=$(git log --author="$author_name" --numstat --pretty="%H" |
		awk '{if (NF==3) additions += $1} END {print additions}')
	local total_deletions=$(git log --author="$author_name" --numstat --pretty="%H" |
		awk '{if (NF==3) deletions += $2} END {print deletions}')

	echo -e "\n" >>"$file_name"
	echo "Total for $author_name: $total_additions additions, $total_deletions deletions" >>"$file_name"
}

# Generate commit logs for each author
generate_commit_logs "Tianpai Z" "tianpai_contrib.txt"
generate_commit_logs "In Kim" "in_contrib.txt"
generate_commit_logs "Jeffrey He" "jeffrey_contrib.txt"
