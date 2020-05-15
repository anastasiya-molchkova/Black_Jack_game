/* Игра "БлэкДжэк" (казино-аналог "21"), переписанная с помощью классов
*/

#include <iostream>
#include <string>    // для std::string
#include <array>     // для std::array
//#include <algorithm> // для std::swap
#include <cstdlib>   // для функций выбора случайного числа srand() и rand()
#include <ctime>     // чтобы в randomize опираться на время запуска программы
#include <clocale>   // для вывода сообщений на кириллице 

using std::cout; using std::endl; using std::cin;
using std::string;

class Card
{
public:
	//перечисление мастей
	enum Suits
	{
		Diamonds, //буби
		Hearts,   //черви
		Spades,   //пики
		Clubs,    //крести
		suitsNumber //для обозначения количества мастей
	};

	//перечисление достоинств карт
	enum CardRanks
	{
		rank2,
		rank3,
		rank4,
		rank5,
		rank6,
		rank7,
		rank8,
		rank9,
		rank10,
		Jack,
		Queen,
		King,
		Ace,
		//Jocker,
		ranksNumber //для обозначения количества достоинств
	};
private:
	//переменные для представления карты (масть и достоинство):
	Suits m_suit;
	CardRanks m_rank;
public:
	//конструктор класса Card с инициализацией карт (переменных-членов класса). Укажите параметры по умолчанию для конструктора (используйте MAX_RANKS и MAX_SUITS).
	Card(Suits suit = suitsNumber, CardRanks rank = ranksNumber) : m_suit(suit), m_rank(rank)
	{}

	//printCard функция возвращает принимаемую как параметр карту в виде строки типа ДостоинствоМасть
	string printCard() const
	{
		string cardCode{};

		//сначала разбираемся с достоинствами карты
		switch (m_rank)
		{
		case rank2:
			cardCode = "2";
			break;
		case rank3:
			cardCode = "3";
			break;
		case rank4:
			cardCode = "4";
			break;
		case rank5:
			cardCode = "5";
			break;
		case rank6:
			cardCode = "6";
			break;
		case rank7:
			cardCode = "7";
			break;
		case rank8:
			cardCode = "8";
			break;
		case rank9:
			cardCode = "9";
			break;
		case rank10:
			cardCode = "10";
			break;
		case Jack:
			cardCode = "Валет";
			break;
		case Queen:
			cardCode = "Дама";
			break;
		case King:
			cardCode = "Kороль";
			break;
		case Ace:
			cardCode = "Туз";
			break;
		default:
			cardCode = "ErrorRank";
		}

		//теперь добавляем в строку масть
		switch (m_suit)
		{
		case Diamonds: //буби
			cardCode += "Буби";
			break;
		case Hearts:   //черви
			cardCode += "Черви";
			break;
		case Spades:   //пики
			cardCode += "Пики";
			break;
		case Clubs:    //крести
			cardCode += "Крести";
			break;
		default:
			cardCode += "ErrorSuit";
		}
		return cardCode;
	}

	//getCardValue возвращает значение карты (валет, королева или король — это 10, туз — это 11)
	unsigned short getCardValue() const
	{
		switch (m_rank)
		{
		case rank2:
		case rank3:
		case rank4:
		case rank5:
		case rank6:
		case rank7:
		case rank8:
		case rank9:
		case rank10:
			return (m_rank + 2); // двойка имеет 0 индекс в перечислении, 3 - первый и т.д.
		case Jack:
		case Queen:
		case King:
			return 10;
		case Ace:
			return 11;
		default:
			return 0;
		}
	}
};

class Deck
{
private:
	//для представления целой колоды карт (52 карты) создаём массив deck (используя std::array) из карт
	std::array <Card, Card::Suits::suitsNumber * Card::CardRanks::ranksNumber> m_deck;
	//вместо указателя на следующую карту в колоде:
	unsigned short m_cardIndex = 0;
public:
	// конструктор, который не принимает никаких параметров и инициализирует каждый элемент массива m_deck случайной картой  
	Deck()
	{
	    //инициализируем каждый элемент колоды определённой картой
		for (unsigned short suit = 0; suit < Card::Suits::suitsNumber; suit++)
			for (unsigned short rank = 0; rank < Card::CardRanks::ranksNumber; rank++)
			{
				//каждый элемент одномерного массива кодируем с помощью перечислителя мастей, перечислителя достоинств и максимального количества достоинств
			    //Внутри циклов создаём анонимный объект Card и присваиваем его каждому элементу массива m_deck
				m_deck.at(suit * Card::CardRanks::ranksNumber + rank) = Card(static_cast<Card::Suits>(suit), static_cast<Card::CardRanks>(rank));
			}
	}
	
