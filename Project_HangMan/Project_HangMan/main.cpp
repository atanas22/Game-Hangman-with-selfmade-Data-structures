#include <iostream>
#include <ctime>
#include <fstream>
#include <set>
#include "Set.h"
#include <queue>
#include <list>
using namespace std;
// data structure course project 2019

/* With vector data strurcure for storing the words the program would work fastest but I decided to make AVL tree to show
   that I can make much complexier data structure than vector structure */

// ----------

Set<string> words; // English words
vector<list<short>> lettersInWord(127);// lettersInWord съхранява всички букви на рандом думата(word), както и индексите, на които се намират те в word
bool chosenLetters[127]; /* тук съхраняваме буквите, които потребителят е въвел, за да не може да ги въведе отново 
						 - а ако въведе отново някоя от тях - не се брои за грешка*/
void playHangman(const string &w); 
void hangman(); 

int main() {
	srand(time(0));
	ifstream in("words.txt");
	if (!in.good()) {
		cout << "error trying to open the file 'words.txt'\n";
		return 0;
	}
	string wrd;
	cout << "Initializing the game..\n";
	while (in >> wrd)  words.insert(wrd);
	cout << "Initialization completed. Enjoy the game! :)\n\n";
	hangman(); // пускане на играта
}


// Definitions
bool troll = true;
void playHangman(const string &w) { // w - рандом избраната дума
	// cout << "word: " << w << "\n\n"; //казва коя е буквата преди да започне играта :) 
	short wordSz = w.size(), maxWrongGuesses = 6;
	string word(wordSz, '-');
	cout << "Guess the word:\n";
	cout << word << "\n";

	short wrongCnt = 0, // брояч за грешните опити
		letterCnt = 0; // брояч за  познатите букви - когато е равен на wordSz - тогава потребителят е познал думата
	char letter = ' ';
	while (letterCnt < wordSz && wrongCnt < maxWrongGuesses) { // ако все още не е познал думата И все още не е направил 6 грешки, тогава..
		cin >> letter;
		if (chosenLetters[short(letter)]) { cout << "\nThe letter has already been chosen!\n" << word << "\n"; continue; }
		if (lettersInWord[(short)letter].size()) { // if size > 0 - има такава буква в думата,
			// в lettersInWord[letter] се съхраняват индексите на тази буква в думата - с цикъла взимаме индексите, на които се намират
			//в думата и на тези индекси се записва буквата в word
			for (int szLetters = lettersInWord[(short)letter].size(), i = 0; i < szLetters; ++i) {
				++letterCnt;// отчитаме познатите букви
				word[lettersInWord[(short)letter].front()] = letter; // записваме в word на съответните индекси познатата букава (letter)
				lettersInWord[(short)letter].pop_front();
			}
			chosenLetters[short(letter)] = true; // отчитаме, че буквата letter е използвана, за да не се работи с нея по-нататък, като потребителят се информира
			cout << "\nCorrect letter\n" << word << "\n";
		}
		else { // противен случаи няма такава буква 
			++wrongCnt; // и отброяв за грешка
			chosenLetters[short(letter)] = true; // на индека на буквата се отчита, че е използвана
			if (maxWrongGuesses - wrongCnt) cout << "\nWrong attempts left: " << maxWrongGuesses - wrongCnt << "\n" << word << "\n";
		}
	}
	if (wrongCnt == maxWrongGuesses) cout << "\nYou are hanged! haha :D\n" << "The word was: \"" << w << "\"\n";
	else if (rand() % 2) { cout << "\nCongratulations! You guessed the word!\n"; troll = false; }
	else cout << "\nWell, you guessed the word, but I will hang you anyway!! >=D\n"; // малко базик :))
	for (int i = 0; i < wordSz; ++i) lettersInWord[(short)w[i]].clear(); // нулираме контейнера, който съдържа, съдържащите се букви на word, както и техните индекси в word
	for (int i = 0; i < 127; ++i) chosenLetters[i] = false; // нулираме и въведените от потребителя букви
}

void hangman() {
	cout << "Welcome to Hangman!\n"
		<< "Guess the word, as you try to enter the correct letters, in order to win.\n"
		<< "You have chance of 6 wrong guessed letters.\n\n";
	char cont = ' ';
	do {
		string word = *words.getRandElement(); // взима произволна дума от words
		while (word.size() == 0) word = *words.getRandElement(); // сред думите има празен/празни символ/и. Отново се инициализира, за да се вземе правилна дума
		for (int sz = word.size(), i = 0; i < sz; ++i)
			lettersInWord[(short)word[i]].push_back(i); // в lettersInWord съхранява всички букви на word, както и индексите, на които се намират те в word - с цел малко увеличаване на бързодействието на програмара, вместо да обхожда всеки път цялата дума, за да види дали дадена буква се съдържа в думата и евентуално да я въведе, че я има. Няма голяма разлика в бързодействието в нашия сличай, но ако редиците от думи са с дължина стотитици или хиляди - тогава ще има голяма разлика. Така че в този случай без проблем може да се използва и 2-рия подход. Ако n(дълж. на дума) = 100 и ако приемем, че потребителят познава думата, тогава имаме ( О(3*100+127) или O(n), използвания подход) срешу O(100*100) (n^2) - 2-рия метод
		playHangman(word);
		cout << "\n\n\nPress 1 if you want to play again.\n";
		cin >> cont; cout << "\n";
	} while (cont == '1');
	cout << "Good bye"; if (troll) cout << ", Loser D: "; cout << "!\n";
}


/*
    PS: с вектор би станало много по-бързо избирането на рандом елемент,
	както и инициализацията на играта, но го правя по този начин(с направен от мен сет), 
	защото е по-интересно.
*/