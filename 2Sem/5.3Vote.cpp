// 5.3  Имеется список результатов голосования избирателей за несколько партий, в виде списка названий данных 
//      партий. На вход программе в первой строке подается количество избирателей в списке N. В каждой из последующих N 
//      строк записано название партии, за которую проголосовал данный избиратель, в виде текстовой строки. Длина строки 
//      не превосходит 50 символов, название может содержать буквы, цифры, пробелы и прочие символы.



#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "const.h"

struct party {
	char name[80];
	int voices;
};

int strToInt(char* str) {
	int i, res = 0;
	for (i = 0; str[i] >= '0' && str[i] <= '9'; i++) {
		res = res * 10 + str[i] - '0';
	}

	return res;
}

int party_in(char* party_name, party* parties, int n) {
	for (int i = 0; i < n; i++) {
		if (!strcmp(parties[i].name, party_name)) return i;
	}

	return -1;
}

int readVotes(party* parties, FILE* votes, int number_of_votes) {
	char party_name[80];
	int difParties = 0;
	int k;

	for (int i = 0; i < number_of_votes; i++) {
		fgets(party_name, 80, votes);
		party_name[strlen(party_name) - 1] = '\0';

		k = party_in(party_name, parties, difParties);
		if (k >= 0) {
			parties[k].voices++;
		}
		else {
			strcpy(parties[difParties].name, party_name);
			parties[difParties].voices = 1;
			difParties++;
		}
	}

	return difParties;
}

void insSortPatries(party* parties, int n) {
	party b; int j;
	for (int i = 1; i < n; i++) {
		b = parties[i];
		j = i - 1;
		while (j >= 0 && parties[j].voices < b.voices) {
			parties[j + 1] = parties[j];
			j--;
		}
		parties[j + 1] = b;
	}
}

int main() {
	FILE* votes = fopen("vote.txt", "r");
	char temp[20];
	fgets(temp, 20, votes);
	int number_of_votes = strToInt(temp);
	party* parties = new party[number_of_votes];
	int number_of_parties = readVotes(parties, votes, number_of_votes);
	insSortPatries(parties, number_of_parties);
	for (int i = 0; i < number_of_parties; i++) {
		printf("%s %d\n", parties[i].name, parties[i].voices);
	}
	delete[] parties;
	fclose(votes);

	return 0;
}