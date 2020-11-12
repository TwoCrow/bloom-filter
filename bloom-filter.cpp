#include <iostream>
#include <string>

// ========================================
//   C++ Implementation of a Bloom Filter
// ========================================
//			by Patrick Sherbondy
// ========================================
//			    Bloom Filters
// ========================================
// A Bloom filter is a very unique and interesting probabilistic data structure. It is comprised solely of an array of boolean hashtables, and only allows for the input
// or lookup of values. Traditional Bloom filters do NOT support deletion.

// Bloom filters work by taking a value and assigning it a hashcode integer. The filter itself is comprised of multiple booleean hashtables. With the hashcode in hand,
// it visits each successive hashtable and mods the hashcode by the hashtable's length, and marks the resulting index as "true". By visiting and marking a single space
// on each hashtable, each value leaves a unique fingerprint on the filter that provides hints that it is present in the filter.

// The purpose of this is to save on space, making Bloom filters a great choice for processing an incredibly large amount of data that cannot be stored efficiently. Booleans
// certainly do not take up as much space as an integer, and so allowing each integer to leave a unique fingerprint effectively communicates the same information at a lesser
// cost. Of course, there is a downside.

// This data structure is classed as "probabilistic" because it can lie to you. It is possible to check if a value exists in the Bloom filter without it ever being added,
// and having the filter state that it is present. False positives are the largest downside of Bloom filters, but having many hashtables of varying sizes can help eliminate
// or diminish the possibility of getting a false positive response.

// ========================================
//			    This Program
// ========================================
// This program only allows for strings to be passed into the Bloom filter. You can play with your own set of strings if you head over to main() and edit inputStrings[]!
// After running the program, you'll get a short report stating that the passed strings probably exist within the filter. Then, a string not added to the filter is passed,
// and stated to (hopefully) not exist within the filter.

class BloomFilter
{
	private:
		// Stores a reference to the hash tables that comprise the entirety of the Bloom filter.
		bool** tables;
		int* sizes;
		int tableSize;
		
	public:
		// Constructor. Takes in an array consisting of the capacities of each hashtable, along with a number for the total number of tables.
		BloomFilter(int tableSizes[], int tableCount)
		{
			tables = new bool*[tableCount];
			sizes = new int[tableCount];
			tableSize = tableCount;

			// Set up the array of hashtables and the hashtables themselves.
			for (int i = 0; i < tableCount; i++)
			{
				tables[i] = new bool[tableSizes[i]];

				// Initialize the hashtables with false values.
				for (int j = 0; j < tableSizes[i]; j++)
				{
					tables[i][j] = false;
				}

				// Saves the size of each individual hashtable.
				sizes[i] = tableSizes[i];
			}
		}

		// Returns a reference to the tables class member.
		bool** getTables()
		{
			return tables;
		}

		// Returns the current length of the hashtables that comprise the Bloom filter.
		int getTableSize()
		{
			return tableSize;
		}

		// Adds a string - key - to the Bloom filter.
		void add(std::string key)
		{
			// Create a hashcode for the string.
			int value = createHashCode(key);

			// Visit each selected space in the filter and mark it "true".
			for (int i = 0; i < tableSize; i++)
			{
				tables[i][value % sizes[i]] = true;
			}
		}

		// Determines if the string - key - currently exists in the Bloom filter. Returns false if it doesn't.
		bool contains(std::string key)
		{
			// Create a hashcode for the string.
			int value = createHashCode(key);

			// Visit each selected space in the filter.
			for (int i = 0; i < tableSize; i++)
			{
				// If we encounter a single place where the key's rightful spot is false, that means that the key was never passed in to the
				// Bloom filter to begin with. Thus, we return false, as there's no possible way the key ever existed in the filter.
				if (tables[i][value % sizes[i]] == false)
				{
					return false;
				}
			}

			// Otherwise, return true, because the string found a 'true' value in all the correct spots.
			return true;
		}

		// Creates an integer hashcode for use in hashing the strings.
		// This uses the typical polynomial rolling hash function to create unique hashcodes.
		int createHashCode(std::string key)
		{
			// Initialize the values.
			// p is 53 to support both upper and lowercase.
			int p = 53;
			// m should always be a large prime number.
			int m = 1e9 + 9;
			int hashcode = 0;
			// the beginning power, which will grow with each successive processed character. 
			int power = 1;

			// For each character in the string, calculate a new integer based on the current character.
			for (char c : key)
			{
				// (c - 'a' + 1) provides a value based on the alphabet.
				hashcode = (hashcode + (c - 'a' + 1) * power) % m;
				power = (power * p) % m;
			}

			return hashcode;
		}
};

int main()
{
	// The sizes of each individual hashtable. I used prime numbers to prevent the possibility of a bunch of insertions at index 0, since the filter mods the hashcode
	// by its size.
	int tableSizes[] = {11, 13, 17, 19, 23, 29, 31, 37};
	int tableCount = sizeof(tableSizes) / sizeof(*tableSizes);

	// Feel free to mess around with this array of strings, as they're the input for the Bloom filter.
	std::string inputStrings[] = {"Patrick", "Cody", "Vandy", "Alex", "Jess", "CaptainJackSparrow", "HollowKnight", "Coding", "Coder", "Code"};

	// Instantiate the filter.
	BloomFilter filter = BloomFilter(tableSizes, tableCount);
	
	// Add all the input strings to the filter.
	for (std::string s : inputStrings)
	{
		filter.add(s);
	}

	// Search the filter for each of the added strings.
	for (std::string s : inputStrings)
	{
		if (filter.contains(s))
		{
			std::cout << "The Bloom filter probably contains " << s << "\n";
		}
		else
		{
			std::cout << "ERROR: An added string returned false! That can't happen!\n";
		}
	}

	// This string was not added to the Bloom filter, to show that it's possible to return false.
	std::string excludedString = "Loner";

	if (!filter.contains(excludedString))
	{
		std::cout << "The string, " << excludedString << ", definitely does not exist in the Bloom filter.\n";
	}
	else
	{
		std::cout << "ERROR: Bloom filter returned a false positive!\n";
	}
}
