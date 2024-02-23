#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DICTIONARY_SIZE 1000
#define MAX_WORD_LENGTH 100

// Node for hash table
struct Node {
    char word[MAX_WORD_LENGTH];
    struct Node *next;
};

// Hash table structure
struct HashTable {
    struct Node *table[DICTIONARY_SIZE];
};

// Hash function
unsigned int hash(const char word[]) {
	int i;
    unsigned int hashValue = 0;
    for (i = 0; i<strlen(word); i++) {
        hashValue = (hashValue << 5) + tolower(word[i]);
    }
    //printf("\n%d",hashValue % DICTIONARY_SIZE);
    return hashValue % DICTIONARY_SIZE;
}

// Load words from a dictionary file into a hash table
void loadDictionary(struct HashTable *hashTable, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
	{
        perror("Error opening dictionary file");
        exit(1);
    }

    char buffer[MAX_WORD_LENGTH];

    while (fgets(buffer, sizeof(buffer), file)) 
	{
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') 
		{
            buffer[len - 1] = '\0'; // Remove newline character
        }

        unsigned int index = hash(buffer);
        struct Node *node = (struct Node *)malloc(sizeof(struct Node));
        strcpy(node->word, buffer);
        node->next = hashTable->table[index];
        hashTable->table[index] = node;
    }
    fclose(file);
}

// Check if a word is in the dictionary using hash table
int isWordInDictionary(struct HashTable *hashTable, char word[]) {
    unsigned int index = hash(word);
    const struct Node *current = hashTable->table[index];
    while (current!=NULL) {
        if (strcasecmp(current->word, word) == 0) {
            return 1; // Found in dictionary
        }
        current = current->next;
    }
    return 0; // Not found in dictionary
}

int min(int a,int b)
{
	if(a<b)
		return b;
	else
		return a;
}

// Calculate Levenshtein distance between two strings
int levenshteinDistance(const char *s1, const char *s2) {
	int i,j,k;
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int dp[len1 + 1][len2 + 1];

    for (i = 0; i <= len1; i++) {
        for (j = 0; j <= len2; j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
            	//int k;
            	k=min(dp[i - 1][j], dp[i][j - 1]);
                dp[i][j] = 1 + min(k, dp[i - 1][j - 1]);
            }
        }
    }

    return dp[len1][len2];
}

int main() {
	int i;
    const char dictionaryFilename[50] = "ukenglish.txt";
    char input[MAX_WORD_LENGTH];
    struct HashTable hashTable;
    for (i = 0; i < DICTIONARY_SIZE; i++) {
        hashTable.table[i] = NULL;
    }
    loadDictionary(&hashTable, dictionaryFilename);

    
    printf("Enter a word: ");
    scanf("%s", input);

    // Convert input to lowercase
    for (i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }

    if(isWordInDictionary(&hashTable, input))
        printf("The word '%s' is spelled correctly.\n", input); 
	else 
	{
        printf("The word '%s' is not found in the dictionary.\n", input);
        // Suggest corrections using Levenshtein distance
        printf("Suggestions:\n");
        unsigned int i;
        for (i = 0; i < DICTIONARY_SIZE; i++) {
            const struct Node *current = hashTable.table[i];
            while (current) {
                if (levenshteinDistance(input, current->word) <= 2) {
                    printf("- %s\n", current->word);
                }
                current = current->next;
            }
        }
    }


    return 0;
}
// Implementation of Levenshtein distance

