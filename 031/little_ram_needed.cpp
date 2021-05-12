// COMPILE WITH THIS COMMAND IN LINUX: OTHERWISE YOU CAN*T TAKE ADVANTAGE OF MORE THAN 2GB OF RAM
// g++ little_ram_needed.cpp -mcmodel=large -o noram
// and run it with this command
// ./noram

/*
Haskell version of the algorithm:
coinCombinations :: Int -> [Int] -> Int
coinCombinations cents coinList
  | cents == 0 = 1
  | coinList == [] = 0
  | cents < (head coinList) = coinCombinations cents (tail coinList)
  | True = coinCombinations (cents - head coinList) coinList + coinCombinations cents (tail coinList)

coinCombinations 200 [200, 100, 50, 20 , 10, 5, 2, 1]
*/

// this algorithm remembers all the values that it has calculated already
// if you ask it for example coinCombinations(10000000, 0) it will take a few seconds
// the next time you run coinCombinations(10000000, 0) it will see that in the combinations[10000000][0] there exists a calculated value and doesn't calculate it again and returns the stored value instead
// the values are stored in a two dimensional array of 128 unsigned integers
// this way our algorithm will run in linear time mening that for example if you give it 1000 bigger values, it will take 1000 times as long
// this is a huge improvement with the version of this algorithm that calculates the same results over and over again that runs in exponential time

#include <iostream>
#include <string>

using namespace std;

const int coinArray1[] = {200, 100, 50, 20, 10, 5, 2, 1};
const int coinArraySize1 = 8;

const int coinArray2[] = {100, 50, 25, 10, 5, 1};
const int coinArraySize2 = 6;

const int coinArray3[] = {200, 100, 50, 20, 10, 5, 3};
const int coinArraySize3 = 7;

const int *coinArray;
int coinArraySize;

const int combinationsSize = 4096;                       // has to be a multiple of 4 and at least 4 times bigger than you biggest coin
const int maxCoinArraySize = 8;
__uint128_t combinations[combinationsSize][maxCoinArraySize];
__uint128_t empty = -1;

__uint128_t newResult(long long int cents, long long int index, __uint128_t value)
{
    combinations[cents % combinationsSize][index] = value;
    return value;
}

__uint128_t coinCombinations(long long int cents, long long int coinIndex)
{

    if (cents % (combinationsSize / 4) == 0 && coinIndex == coinArraySize - 1)
    {
        for (int i = (cents + combinationsSize / 4 * 1) % combinationsSize; i <= (cents + combinationsSize / 4 * 2 - 1) % combinationsSize; i++)
        {
            for (int j = 0; j < maxCoinArraySize; j++) combinations[i][j] = empty;
        }
    }
    
    if (combinations[cents % combinationsSize][coinIndex] != empty) return combinations[cents % combinationsSize][coinIndex];
    
    if (cents <= 0)
    {
        return newResult(cents, coinIndex, 1);
    }

    if (coinIndex >= coinArraySize)
    {
        return 0;
    }

    if (coinIndex == coinArraySize - 1)
    {
        if (cents % coinArray[coinIndex] == 0) return newResult(cents, coinIndex, 1);
        else return newResult(cents, coinIndex, 0);
    }

    if (cents < coinArray[coinIndex]) return newResult(cents, coinIndex,
                coinCombinations(cents, coinIndex + 1));

    return newResult(cents, coinIndex,
                coinCombinations(cents - coinArray[coinIndex], coinIndex)
                + coinCombinations(cents, coinIndex + 1));
}

string __128intToString(__uint128_t bigNumber)
{
    if (bigNumber <= 0) return "";
    int smallNumber = bigNumber % 10;
    return __128intToString(bigNumber / 10) + to_string(smallNumber);
}

int main()
{
    string input = "placeholder";

    cout << "Which coinset will you choose?\n";
    cout << "1: ";
    for (int i = 0; i < coinArraySize1; i++)
    {
        cout << " [" << coinArray1[i] << "]";
    }
    
    cout << "\n2: ";
    for (int i = 0; i < coinArraySize2; i++)
    {
        cout << " [" << coinArray2[i] << "]";
    }
    
    cout << "\n3: ";
    for (int i = 0; i < coinArraySize3; i++)
    {
        cout << " [" << coinArray3[i] << "]";
    }
    
    cout << "\n";
    getline (cin, input);
    if (input == "2")
    {
        coinArray = coinArray2;
        coinArraySize = coinArraySize2;
    }
    else if (input == "3")
    {
        coinArray = coinArray3;
        coinArraySize = coinArraySize3;
    }
    else
    {
        coinArray = coinArray1;
        coinArraySize = coinArraySize1;
    }

    empty = -1;
    // we print these here to make sure that the number we use as a marker for value in the combinations array not yet counted is the biggest possible 128 bit unsigned integer

    while (input != "exit")
    {
        for (long long int i = 0; i < combinationsSize; i++)
            for (long long int j = 0; j < coinArraySize; j++)
                combinations[i][j] = empty;
        cout << "\nHow many cents do you have?\n";
        getline (cin, input);
        long long int cents = stoll(input);
	cout << "\nCoin list index 0 - " << coinArraySize - 1 << " (hint: 0)\n";
        getline (cin, input);
	int coinIndex = stoi(input);
        for (long long int i = 10; i <= cents; i++)              // i put this loop here because calling coinCombinations directly with these huge numbers made the program run out of stack
        {
	    if (coinCombinations(i, 0) < coinCombinations(i - 1, 0)
                && coinCombinations(i - 1, 0) > empty / 4 * 3 && coinCombinations(i, 0) < empty / 2)        // this test is here to alert us if our 128 bit unsigned integer has overflows
	    {
                cents = i - 1;
            }
        }
        cout << "\n" << cents << " cents can be made with "
             << __128intToString(coinCombinations(cents, coinIndex)) << " coin combinations\n";
    }

    cout << "\nDone counting coins!\n";
    return 0;
}

