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

list<string> ShuntingAlgorithm(string, double);
double ReversePolish(list<string>);
string ReplaceAll(string, const string&, const string&);
double f(string, double);

int main() {
    // Input function
    string func = "";
    cout << "Enter function: ";
    cin >> func;
    
    double x = -4;
    double answer = f(func, x);
    cout << "Answer = " << answer << endl;
    
}

double f(string s, double x)
{
    list<string>queue = ShuntingAlgorithm(s, x);
    
    copy(queue.begin(), queue.end(), ostream_iterator<string>(cout, " "));
    cout << endl;
    return ReversePolish(queue);
}

list<string> ShuntingAlgorithm(string s, double x)
{
    /**
     * The shunting algorithm
     */
    list<char> stack;
    list<string> queue;
    string prev = "o";
    string number;
    
    map<char, int> precedence = {
        {'-', 1}, {'+', 1},
        {'*', 2}, {'/', 2},
        {'^', 3}
    };
    
    // replace 'x' by x-value
    s = ReplaceAll(s, "x", to_string(x));
    
    // work with '-'
    bool numbers_before = false;
    int i = 0;
    while(i < s.length())
    {
        cout << "i = " << i << endl;
        if (isdigit(s[i]))
            numbers_before = true;
        i++;
        if (s[i] == '-' && numbers_before)
        {
            s.insert(i, "+");
            i++;
        }
    }
    
    for (int i = 0; i < s.length(); ++i)
    {
        if (isdigit(s[i]) || s[i] == '.' || s[i] == '-')
        {
            // It's a number
            if (prev.length() == 1)
            {
                if (isdigit(prev[0]) || prev[0] == '.' || prev[0] == '-')
                {
                    number = prev + string(1, s[i]);
//                    cout << "Number: " << number << endl;
                    queue.pop_back();
                    queue.push_back(number);
                    prev = number;
                }
                else
                {
                    queue.push_back(string(1, s[i]));
                    prev = s[i];
                }
            }
            else
            {
                number = prev + string(1, s[i]);
//                cout << "Number: " << number << endl;
                queue.pop_back();
                queue.push_back(number);
                prev = number;
            }
        }
        else
        {
            if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^')
            {
                // It's an operator
                while (!stack.empty())
                {
                    if (precedence[stack.back()] > precedence[s[i]])
                    {
                        queue.push_back(string(1, stack.back()));
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
                            queue.push_back(string(1, stack.back()));
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
            prev = s[i];
        }
    }
    
    // While there's operators on the stack, pop them to the queue
    while (!stack.empty())
    {
        queue.push_back(string(1, stack.back()));
        stack.pop_back();
    }
    
    return queue;
}

double ReversePolish(list<string> queue)
{
    /**
     * Reverse polish
     */
    double a, b, result = 0;
    list<double>fstack;
    string str;
    
    for (list<string>::iterator p = queue.begin(); p != queue.end(); ++p)
    {
        str = *p;
        if (str[0] == '-')
        {
            fstack.push_back(stod(str));
            continue;
        }
        if (isdigit(str[0]))
            fstack.push_back(stod(str));
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
            switch (str[0])
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

string ReplaceAll(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

// 4*8/(9-5)
// 6^2-9*2/3
// 7.56^2-9.1*56
