#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// PROTOTYPE
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float get_num_per100(int numitem, int numwords);
float get_CLindex(float L, float S);

/* Purpose: From user input of a paragraph of text,
 *          identify the reading level of the paragraph
 *          using a readability test called Coleman-Liau index.
 */

int main(void)
{
    // Get text from user input
    string text = get_string("Text: ");
    int num_letters = count_letters(text);
    int num_words = count_words(text);
    int num_sentences = count_sentences(text);

    //printf("%i letter(s)\n", num_letters);
    //printf("%i words(s)\n", num_words);
    //printf("%i sentences(s)\n", num_sentences);

    float letters_per100 = get_num_per100(num_letters, num_words);
    float sentences_per100 = get_num_per100(num_sentences, num_words);

    //printf("%f letters per 100 words\n", letters_per100);
    //printf("%f sentences per 100 words\n", sentences_per100);

    int index = round(get_CLindex(letters_per100, sentences_per100));

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int num = 0; // number of letters

    for (int i = 0, length = strlen(text); i < length; i++)
    {
        int small = ((text[i] >= 'a') && (text[i] <= 'z')) ? 1 : 0;
        int capital = ((text[i] >= 'A') && (text[i] <= 'Z')) ? 1 : 0;

        if (capital || small)
        {
            num++;
        }
    }

    return num;
}

int count_words(string text)
{
    int num = 0; // number of words
    int prevchar = ' ';

    for (int i = 0, length = strlen(text); i < length; i++)
    {
        int small = ((prevchar >= 'a') && (prevchar <= 'z')) ? 1 : 0;
        int capital = ((prevchar >= 'A') && (prevchar <= 'Z')) ? 1 : 0;
        int letters = (small || capital);
        int punctuation = ((text[i] == '.') || (text[i] == '!') || (text[i] == '?') || (text[i] == ',') || (text[i] == ';')
                           || (text[i] == ':')) ? 1 : 0;

        int countwords = ((text[i] == ' ' || punctuation) && letters) ? 1 : 0;

        if (countwords)
        {
            num++;
        }

        prevchar = text[i];
    }

    return num;
}

int count_sentences(string text)
{
    int num = 0; // number of sentences

    for (int i = 0, length = strlen(text); i < length; i++)
    {
        int period = ((text[i] == '.') || (text[i] == '!') || (text[i] == '?')) ? 1 : 0;

        if (period)
        {
            num++;
        }
    }

    return num;
}

float get_num_per100(int numitem, int numwords)
{
    // Purpose: find number of items per 100 words
    // int numitem: number of letters or sentences (depending on intent)
    // int numwords: number of words

    float average = numitem / (float) numwords;
    float num_per100 = average * 100;

    return num_per100;
}

float get_CLindex(float L, float S)
{
    // int L: average number of letters per 100 words
    // int S: average number of sentences per 100 words

    return (0.0588 * L - 0.296 * S - 15.8);
}