	//функция выводит все значения (карты) из колоды
	void printDeck() const
	{
		//используем цикл foreach, обращаемся к каждой карте из колоды, печатаем её код
		for (const auto &card : m_deck)
			cout << card.printCard() << " ";
		cout << endl;
	}

	// открытый метод dealCard(), который будет возвращать константную ссылку на текущую карту
	const Card& dealCard()
	{
		return m_deck[m_cardIndex]; //указываем на текущую карту
	}
	// открытый метод goToTheNextCard(), который будет увеличивать m_cardIndex
	void goToTheNextCard()
	{
		m_cardIndex++; //сдвигаем указатель
	}

private:
	//функция выбора случайного числа между двумя заданными значениями
	static unsigned short getRandomNumber(unsigned short min, unsigned short max)
	{
		static const double fraction = 1.0 / static_cast<double>(RAND_MAX + 1.0);
		return static_cast<unsigned short>(rand() * fraction * (max - min + 1) + min);
	}
	//функция принимает две карты по ссылке и меняет местами их значения
	static void swap2Cards(Card& cardA, Card& cardB)
	{
		Card temporaryCard = cardA;
		cardA = cardB;
		cardB = temporaryCard;
	}
public:
	//функция для перетасовки колоды карт，в качестве параметра принимает массив колоды карт по ссылке
	void shuffleDeck()
	{
		//используем цикл for с итерацией по массиву. Перетасовка карт должна произойти 52 раза
		for (Card& cardFromDeck : m_deck)
		{
			//выбираем случайную карту из колоды (индекс от 0 до 51) и вызываем функцию замены двух карт для текущей карты и карты, выбранной случайным образом
			Card& theSecondCard = m_deck.at(getRandomNumber(0, (Card::Suits::suitsNumber * Card::CardRanks::ranksNumber - 1)));
			swap2Cards(cardFromDeck, theSecondCard);
		}
		cout << "Колода перетасована!" << endl;
		//после перетасовки колоды, раздаётся карта, которая является верхней
		m_cardIndex = 0;
	}
};

// возвращает истину, если игрок хочет сыграть ещё раз и ложь, если не хочет
bool wantsContinue()
{
	cout << endl;
	cout << "Хотите сыграть ещё раз? Нажмите 'y', если да или 'n', если нет ";
	string answer{ "" };
	cin.ignore(10000, '\n'); //чистим cin
	std::getline(cin, answer);
	while ((answer != "y") && (answer != "n"))
	{
		cout << "Ваш ответ не понятен! Хотите сыграть ещё раз? Нажмите 'y', если да или 'n', если нет ";
		std::getline(cin, answer);
	}
	cout << endl;
	if (answer == "y") return true;
	else return false;
}

//получаем внятный ответ от пользователя, что он хочет - взять карту или остановиться, возвращаем истину, если хочет взять и ложь - если остановиться
bool playerWantsHit()
{
	string answer{ "" };
	do
	{
		cout << "Взять карту - нажмите 1, удержаться - нажмите 2: ";
		cin >> answer;
		if (answer == "1")
			return true;
		if (answer == "2")
			return false;
	} while ((answer != "1") && (answer != "2"));
}

