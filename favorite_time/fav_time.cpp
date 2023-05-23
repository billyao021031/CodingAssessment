/* 
Problem 1: Finding favorite time
Answered By: Bill Yao

In this question, we were given an integer within the ranger of 0 to 1,000,000,000 to represent the duration of clock running time
Then, we are expected to output the number of times the clock displays an arithmetic sequence time. 

Therefore we need three functions to resolve this question:

1). main() --> get inputs from the command line and output the results to the command line
2). isArithmetic() --> this function helps determine if a specific time xx:xx is in the format of an arithmetic sequence
3). countArithmetic --> this function helps determine the amount of times the clock hits arithmetic sequence time during the given period

However, I did think about the running time of this question to be O(n), which was too inefficient when we are given a huge input. If we can, instead of looping through every signle minute,
loop through each arithmetic-sequence time and  check if it's still within the given period, we might be able to change the running time.
*/

#include <string>
#include <vector>
#include <iostream>

using namespace std;

/* 
In this boolean function, it is easy to figure out if a specific time is in arithmetic sequence. 
This can be easily done by dividing the time into single digit and compare the difference from the first to the last digit.
*/
bool isArithmetic(const string& period) {
    vector<int> digits;
    for (char c : period) {
        if (isdigit(c)) {
            digits.push_back(c - '0'); //make sure the char turns into integer
        }
    }

    int difference = digits[1] - digits[0];
    for (size_t i = 2; i < digits.size(); i++) {
        if (digits[i] - digits[i - 1] != difference) {
            return false;
        }
    }

    return true;
}


/*
In this int function, we have three important questions to resolve:
1). How to make sure the time starts from 12 instead of 0;
2). How to make sure the time loop back to 12 once it hits 11:59;
3). How to construct a proper time string. 
*/
int countArithmetic(int D) {
    int count = 0;
    for (int minute = 0; minute <= D; ++minute) {
        int hour = (minute / 60) % 12; //make sure the time loop back to 12
        int minuteValue = minute % 60;
        string time = to_string((hour == 0) ? 12 : hour) + ":" + (minuteValue < 10 ? "0" : "") + to_string(minuteValue); //make sure the time is in 12-hour format
        
        if (isArithmetic(time)) {
            count++;
        }
    }
    return count;
}

//main function used for getting and outputting in the command line
int main() {
    int D;  
    while(D >= 0 && D <= 1000000000){
        cin >> D;
        cout << countArithmetic(D) << endl;
    }
    return 0;
}

