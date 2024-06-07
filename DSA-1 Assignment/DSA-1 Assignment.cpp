/*
References

References to the theory of how good suffix and the kmp algorithm works were used for understanding the problem.

Boyyer moore good suffix:

GeeksForGeeks -> Boyer Moore Algorithm | Good Suffix heuristic (2019) Available at: https://www.geeksforgeeks.org/boyer-moore-algorithm-good-suffix-heuristic/?ref=lbp . [Accessed 10th December]

KMP: 

Educba -> KMP Algorithm (No date). Available at: https://www.educba.com/kmp-algorithm/ . [Accessed 15th December]

Small text:

Text from a website test example. 

indstate -> Knuth-Morris-Pratt Algorithm (2011). Available at: http://cs.indstate.edu/~kmandumula/presentation.pdf . [Accessed 15th December]


Medium text:

A random word generator was used for the text. 

Word counter -> Random word generator (No date). Available at: https://wordcounter.net/random-word-generator. [Accessed 15th December]

Large text:

Science Direct -> Penetration Enhancers (2012). Available at: https://www-sciencedirect-com.libproxy.abertay.ac.uk/science/article/pii/S0169409X1200292X?via%3Dihub . [Accessed 27th December]

---------------

All code marked with [--Code print--] is for code which displays information of the program running. This can be commented out when needed for benchmarking.


*/

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <deque>
#include <thread>
#include <chrono>

#include "utilities.h"


using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::cout;
using std::endl;
using std::this_thread::sleep_for;
using namespace std;
using std::list;
using std::string;

//Type of clock
typedef std::chrono::steady_clock the_clock;

