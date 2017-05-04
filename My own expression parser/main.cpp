//
//  main.cpp
//  My own expression parser
//
//  Created by  Roman Usherenko on 03.05.17.
//  Copyright © 2017 Kushka Misha & Yazenok Misha. All rights reserved.
//

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cmath>
#include <stdio.h>
using namespace std;

list<char> ShuntingAlgorithm(string s)
{
    /**
     * The shunting algorithm
     */
    list<char> stack;
    list<char> queue;
    string c = "";
    map<char, int> precedence = {
        {'-', 1}, {'+', 1},
        {'*', 2}, {'/', 2},
        {'^', 3}
    };
    
    for (int i = 0; i < s.length(); ++i)
    {
        if (isdigit(s[i]))
            // It's a number
            queue.push_back(s[i]);
        else
        {
            if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^')
            {
                // It's an operator
                while (!stack.empty())
                {
                    if (precedence[stack.back()] > precedence[s[i]])
                    {
                        queue.push_back(stack.back());
                        stack.pop_back();
                    }
                    else
                        break;
                }
                stack.push_back(s[i]);
            }
            else
            {
                if (s[i] == '(')
                    stack.push_back('(');
                if (s[i] == ')')
                {
                    while (!stack.empty())
                    {
                        if (stack.back() != '(')
                        {
                            queue.push_back(stack.back());
                            stack.pop_back();
                        }
                        else
                        {
                            stack.pop_back();
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // While there's operators on the stack, pop them to the queue
    while (!stack.empty())
    {
        queue.push_back(stack.back());
        stack.pop_back();
    }
    
    return queue;
}

double ReversePolish(list<char> queue)
{
    /**
     * Reverse polish
     */
    double a, b, result = 0;
    list<double>fstack;
    
    for (list<char>::iterator p = queue.begin(); p != queue.end(); ++p)
    {
        if (isdigit(*p))
            fstack.push_back(*p - '0');
        else
        {
//            cout << "before: ";
//            copy(fstack.begin(), fstack.end(), ostream_iterator<double>(cout, " "));
//            cout << endl;
            a = fstack.back();
            fstack.pop_back();
            b = fstack.back();
            fstack.pop_back();
//            cout << "a = " << a << endl;
//            cout << "b = " << b << endl;
            switch (*p)
            {
                case '+':
                    result = b + a;
                    break;
                case '-':
                    result = b - a;
                    break;
                case '*':
                    result = b * a;
                    break;
                case '/':
                    result = (double) b / a;
                    break;
                case '^':
                    result = (double) pow((double) b, a);
                    break;
            }
//            cout << "result = " << result << endl;
            fstack.push_back(result);
//            cout << "after: ";
//            copy(fstack.begin(), fstack.end(), ostream_iterator<double>(cout, " "));
//            cout << endl;
        }
    }
//    copy(fstack.begin(), fstack.end(), ostream_iterator<double>(cout, " "));
//    cout << endl;
    
    return fstack.back();
}

int main() {
    // Input data
    string s = "";
    cout << "Enter expression: ";
    cin >> s;
    
    list<char>queue = ShuntingAlgorithm(s);
    copy(queue.begin(), queue.end(), ostream_iterator<char>(cout, " "));
    cout << endl;
    double answer = ReversePolish(queue);
    cout << "Answer: " << answer << endl;
    
}

// 4*8/(9-5)
// 6^2-9*2/3
