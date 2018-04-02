#include "HashTable.h"
#include <iomanip>  // for use of setw
#include <cstring>
#include <cmath>
using namespace std;

// a new hash table whose capacity is the prime number closest to
// and greater that 2 times the capacity of the old hash table
// replaces the old hash table and all items from the old hash table
// are rehashed (re-inserted) into the new hash table
// (the old hash table is discarded - memory returned to heap)
// (HINT: put next_prime and insert to good use)
void HashTable::rehash()
{
    // find a new prime size that is at least 2* the size of the
    // old size
    size_type old_cap = capacity;
    while (cap() <= (2 * old_cap))
        capacity = next_prime(cap() + 1);

    // preserve the old hash table and create a new table
    // with the new capacity
    Item* oldData = data;
    data = new Item[cap()];

    // insert each word in the old hash table into the new
    // hash table
    for (size_type i = 0; i < old_cap; i++)
    {
        if (strcmp(oldData[i].word, "") != 0)
            insert(oldData[i].word);
    }
    // deallocate the old hash table
    delete [] oldData;
}

// returns true if cStr already exists in the hash table,
// otherwise returns false
bool HashTable::exists(const char* cStr) const
{
   for (size_type i = 0; i < capacity; ++i)
      if ( ! strcmp(data[i].word, cStr) ) return true;
   return false;
}

// returns true if cStr can be found in the hash table
// (MUST use hashing technique, NOT doing a linear search
// like what is done in exists above),
// otherwise return false
// CAUTION: major penalty if not using hashing technique
bool HashTable::search(const char* cStr) const
{
    /** Note - see insert function for explanation of why
      * hash(cStr) + 1 is used **/

    // find the home location of the word to look for
    size_type i = hash(cStr) + 1;
    size_type loc0 = i;
    size_type loci = i;

    // as long as the word is not yet found, do quadratic
    // probing to find it
    while (strcmp(data[loci - 1].word, cStr) != 0)
    {
        // qhadratic probing
        loci = (loci + (i * i)) % cap();

        // if the end of the table is reached, return false
        if (i == loc0)
            return false;
    }
    // if the while loop finishes before the end of the table
    // is reached, the word was found, so return true
    return true;
}

// returns load-factor calculated as a fraction
double HashTable::load_factor() const
{ return double(used) / capacity; }

// returns hash value computed using the djb2 hash algorithm
// (2nd page of Lecture Note 324s02AdditionalNotesOnHashFunctions)
HashTable::size_type HashTable::hash(const char* word) const
{
    // djb2 hash function
    size_type hash = 5381;
    int c;
    while(c = *word++) hash = ((hash << 5) + hash) + c;

    if (hash > cap())
        hash = hash % cap();
    return hash;
}

// constructs an empty initial hash table
HashTable::HashTable(size_type initial_capacity)
          : capacity(initial_capacity), used(0)
{
   if (capacity < 11)
      capacity = next_prime(INIT_CAP);
   else if ( ! is_prime(capacity))
      capacity = next_prime(capacity);
   data = new Item[capacity];
   for (size_type i = 0; i < capacity; ++i)
      strcpy(data[i].word, "");
}

// returns dynamic memory used by the hash table to heap
HashTable::~HashTable() { delete [] data; }

// returns the hash table's current capacity
HashTable::size_type HashTable::cap() const
{ return capacity; }

// returns the # of hash-table slots currently in use (non-vacant)
HashTable::size_type HashTable::size() const
{ return used; }

// graphs a horizontal histogram that gives a decent idea of how
// items are distributed over the hash table
void HashTable::scat_plot(ostream& out) const
{
   out << endl << "Scatter plot of where hash table is used:";
   size_type lo_index = 0,
             hi_index = capacity - 1,
             width;
   if (capacity >= 100000)
      width = capacity / 250;
   else if (capacity >= 10000)
      width = capacity / 25;
   else
      width = capacity / 10;
   size_type max_digits = size_type( floor( log10(hi_index) ) + 1 ),
             label_beg  = lo_index,
             label_end  = label_beg + width - 1;
   for(label_beg = lo_index; label_beg <= hi_index; label_beg += width)
   {
      out << endl;
      if( label_end > hi_index)
         out << setw(max_digits) << label_beg << " - " << setw(max_digits) << hi_index << ": ";
      else
         out << setw(max_digits) << label_beg << " - " << setw(max_digits) << label_end << ": ";
      size_type i = label_beg;
      while ( i <= label_end && i <= hi_index)
      {
         if (data[i].word[0] != '\0')
            out << '*';
         ++i;
      }
      label_end = label_end + width;
   }
   out << endl << endl;
}

