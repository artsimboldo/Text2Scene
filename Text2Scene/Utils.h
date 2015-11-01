#pragma once
#include <vector>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <regex>
using namespace std;

namespace Text2Scene
{

	// remove accents in string
	static inline string& RemoveAccents(string& s)
	{
		s = regex_replace(s, regex("�|�|�|�|�"), "A");
		s = regex_replace(s, regex("�|�|�|�"), "E");
		s = regex_replace(s, regex("�|�|�|�"), "I");
		s = regex_replace(s, regex("�|�|�|�|�"), "O");
		s = regex_replace(s, regex("�|�|�|�"), "U");
		s = regex_replace(s, regex("�"), "C");
		s = regex_replace(s, regex("�"), "N");
		return s;
	}

	// make string to uppercase
	static inline void ToUpper(string& s)
	{
		transform(s.begin(), s.end(), s.begin(), ::toupper);
	}

	// make string to uppercase
	static inline void ToLower(string& s)
	{
		transform(s.begin(), s.end(), s.begin(), ::tolower);
	}

	// trim from start
	static inline string& TrimLeft(string &s) 
	{
			s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun(iswspace))));
			s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun(iswcntrl))));
			return s;
	}

	// trim from end
	static inline string& TrimRight(string &s) 
	{
			s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun(iswspace))).base(), s.end());
			s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun(iswcntrl))).base(), s.end());
			return s;
	}

	// trim from both ends
	static inline string& Trim(string &s) 
	{
		return TrimLeft(TrimRight(s));
	}

	template <class T>
	static inline void DeleteVector(vector<T*>* deleteme) 
	{
		while(!deleteme->empty())
		{
			delete deleteme->back();
			deleteme->pop_back();
		}
	}

}