//Игра в БлэкДжэк, игроку нужно набрать меньше 21 очка и обойти дилера
void playBlackjack(Deck& deck)
{
	const unsigned short goal{ 21 };
	const unsigned short dealerLimit{ 17 };
	// создаём переменные для суммы очков у дилера и игрока
	int ganePlayer{ 0 }; int ganeDealer{ 0 };
	// и переменные для подсчёта количества тузов у игрока и дилера:
	unsigned short playerAcesNumber{ 0 };
	unsigned short dealerAcesNumber{ 0 };

	cout << "Итак, мы начинаем игру! Чтобы выиграть, нужно набрать не больше 21 очка и обойти дилера. Желаем удачи! " << endl;
	//дилеру в открытую раздаётся первая карта из перемешанной колоды:
	Card firstCard = deck.dealCard();
	cout << "У дилера " << firstCard.printCard() << ", его сумма " << (ganeDealer += firstCard.getCardValue()) << endl;
	//если карта дилера - туз, увеличиваем количество тузов дилера
	if (firstCard.getCardValue() == Card::CardRanks::Ace) dealerAcesNumber++;
	//одну карту "раздали", переходим на следующую карту
	deck.goToTheNextCard();
	
	//игроку раздаются следующие две карты из колоды:
	Card secondCard = deck.dealCard();
	deck.goToTheNextCard();
	Card thirdCard = deck.dealCard();
	cout << "У Вас " << secondCard.printCard() << " и " << thirdCard.printCard() << ", Ваша сумма ";
	//если карта - туз, увеличиваем количество тузов игрока
	if (secondCard.getCardValue() == Card::CardRanks::Ace) playerAcesNumber++;
	if (thirdCard.getCardValue() == Card::CardRanks::Ace) playerAcesNumber++;
	//считаем сумму:
	ganePlayer += ( secondCard.getCardValue() + thirdCard.getCardValue() );
	//считаем туз как 1 вместо 11, если сумма больше 21:
	if ((ganePlayer > goal) && (playerAcesNumber > 0))
	{
		ganePlayer -= 10;
		playerAcesNumber--;
	}
	cout << ganePlayer << endl;
	//две карты "раздали", переходим на следующую карту
	deck.goToTheNextCard();

	//Ход игрока, пока он не переберёт или пока не решит остановиться:
	while (ganePlayer <= goal)
	{
		// если хочет взять карту - даём следующую карту из колоды
		if (playerWantsHit() == true)
		{
			Card nextCard = deck.dealCard();
			cout << "У Вас " << nextCard.printCard() << ", Ваша сумма ";
			//если карта - туз, увеличиваем количество тузов игрока
			if (nextCard.getCardValue() == Card::CardRanks::Ace) playerAcesNumber++;
			//считаем сумму:
			ganePlayer += nextCard.getCardValue();
			//считаем тузы как 1 вместо 11, если сумма больше 21:
			while ((ganePlayer > goal) && (playerAcesNumber > 0))
			{
				ganePlayer -= 10;
				playerAcesNumber--;
			}
			cout << ganePlayer << endl;
			//одну карту "раздали", переходим на следующую карту
			deck.goToTheNextCard();
		}
		//если хочет остановиться, выходим из цикла while
		else break;
	}

	if (ganePlayer > goal) // если перебрал
		cout << "К сожалению, ВЫ ПРОИГРАЛИ, ведь у Вас больше 21 очка!" << endl;
	else // игрок остановился на сумме меньше 21:
	{
		//Ход дилера, пока не наберёт 17 или выше, он берёт очередную карту из колоды:
		while (ganeDealer < dealerLimit)
		{
			Card nextCard = deck.dealCard();
			cout << "У дилера меньше 17 очков, поэтому он берёт карту, он взял " << nextCard.printCard() << ", теперь его сумма ";
			//если карта дилера - туз, увеличиваем количество тузов дилера
			if (nextCard.getCardValue() == Card::CardRanks::Ace) dealerAcesNumber++;
			//считаем сумму:
			ganeDealer += nextCard.getCardValue();
			//считаем тузы как 1 вместо 11, если сумма больше 21:
			while ((ganeDealer > goal) && (dealerAcesNumber > 0))
			{
				ganeDealer -= 10;
				dealerAcesNumber--;
			}
			cout << ganeDealer << endl;
			//одну карту "раздали", переходим на следующую карту
			deck.goToTheNextCard();
		}

		// ходы окончены, разбираемся, кто победил
		if ((ganeDealer <= goal) && (ganePlayer < ganeDealer))
			cout << "К сожалению, ВЫ ПРОИГРАЛИ, так как дилер набрал больше очков!" << endl;
		else
		{
			if (ganeDealer == ganePlayer)
				cout << "Вот это да! У нас НИЧЬЯ! Вы с дилером набрали одинаковое количество очков!" << endl;
			else cout << "Поздравляем, ВЫ ВЫИГРАЛИ!" << endl; //дилер перебрал или его сумма меньше суммы игрока, при том, что оба не перебрали
		}
	}
}

int main()
{
	//аналог randomize с привязкой ко времени запуска:
	srand(static_cast<unsigned int>(time(0)));
	rand();

	// для вывода сообщений пользователю на кириллице
	setlocale(LC_CTYPE, "rus");

	//создаём колоду карт
	Deck deck;

	do //играем в БлэкДжэк с новой перемешанной колодой, пока игрок хочет играть
	{
		//перемешивание колоды:
		deck.shuffleDeck();
		//печать всей колоды:
		//deck.printDeck();
		playBlackjack(deck);
	} 
	while (wantsContinue() == true);

	return 0;
}
