# THIS PROGRAM READS A CSV FILE CONTAINING A DATABASE OF PEOPLE
# AND A TEXT FILE CONTAINING A DNA SEQUENCE.
# FROM THE TWO FILES, THE PROGRAM MATCHES THE DNA SEQUENCE TO THE PERSON (IF ANY)

# Usage: python dna.py [data.csv] [sequence.txt]
# inputs:
# 1. csv - Rows: people. Columns: DNA STR sequences.
#           first column: names. subsequent columns: STR count of the STR for the person
# 2. txt - A long string of characters (ATCG) representing a DNA sequence. No return chars.

# LIBRARIES
import csv  # for reading csv files
import magic  # for determining file type
import sys  # for argv and exit code


def main():
    # Ensure proper usage
    error = usage_error()
    if error:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    csvfilename = sys.argv[1]
    txtfilename = sys.argv[2]

    # Read csv file and return database
    database = read_csv(csvfilename)

    # Read txt file and return sequence
    sequence = read_txt(txtfilename)

    # Create dictionary containing counts for each STR in the sequence
    strdict = create_dict(sequence, database)

    # Identify person with matching STR counts
    person = compare(strdict, database)
    print(person)


# Ensure proper usage
def usage_error():
    # error identifier
    error = False

    # check for correct number of arg
    if (len(sys.argv) != 3):
        error = True

    # check for correct file types
    if (magic.from_file(sys.argv[1], mime=True) != 'application/csv'):
        error = True
    if (magic.from_file(sys.argv[2], mime=True) != 'text/plain'):
        error = True

    if error:
        return True
    else:
        return False


# Read csv file and return a list of dictionaries
def read_csv(filename):
    # Create a list to store each person's data as a dictionary
    database = []

    with open(filename) as file:
        reader = csv.DictReader(file)

        # write each row as a dictionary into the database
        for row in reader:
            database.append(row)

    return database


# Read txt file and return sequence
def read_txt(filename):
    # Create a list to write each line
    sequences = []

    with open(filename) as file:
        # Write each line into the list
        for line in file:
            sequences.append(line)

    # there is only one line in file
    return sequences[0]


# Create dictionary containing counts for each STR in the sequence
def create_dict(sequence, database):
    person = database[0]

    sequencelength = len(sequence)
    strdict = {}
    # Access first person in database to identify STR counts in sequence
    for strdna in person:
        count = 0
        maxcount = 0
        if (strdna != 'name'):
            # Get STR length
            strlength = len(strdna)
            for i in range(sequencelength - strlength):
                count = 0
                if sequence[i:i + strlength] == strdna:
                    # Get next set of string to see if it matches again and again
                    while(sequence[i: i + strlength] == strdna and i + strlength <= sequencelength):
                        count += 1
                        i += strlength
                    if count > maxcount:
                        maxcount = count

            strdict[strdna] = str(max(count, maxcount))

    return strdict


# Identify person with matching STR counts
def compare(strdict, database):
    for person in database:
        match = 0
        for strdna in person:
            if (strdna != 'name'):
                if person[strdna] == strdict[strdna]:
                    match += 1
        if match == len(strdict):
            return person['name']

    return 'No match'


if __name__ == '__main__':
    main()