template <typename T>
Position bm_string_change(T& results, string& pat, string& text) { // Boyer moore skip search (string entered is checked by being fully uppercase and first letter uppercase as well)
	Position pat_len = pat.size();
	Position text_len = text.size();

	//make the text and pattern all lowercase.
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	transform(pat.begin(), pat.end(), pat.begin(), ::tolower);

	//Lookup table for Good suffix rule. Map to hold the places of all matches in the pattern. 

	unordered_map<string, int> places; // Map to hold places of matches

	string holdString;
	string comparisonString;

	for (int j = 0; j < pat_len; j++) //For all the patterns in string 
	{
		holdString = pat[(pat_len - 1) - j] + holdString;

		//cout << "\n"; //[--Code print--]
		//cout << "holdString Testing: " << holdString << "\n"; //[--Code print--]

		for (int u = 0; u < pat_len; u++) // Loop from start of pattern to end e.g. Dundee -> 0 -> D, 1 -> u etc.
		{
			int num = 0;
			comparisonString = comparisonString + pat[u];

			if (size(holdString) != 1) // Add more characters to the comparison to match the size of the current held string
			{
				num = 1;
				for (int e = 0; e < size(holdString) - 1; e++)
				{
					if (u + num < pat_len)
					{
						comparisonString = comparisonString + pat[u + num];
						num++;
					}

				}
			}

			if(size(comparisonString) != size(holdString))
			{
				comparisonString = "";
				//continue;
			}
			else
			{
				if (holdString == comparisonString && u != pat_len - 1 - j) // If match is made. Check whether the match has already been made. If match already made, 
				{
					//cout << "Success!\n";//[--Code print--]
					if (size(comparisonString) == 1)
					{

						string s(1, pat[u]);
						if (places.count(s) == 0) 
						{
							places[s] = u; // Present
						}
						else
						{
							//Do nothing Not present
						}

					}
					else
					{
						if (places.count(holdString) == 0) 
						{
							places[holdString] = u;//Not present
						}
						else
						{
							//Do nothing Present
						}
					}
				}

				//cout << "comparisonString Testing: " << comparisonString << "\n";//[--Code print--]
				comparisonString = "";
			}
		}


	}




	 

	//Lookup table for bad characters.
	int skip[256];
	for (int i = 0; i < 256; i++) // Set every value in array to -1
	{
		skip[i] = -1;
	}
	for (int i = 0; i < pat_len; i++) // Set characters equal to the last occurence of the character e.g. abc: c = 2, b = 1, a = 0.
	{
		skip[int(pat[i])] = i;
		//cout << pat[i] << skip[int(pat[i])] << "\n";//[--Code print--]
	}


	//cout << "\n\nGood Suffix rule\n\n"; //[--Code print--]

	//Good suffix rule
	Position shift = 0; // Position in text

	//Comparing shift position between bad and good rule
	int valueBad = 0;
	int valueGood = 0;

	for (Position i = 0; shift < text_len - pat_len; ++i) // loop around all text length
	{
		int j = pat_len - 1; // character position
		//show_context(text, shift);//[--Code print--]
		for (int i = 0; j >= 0 && pat[j] == text[shift + j]; i++) // For the length of the pattern while both the pattern is matching the text, reduce j. 
		{
			j--;
			
		}
		if (j < 0) // If j is -1 from the loop above, a match has been found and the shift postition is stored
		{
			results.push_back(shift);

			if (shift + pat_len < text_len) // push shift to the next space past the found word.
			{
				shift += pat_len - skip[text[shift + pat_len]];
			}
			else
			{
				break;
			}
		}
		else // Skip over values. Max to skip over the maximum number of items.
		{

			//Bad character
			int badShifter = max(1, j - skip[text[shift + j]]); 
			if (badShifter > 1000)
			{
				badShifter = pat_len;
			}
			//cout << "Can shift " << badShifter << " places with bad rule\n";//[--Code print--]

			//Good suffix 
			int oldJValue = j; //Hold old value of j
			j++; 
			string holdtext;
			while (j < pat_len)
			{
				holdtext = holdtext + pat[j];
				j++;

			}
			
			if (oldJValue < 0)
			{
				//cout << "J value: " << oldJValue << " Pattern of which is " << pat[oldJValue - 1] << "\n";//[--Code print--]
			}

			if (holdtext != "")
			{
				if (places.find(holdtext)==places.end())//Check if at end and the pattern doesn't exist
				{
					//cout << "Doesn't exist\n"; //[--Code print--]
					shift += j - 1;
				}
				else //Pattern exists, Shift by largest value against each rule.
				{
					int pos = places[holdtext];
					//cout << "Position of another of " << holdtext << " is " << pos << "\n";//[--Code print--]
					int goodShifter = (oldJValue + 1) - pos;
					//cout << "Can shift " << goodShifter << " places with good rule\n";//[--Code print--]
					if (goodShifter == badShifter)
					{
						shift += badShifter; // Doesn't matter which to shift by
					}

					if (goodShifter > badShifter)
					{
						shift += goodShifter;
					}
					else if (goodShifter < badShifter)
					{
						shift += badShifter;
					}
				}
				
			}
			else
			{
				shift += badShifter;
			}

			//cout << "Aligning bad character\n";//[--Code print--]
		}


	}
	return 0; // return what has been found. If nothing, -1 is returned automatically (Or 0).
	

}

