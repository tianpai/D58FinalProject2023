#!/bin/bash

# Function to calculate changes for a specific author
calculate_author_changes() {
	local author=$1
	local directory=$2
	git log --numstat --pretty="%H" --author="$author" -- "$directory" |
		awk '{if (NF == 3) {additions += $1; deletions += $2}} END {print additions, deletions}'
}

# Function to calculate total changes in the directory
calculate_total_changes() {
	local directory=$1
	git log --numstat --pretty="%H" -- "$directory" |
		awk '{if (NF == 3) {additions += $1; deletions += $2}} END {print additions, deletions}'
}

# Function to append contributions to the report
append_contributions() {
	local author=$1
	local author_alias=$2
	local directory=$3
	local dir_name=$4
	local output_file=$5

	# Calculate author's changes
	read author_additions author_deletions <<<$(calculate_author_changes "$author" "$directory")
	if [ -n "$author_alias" ]; then
		read alias_additions alias_deletions <<<$(calculate_author_changes "$author_alias" "$directory")
		author_additions=$((author_additions + alias_additions))
		author_deletions=$((author_deletions + alias_deletions))
	fi

	# Calculate total changes in the directory
	read total_additions total_deletions <<<$(calculate_total_changes "$directory")

	# Calculate contribution percentage
	author_total_changes=$((author_additions + author_deletions))
	total_changes=$((total_additions + total_deletions))
	contribution_percentage=$(awk "BEGIN {print ($author_total_changes / $total_changes) * 100}")

	# Output commit details and contribution percentage
	echo "$author's Contribution Report for $dir_name" >>"$output_file"
	echo "-------------------------------------------------" >>"$output_file"
	git log --numstat --pretty="Date: %ad%n%s" --author="$author" -- "$directory" >>"$output_file"
	if [ -n "$author_alias" ]; then
		git log --numstat --pretty="Date: %ad%n%s" --author="$author_alias" -- "$directory" >>"$output_file"
	fi
	echo "" >>"$output_file"
	echo "Total Additions by $author: $author_additions" >>"$output_file"
	echo "Total Deletions by $author: $author_deletions" >>"$output_file"
	echo "Total Changes in $dir_name: Additions: $total_additions, Deletions: $total_deletions" >>"$output_file"
	printf "Contribution Percentage: %.2f%%\n" $contribution_percentage >>"$output_file"
	echo "" >>"$output_file"
}

# Directories and output file
doc_directory="../doc/"
src_directory="../src/"
output_file="contrib_report.txt"

# Clear the output file
>"$output_file"

# Append contributions for each author and directory
append_contributions "Tianpai Z" "" "$doc_directory" "doc" "$output_file"
append_contributions "Tianpai Z" "" "$src_directory" "src" "$output_file"
append_contributions "In Kim" "kmstr87" "$doc_directory" "doc" "$output_file"
append_contributions "In Kim" "kmstr87" "$src_directory" "src" "$output_file"
append_contributions "Jeffrey He" "" "$doc_directory" "doc" "$output_file"
append_contributions "Jeffrey He" "" "$src_directory" "src" "$output_file"

# Display the report
cat "$output_file"