// dumping to out contents of "segment of slots" of the hash table
void HashTable::grading_helper_print(ostream& out) const
{
   out << endl << "Content of selected hash table segment:\n";
   for (size_type i = 10; i < 30; ++i)
      out << '[' << i << "]: " << data[i].word << endl;
}

// cStr (assumed to be currently non-existant in the hash table)
// is inserted into the hash table, using the djb2 hash function
// and quadratic probing for collision resolution
// (if the insertion results in the load-factor exceeding 0.45,
// rehash is called to bring down the load-factor)
void HashTable::insert(const char* cStr)
{
    /** Note: +1 is added to the hash value to ensure that
      * quadratic probing does not run into any issues. Without
      * this added +1, probing would hang up when i = 0. In this
      * case, loci + (i * i) will always be loci, since 0 * 0 = 0.
      * adding +1 to the calculated hash value ensures that even if
      * the hash value is 0, there is still some non-0 representation
      *of it to use for quadratic probing. Similarly, when using the
      * resulting value as an index, 1 is subtracted from the value.
      * Essentially, this shifts the indices over by 1 to avoid 0 while
      * probing, and back over to include 0 when accessing the table.
    **/

    // rehash if load factor becomes too high
    if ((size() + 1) / static_cast<double>(cap()) > 0.45)
        rehash();

    // find the has value + 1 for the word to be inserted.
    size_type i = hash(cStr) + 1;

    // this prints out some of the words being added, since loading
    // the large dictionary may take several minutes to complete.
    // This way, the loading is not silent and you get a sort of
    // primitive "progress bar" so you know the program is not frozen.
    // Not all the words are printed in order, probably due to the
    // buffer not always being able to keep up with the hashing.
    if (i % 337 == 0)
        cout << "inserting " << cStr << "...\n";

    // set the home location to the hash value + 1
    size_type loci = i;
    // do quadratic probing for collisions
    while (strcmp(data[loci - 1].word, "") != 0)
        loci = (loci + (i* i)) % cap();

    // once a suitable location has been found, copy the word
    // into the character array for that location.
    int j = 0;
    while (cStr[j] != '\0')
    {
        data[loci - 1].word[j] = cStr[j];
        j++;
    }
    // don't forget the null terminator (he'll be back)
    data[loci - 1].word[j + 1] = '\0';
    // increment used to indicate that a word has been added
    // to the table
    used++;
}

// adaption of : http://stackoverflow.com/questions/4475996
//               (Howard Hinnant, Implementation 5)
// returns true if a given non-negative # is prime
// otherwise returns false
// making use of following:
//   if a # is not divisible by 2 or by 3, then
//   it is of the form 6k+1 or of the form 6k+5
bool is_prime(HashTable::size_type x)
{
   if (x <= 3 || x == 5) return true;
   if (x == 4 || x == 6) return false;

   HashTable::size_type inc = 4;
   for (HashTable::size_type i = 5; true; i += inc)
   {
      HashTable::size_type q = x / i;
      if (q < i)
         return true;
      if (x == q * i)
         return false;
      inc ^= 6;
   }
   return true;
}

// adaption of : http://stackoverflow.com/questions/4475996
//               (Howard Hinnant, Implementation 5)
// returns the smallest prime that is >= x
HashTable::size_type next_prime(HashTable::size_type x)
{
    switch (x)
    {
    case 0:
    case 1:
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
    case 5:
        return 5;
    }
    HashTable::size_type k = x / 6;
    HashTable::size_type i = x - 6 * k;
    HashTable::size_type inc = i < 2 ? 1 : 5;
    x = 6 * k + inc;
    for (i = (3 + inc) / 2; !is_prime(x); x += i)
        i ^= 6;
    return x;
}