template <typename T>
Position kmp_string_change(T& results, string& pat, string& text) {


	//cout << "\n"; //[--Code print--]
	//cout << "KMP Algorithm\n"; //[--Code print--]
	//cout << "\n";//[--Code print--]
	Position pat_len = pat.size();
	Position text_len = text.size();

	//make the text and pattern all lowercase.
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	transform(pat.begin(), pat.end(), pat.begin(), ::tolower);
	
	//Preprocessing for KMP -> For every pattern say dundee, check starting with 2 letters for the largest matching proper prefix and suffix and store if match the value to go to at that 

	unordered_map<string, int> place; //Map to hold where in string to go to with the pattern found

	string holdString; //What is currently in the string
	string comparisonString; // Comparing the string
	

	string prefix; //The prefix you're comparing
	string suffix; //The suffix you're comparing

	string compareSuf; //Compare for largest match
	string comparePre; // Compare for largest match

	//cout << "\n";//[--Code print--]
	for (int i = 0; i < pat_len; i++)
	{
		int suffixHold = size(holdString);
		int prefixHold = size(holdString);

		holdString = holdString + pat[i];
	
		//cout << "holdString: " << holdString << "\n";//[--Code print--]
		if (prefixHold > 0)
		{
			prefix = prefix + holdString[prefixHold -1];
			//cout << "Prefix: " << prefix << "\n";//[--Code print--]
		}

		
		if (suffixHold > 0)
		{
			suffix = suffix + holdString[suffixHold];
			//cout << "Suffix: " << suffix << "\n";//[--Code print--]
		} 


		//Check all proper Prefix and Suffix to the biggest in holdString for match to be stored

		for (int x = 0; x < prefixHold; x++) //Loop through all options for length
		{
			comparePre = comparePre + holdString[x]; //Iterate proper prefix
			//cout << "compare Prefix: " << comparePre << "\n";//[--Code print--]
			//cout << "Position: " << x << "\n";//[--Code print--]


			compareSuf = holdString[suffixHold - x] + compareSuf; // Iterate proper suffix 
			//cout << "compare Suffix: " << compareSuf << "\n";//[--Code print--]
			//cout << "Position: " << suffixHold-x << "\n";//[--Code print--]

			if (comparePre == compareSuf)
			{
				//cout << "Prefix and suffix match found!\n";//[--Code print--]
				place[holdString] = x; //   = suffixHold -x (For holding suffix value) ||  = x (For holding prefix value)
				
			}
		}

		compareSuf = "";
		comparePre = "";


	}


	//KMP algorithm after preprocessing

	bool check = false;
	

	int j = 0; // Position in pattern


	for (Position i = 0; i < text_len; ++i) // loop around all text length. i = position in text
	{		
		string holdText = ""; // String to hold the matching characters
		//show_context(text, i);//[--Code print--]

		if (text[i] == pat[j]) //Check if there is a match, if so, go onto a more thorough search
		{
			for (int x = i; text[x] == pat[j] && x < text_len; x++) // Only iterates if pattern matches the current position in text
			{
				holdText = holdText + pat[j];
				j++;

				if (j == pat_len) //If there is a match...
				{
					check = true;

					//When skipping large chuncks, make sure the correct position in the text is identified for where the start of the match is.
					int countAfter = 0;
					int countBefore = 0;
					if (size(holdText) != pat_len)
					{
						countAfter = size(holdText);
						countBefore = size(holdText);
						while (countAfter != pat_len)
						{
							countAfter++;
						}
						
					}
					int remove = countBefore - countAfter;

					//Add the match to results with the position. 
					results.push_back(i + remove);

					//Skip the length of the match

					i = i + size(holdText) - 1;
				}
			}

			if (check == false) //Display a fail
			{

				//Check how far to move past the mismatch or not if proper suffix and prefix

				if (place[holdText] != 0) //Check for proper prefix and suffix then move along 
				{
					//cout << "Proper prefix and suffix match found.\n";//[--Code print--]
					i =  i + size(holdText) -1; //i + place[holdText] - 1 // Iterate along the text.

					//Make a text to hold the previous match for next iteration

					j = place[holdText] + 1; // Iterate to the next pattern comparison, skipping the match already made.

				}
				else // No match available. Iterate as normal.
				{
					//cout << "No match. Iterating as normal\n";//[--Code print--]
					j = 0;
					i = i + size(holdText) -1;
				}

			}
			else //Display what is being held with no fail message
			{
				check = false;
				//cout << "Hold text: " << holdText << "\n";//[--Code print--]
				j = 0;
			}
		}
		else
		{
			//If there is no match, check j to see if the length is greater than 0, make hold text and check for matches and move by that. Otherwise iterate as normal.


			//Loop to make the holdText string
			for (int y = 0; y < j; y++)
			{
				holdText = holdText + pat[y];
			}
			//cout << "holdText value at this point: " << holdText << "\n";//[--Code print--]

			if (place[holdText] != 0) // Check if there is a common suffix and prefix
			{
				//If so, exact same match as done before.
				//cout << "Proper prefix and suffix match found.\n";//[--Code print--]
				i = size(holdText) - 1; //i + place[holdText] - 1 // Iterate along the text.
				j = 0;
				j = place[holdText] - 1; // Iterate to the next pattern comparison, skipping the match already made.

			}
			else // Otherwise iterate as normal
			{
				//cout << "Iterating as normal\n";//[--Code print--]
				if (holdText != "")
				{
					i = i - 1;
				}
				
				j = place[holdText];

			}
		}

	}

	return 0;
}

