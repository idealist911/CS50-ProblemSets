# THIS PROGRAM ASKS USER FOR SENTENCES OF TEXT
# AND OUTPUTS THE GRADE LEVEL FOR THE TEXT
# CALCULATED USING THE COLEMAN-LIAU FORMULA

# LIBRARIES
import re


def main():
    # Get text from user
    text = input("Text: ")

    # Calculate CL index
    index = get_CLindex(text)
    ind_int = round(index)

    # Returns grade level
    if (ind_int < 1):
        print("Before Grade 1")
    elif (ind_int >= 16):
        print("Grade 16+")
    else:
        print(f"Grade {ind_int}")


def get_CLindex(text):
    # Get L: number of letters per 100 words
    num_words = count_words(text)
    num_letters = count_letters(text)
    L = (num_letters / num_words) * 100

    # Get S: number of sentences per 100 words
    num_sentences = count_sentences(text)
    S = (num_sentences / num_words) * 100

    # CL index
    return 0.0588 * L - 0.296 * S - 15.8


def count_words(text):
    words = text.split()
    return len(words)


def count_letters(text):
    count = 0
    for letter in text:
        if re.match('\w', letter):
            count += 1
    return count


def count_sentences(text):
    count = 0
    for letter in text:
        if re.match('[.!?]', letter):
            count += 1
    return count


# Execute main()
if __name__ == '__main__':
    main()
