#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct Node
{
	struct RecordType data;
	struct Node* next;
};

struct HashType
{
	struct Node** hashArray;
	int size;
};

// Compute the hash function
int hash(int x, int tableSize)
{
	return x % tableSize;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....

void insertRecord(struct HashType* pHashArray, int id, char name, int order)
{
	int hashValue = hash(id, pHashArray->size);

	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	if (newNode == NULL)
	{
		printf("Memory allocation failed for a new node.\n");
		exit(-1);
	}

	newNode->data.id = id;
	newNode->data.name = name;
	newNode->data.order = order;
	newNode->next = NULL;

	newNode->next = pHashArray->hashArray[hashValue];
	pHashArray->hashArray[hashValue] = newNode;
}

void displayRecordsInHash(struct HashType* pHashArray)
{
	printf("\nRecords in the hash table:\n");
	for (int i = 0; i < pHashArray->size; ++i)
	{
		printf("Index %d -> ", i);
		struct Node* currentNode = pHashArray->hashArray[i];
		while (currentNode != NULL)
		{
			printf("%d %c %d -> ", currentNode->data.id, currentNode->data.name, currentNode->data.order);
			currentNode = currentNode->next;
		}
		printf("NULL\n");
	}
	printf("\n");
}


int main(void)
{
	struct RecordType* pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	int hashSize = recordSz * 2;
	struct HashType hashTable;
	hashTable.size = hashSize;
	hashTable.hashArray = (struct Node**)malloc(sizeof(struct Node*) * hashSize);
	if (hashTable.hashArray == NULL)
	{
		printf("Memory allocation failed for the hash table.\n");
		exit(-1);
	}
	for (int i = 0; i < hashSize; ++i)
		hashTable.hashArray[i] = NULL;

	for (int i = 0; i < recordSz; ++i)
	{
		insertRecord(&hashTable, pRecords[i].id, pRecords[i].name, pRecords[i].order);
	}

	displayRecordsInHash(&hashTable);

	for (int i = 0; i < hashSize; ++i)
	{
		struct Node* currentNode = hashTable.hashArray[i];
		while (currentNode != NULL)
		{
			struct Node* temp = currentNode;
			currentNode = currentNode->next;
			free(temp);
		}
	}
	free(hashTable.hashArray);
	free(pRecords);

	return 0;
}