template <typename T>
void ShowValues(const T& collection) {
	for (auto item : collection) {
		//cout << " " << item;//[--Code print--]
	}
	//cout << "\n";//[--Code print--]
}


int main(int argc, char* argv[]) {

	int medianBm[1001] = {};
	int medianKMP[1001] = {};



	
	int option = 0;

	while (option > 3 || option <= 0)
	{
		cout << "(1) For small text file.\n";
		cout << "(2) For medium text file.\n";
		cout << "(3) For large text file.\n";
		cout << "Option: ";
		cin >> option;
	} 

	


	for (int i = 0; i < 1001; i++)
	{
		ofstream file("outputFile.txt");


		string text;

		
		string pat;

		switch (option)
		{
		case 1:
			load_file("small.txt", text); //Small.txt
			pat = "ababaca";//Small.txt
			break;
		case 2:
			load_file("medium.txt", text); //Medium.txt
			pat = "parti";//Medium.txt
			break;
		case 3:
			load_file("large.txt", text); //Large.txt
			pat = "penetration";//Large.txt
			break;
		}
		


		/*
		Manual loading of files
		*/
		//load_file("small.txt", text); //Small.txt
		//load_file("medium.txt", text); //Medium.txt
		//load_file("large.txt", text); //Large.txt

		//string pat = "ababaca";//Small.txt
		//string pat = "parti";//Medium.txt
		//string pat = "penetration";//Large.txt

		list<Position> results;
		deque<int>queue;

		the_clock::time_point start = the_clock::now();

		Position pos = bm_string_change(results, pat, text);

		the_clock::time_point end = the_clock::now();

		auto time_taken_bm = duration_cast<microseconds>(end - start).count();


		the_clock::time_point start1 = the_clock::now();

		Position pas = kmp_string_change(results, pat, text);

		the_clock::time_point end1 = the_clock::now();

		auto time_taken_kmp = duration_cast<microseconds>(end1 - start1).count();

		ShowValues(results);
		for (auto item : results)
		{
			pos = item;
			//cout << "Found '" << pat << "' at position " << pos << ":\n"; //[--Code print--]
			//show_context(text, pos); //[--Code print--]

			queue.push_back(pos);

		}
		int halfQ = queue.size() / 2;
		while (queue.size() > halfQ)
		{
			//cout << queue.back() << "\n";//[--Code print--]
			text.replace(queue.back(), size(pat), " "); // Censor and remove word from text.
			queue.pop_back();
		}


		file << text; // Place new updated text into file.
		file.close();

		//cout << "It took: " << time_taken_bm << " milliseconds for boyer moore algorithm to finish.\n";//[--Code print--]
		//cout << "It took: " << time_taken_kmp << " milliseconds for kmp algorithm to finish.\n";//[--Code print--]

		medianBm[i] = time_taken_bm;
		medianKMP[i] = time_taken_kmp;
	}

	// Place results into files

	ofstream results("results.csv");
	ofstream kmpData("kmpData.txt");
	ofstream boyerData("boyerData.txt");

	results << "Boyer-Moore";
	results << ",";
	results << "KMP";
	results << "\n";

	for (int i = 0; i < 1001; i++)
	{
		results << medianBm[i] << ",";
		results << medianKMP[i];
		results << "\n";

		boyerData << medianBm[i] << ",";

		kmpData << medianKMP[i] << ",";

	}
	



	return 0;
}



